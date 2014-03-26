#include <inttypes.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/boot.h>

#include <I2CSlave.h>
#include "../masterLib/include/I2CDeviceBootableRegisters.h"
#include "../slaveLib/include/shared.h"

#define I2C_BUFFER_LENGTH (SPM_PAGESIZE+1)

void onWriteFunction(uint8_t reg, uint8_t* buffer, size_t bufferSize);
size_t onReadFunction(uint8_t reg, uint8_t* buffer);

typedef struct {
  uint8_t mode;
  uint16_t pageNumber;
  bool lastWriteSuccess;
} AVRStatus_t ;

#define AVRStatus (*(AVRStatus_t*)0x120) 
#define AVRI2CBOOTLOADER_VERSION 0x01

#define IN_BOOT_LOADER() (AVRStatus.mode == I2C_DEVICE_BOOTABLE_STATUS_BOOT_LOADER)
#define IN_APPLICATION() (AVRStatus.mode == I2C_DEVICE_BOOTABLE_STATUS_APPLICATION)

bool programApplication(uint8_t* buffer, size_t bufferSize);

int main()
{
  MCUCR = (1 << IVCE);
  MCUCR = (1 << IVSEL);

  AVRStatus.mode = I2C_DEVICE_BOOTABLE_STATUS_BOOT_LOADER;
  AVRStatus.pageNumber = 0;
  AVRStatus.lastWriteSuccess = false;

  I2CCallbacks.onReadFunction = NULL;
  I2CCallbacks.onWriteFunction = NULL;

  i2CSlaveInit(64, onReadFunction, onWriteFunction, I2C_BUFFER_LENGTH);

  while(1);

  return 0;
}

void onWriteFunction(uint8_t reg, uint8_t* buffer, size_t bufferSize)
{
  switch(reg)
  {
    case I2C_DEVICE_BOOTABLE_REGISTER_BOOT_STATUS:
    {
      switch(buffer[0])
      {
	case I2C_DEVICE_BOOTABLE_STATUS_BOOT_LOADER:
	{
	  if(IN_APPLICATION())
	  {
	    AVRStatus.mode = I2C_DEVICE_BOOTABLE_STATUS_BOOT_LOADER;

	    sei();
	    i2cPostOnWrite();

	    //TODO: use Makefile variable
	    asm("jmp 28762");

	    while(1);
	  }

	  break;
	}
	case I2C_DEVICE_BOOTABLE_STATUS_APPLICATION:
	{
	  if(IN_BOOT_LOADER())
	  {
	    AVRStatus.mode = I2C_DEVICE_BOOTABLE_STATUS_APPLICATION;
  
	    sei();
	    i2cPostOnWrite();

	    MCUCR = (1 << IVCE);
	    MCUCR = 0;
	    sei();

	    asm("jmp 0000");

	    while(1);
	  }

	  break;
	}
	default:
	{
	  break;
	}
      }
      
      break;
    }
    case I2C_DEVICE_BOOTABLE_REGISTER_PROGRAM_PAGE_NUMBER:
    {
      AVRStatus.pageNumber = (buffer[1] << 8) | buffer[0];
      break;
    }
    case I2C_DEVICE_BOOTABLE_REGISTER_PROGRAM_PAGE_DATA:
    {
      if(IN_BOOT_LOADER())
      {
	AVRStatus.lastWriteSuccess = programApplication(buffer, bufferSize);
      }
      else
      {
	AVRStatus.lastWriteSuccess = false;
      }

      break;
    }
    default:
    {
      if(IN_APPLICATION() && (I2CCallbacks.onWriteFunction != NULL))
      {
	I2CCallbacks.onWriteFunction(reg, buffer, bufferSize);
      }
    
      break;
    }
  }
}

size_t onReadFunction(uint8_t reg, uint8_t* buffer)
{
  switch(reg)
  {
    case I2C_DEVICE_BOOTABLE_REGISTER_BOOT_MAGIC:
    {
      buffer[0] = I2C_DEVICE_BOOTABLE_BOOT_MAGIC;
      return 1;
    }
    case I2C_DEVICE_BOOTABLE_REGISTER_BOOT_STATUS:
    {
      buffer[0] = AVRStatus.mode;
      return 1;
    }
    case I2C_DEVICE_BOOTABLE_REGISTER_VERSION:
    {
      buffer[0] = AVRI2CBOOTLOADER_VERSION;
      return 1;
    }
    case I2C_DEVICE_BOOTABLE_REGISTER_PAGE_SIZE:
    {
      buffer[0] = SPM_PAGESIZE;
      return 1;
    }
    case I2C_DEVICE_BOOTABLE_REGISTER_PROGRAM_PAGE_NUMBER:
    {
      buffer[0] = (AVRStatus.pageNumber & 0xFF);
      buffer[1] = ((AVRStatus.pageNumber >> 8) & 0xFF);
      return 2;
    }
    case I2C_DEVICE_BOOTABLE_REGISTER_PROGRAM_PAGE_DATA:
    {
      buffer[0] = AVRStatus.lastWriteSuccess;
      return 1; 
    }
    default:
    {
      if(IN_APPLICATION() && (I2CCallbacks.onReadFunction != NULL))
      {
	return I2CCallbacks.onReadFunction(reg, buffer);
      }
      else
      { 
	return 0;
      }
    }
  }
}

bool programApplication(uint8_t* buffer, size_t bufferSize)
{
  uint32_t page = AVRStatus.pageNumber * SPM_PAGESIZE;

  cli();  
  eeprom_busy_wait();
  boot_page_erase(page);
  boot_spm_busy_wait();

  uint16_t i;

  for(i = 0; i < SPM_PAGESIZE; i+=2)
  {
    uint16_t v = *buffer++;
    v += (*buffer++) << 8;
    boot_page_fill (page + i, v);
  }

  boot_page_write(page);
  boot_spm_busy_wait();
  boot_rww_enable ();

  sei();

  return (bufferSize == SPM_PAGESIZE);
}


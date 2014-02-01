#include <inttypes.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/boot.h>

#include "I2CSlave/I2CSlave.h"
#include "../master/include/I2CDeviceBootableRegisters.h"

#define I2C_BUFFER_LENGTH 10 
//SPM_PAGESIZE

void onWriteFunction(uint8_t reg, uint8_t* buffer, size_t bufferSize);
size_t onReadFunction(uint8_t reg, uint8_t* buffer);

typedef struct {
  uint8_t mode;
  uint16_t pageNumber;
  bool lastWriteSuccess;
} AVRStatus_t ;

#define AVRStatus (*(AVRStatus_t*)0x120) 
#define AVRI2CBOOTLOADER_VERSION 0x01

void programApplication(uint8_t* buffer, size_t bufferSize);

int main()
{
  MCUCR = (1 << IVCE);
  MCUCR = (1 << IVSEL);

  AVRStatus.mode = I2C_DEVICE_BOOTABLE_STATUS_BOOT_LOADER;
  AVRStatus.pageNumber = 0;
  AVRStatus.lastWriteSuccess = false;

  i2CSlaveInit(0x40, onReadFunction, onWriteFunction, I2C_BUFFER_LENGTH);

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
	  if(AVRStatus.mode == I2C_DEVICE_BOOTABLE_STATUS_APPLICATION)
	  {
	    AVRStatus.mode = I2C_DEVICE_BOOTABLE_STATUS_BOOT_LOADER;

	    //TODO: use Makefile variable
	    asm("jmp 6144");
	  }
	}
	case I2C_DEVICE_BOOTABLE_STATUS_APPLICATION:
	{
	  if(AVRStatus.mode == I2C_DEVICE_BOOTABLE_STATUS_BOOT_LOADER)
	  {
	    AVRStatus.mode = I2C_DEVICE_BOOTABLE_STATUS_APPLICATION;

	    MCUCR = (1 << IVCE);
	    MCUCR = 0;

	    asm("jmp 0000");
	  }
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
      if(AVRStatus.mode == I2C_DEVICE_BOOTABLE_STATUS_BOOT_LOADER)
      {
	programApplication(buffer, bufferSize);
	AVRStatus.lastWriteSuccess = true;
      }
      else
      {
	AVRStatus.lastWriteSuccess = false;
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
      buffer[1] = ((AVRStatus.pageNumber >> 4) & 0xFF);
      return 2;
    }
    case I2C_DEVICE_BOOTABLE_REGISTER_PROGRAM_PAGE_DATA:
    {
      buffer[0] = AVRStatus.lastWriteSuccess;
      return 1; 
    } 
    default:
    {
      return 0;
    }
  }

  return 0;
}

void programApplication(uint8_t* buffer, size_t bufferSize)
{
  uint32_t page = AVRStatus.pageNumber;

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
}


#include <inttypes.h>
#include <avr/io.h>
#include <avr/boot.h>

#include "I2CSlave/I2CSlave.h"
#include "../master/include/I2CDeviceBootableRegisters.h"

#define I2C_BUFFER_LENGTH SPM_PAGESIZE

void onWriteFunction(uint8_t reg, uint8_t* buffer, size_t bufferSize);
size_t onReadFunction(uint8_t reg, uint8_t* buffer);

int main()
{
  i2CSlaveInit(0x40, onReadFunction, onWriteFunction, I2C_BUFFER_LENGTH);

  while(1);

  return 0;
}

void onWriteFunction(uint8_t reg, uint8_t* buffer, size_t bufferSize)
{
}

size_t onReadFunction(uint8_t reg, uint8_t* buffer)
{
  switch(reg)
  {
    case I2C_DEVICE_BOOTABLE_REGISTER_BOOT_MAGIC:
    {
      buffer[0] = 0xAE;
      return 1;
    }
    case I2C_DEVICE_BOOTABLE_REGISTER_BOOT_STATUS:
    {
      buffer[0] = 0x2;
      return 1;
    }
    case I2C_DEVICE_BOOTABLE_REGISTER_VERSION:
    {
      buffer[0] = 0x01;
      return 1;
    }
    case I2C_DEVICE_BOOTABLE_REGISTER_PAGE_SIZE:
    {
      buffer[0] = SPM_PAGESIZE;
      return 1;
    }
    case I2C_DEVICE_BOOTABLE_REGISTER_PROGRAM_PAGE_NUMBER:
    {
      buffer[0] = 0x0;
      buffer[1] = 0x0;
      return 2;
    }
    default:
    {
      return 0;
    }
  }
}

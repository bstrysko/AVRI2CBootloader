#include <AVRI2CBootloader/avr/application.h>

#include "./testApplication/include/TestDeviceRegisters.h"

#include <inttypes.h>
#include <stddef.h>
#include <stdlib.h>

size_t onI2CRead(uint8_t reg, uint8_t* buffer);
void onI2CWrite(uint8_t reg, uint8_t* buffer, size_t bufferSize);

#define a (*(volatile uint8_t*)0x140)

int main()
{
  a = 0;

  I2CCallbacks.onReadFunction = onI2CRead;
  I2CCallbacks.onWriteFunction = onI2CWrite;

  sei();

  while(1);

  return 0;
}

size_t onI2CRead(uint8_t reg, uint8_t* buffer)
{
  switch(reg)
  {
    case REGISTER_A:
    {
      buffer[0] = a;
      return 1;
    }
    case REGISTER_B:
    {
      buffer[0] = 111;
      return 1;
    }
  }

  return 0;
}

void onI2CWrite(uint8_t reg, uint8_t* buffer, size_t bufferSize)
{
  switch(reg)
  {
    case REGISTER_A:
    {
      a = buffer[0];
      break;
    } 
    case REGISTER_B:
    {
      a = buffer[0] + 5;
      break;
    } 
  }
}

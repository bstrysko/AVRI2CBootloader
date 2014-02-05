#include <AVRI2CBootloader/avr/application.h>

size_t onI2CRead(uint8_t reg, uint8_t* buffer);
void onI2CWrite(uint8_t reg, uint8_t* buffer, size_t bufferSize);

volatile uint8_t a;

int main()
{
  a = 40;

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
    case 0x20:
    {
      buffer[0] = a;
      return 1;
    }
    case 0x21:
    {
      buffer[0] = a;
      return 1;
    }
    default:
    {
      buffer[0] = 123;
      return 1;
    }
  }
}

void onI2CWrite(uint8_t reg, uint8_t* buffer, size_t bufferSize)
{
  switch(reg)
  {
    case 0x20:
    {
      a = buffer[0];
      break;
    }
    case 0x21:
    {
      a = buffer[0];
      break;
    }
  }
}

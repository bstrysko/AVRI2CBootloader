#include <AVRI2CBootloader/avr/application.h>

#include <inttypes.h>
#include <stddef.h>
#include <stdlib.h>
#include <util/delay.h>

size_t onI2CRead(uint8_t reg, uint8_t* buffer);
void onI2CWrite(uint8_t reg, uint8_t* buffer, size_t bufferSize);

#define a (*(uint8_t*)0x140)
#define b (*(uint8_t*)0x172)

int main()
{
  a = 10;
  b = 20;

  I2CCallbacks.onReadFunction = onI2CRead;
  I2CCallbacks.onWriteFunction = onI2CWrite;

  DDRC |= (1 << 1);
  sei();

  while(1)
  {
    DDRC &= ~((1 << 0));

    _delay_ms(100);
    _delay_ms(100);
    _delay_ms(100);
    _delay_ms(100);
    _delay_ms(100);

    DDRC |= (1 << 0);

    _delay_ms(100);
    _delay_ms(100);
    _delay_ms(100);
    _delay_ms(100);
    _delay_ms(100);
  }

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
      break;
    }
/*
    case 0x20:
    {
      buffer[0] = b;
      return 1;
      break;
    }
*/
    default:
    {
      buffer[0] = 123;
      return 1;
      break;
    }
  }

  return 0;
}

void onI2CWrite(uint8_t reg, uint8_t* buffer, size_t bufferSize)
{
  switch(reg)
  {
    case 0x20:
    {
      a = buffer[0];
      return;
      break;
    }
    case 0x21:
    {
      b = buffer[0];
      return;
      break;
    }
  }
}

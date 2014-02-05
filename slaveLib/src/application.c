#include <application.h>

ISR(TWI_vect)
{
  PROVIDE_ENTRY(TWI);
  asm("jmp 0x7060");
}


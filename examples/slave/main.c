#include <avr/interrupt.h>

void TWI_vect(void) __attribute__((naked));

ISR(TWI_vect)
{
  asm("jmp 0x7060");
}

int main()
{
  sei();

  while(1);

  return 0;
}

#ifndef _AVR_I2C_BOOTLOADER_AVR_APPLICATION_H_
#define _AVR_I2C_BOOTLOADER_AVR_APPLICATION_H_

#include <avr/interrupt.h>

#include "shared.h"

#if defined(__ASSEMBLER__) 
    #define PROVIDE_SYM(name)   GLOBAL_SYM name 
    #define REQUEST_SYM(name)   .global name 
    .macro GLOBAL_SYM _name 
        .global \_name 
    \_name: 
    .endm 
#else 
    #define PROVIDE_SYM(name)   __asm__ __volatile__(".global " #name "\n" #name ":\n"::) 
    #define REQUEST_SYM(name)   __asm__ __volatile__(".global " #name "\n"::) 
#endif 
#define PROVIDE_ENTRY(name)     PROVIDE_SYM(tag_##name) 
#define REQUEST_ENTRY(name)     REQUEST_SYM(tag_##name)

void TWI_vect(void) __attribute__((naked));

#endif

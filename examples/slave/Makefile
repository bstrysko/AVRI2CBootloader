PART=m328p
MCU=atmega328p
PROG=avrispMKII
F_CPU=8000000

FLAGS=-mmcu=$(MCU) -DF_CPU=$(F_CPU)UL -Os -Wall -Wl,--section-start=.data=0x800130

SRC= main.c
HDR=

all: avr.hex

%.hex: %.elf
	avr-objcopy -j .text -j .data -O ihex $< $@

avr.elf: $(SRC) $(HDR)
	avr-gcc $(FLAGS) $(SRC) -o avr.elf

flash:
	echo "Un-Implemented"

clean:
	rm avr.hex avr.elf

disasm:
	avr-objdump -s -m avr5 -D avr.hex

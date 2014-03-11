BOOTSTART=0x7000
PART=m328p
MCU=atmega328p

PROG=avrispMKII

F_CPU=8000000

FLAGS=-mmcu=$(MCU) -DF_CPU=$(F_CPU)UL -Os -Wall -Wl,--section-start=.text=$(BOOTSTART) -Wl,--section-start=.data=0x800130

LFUSE=0xE2
HFUSE=0xD8
EFUSE=0xFF

SRC= I2CSlave/I2CSlave.c main.c
HDR=

all: avr.hex

%.hex: %.elf
	avr-objcopy -j .text -j .data -O ihex $< $@

avr.elf: $(SRC) $(HDR)
	avr-gcc $(FLAGS) $(SRC) -o avr.elf

flash: avr.hex
	avrdude -F -p $(PART) -c $(PROG) -P usb -U flash:w:avr.hex

fuse:
	avrdude -p $(PART) -c $(PROG) -P usb -U lfuse:w:$(LFUSE):m -U hfuse:w:$(HFUSE):m -U efuse:w:$(EFUSE):m

clean:
	rm avr.hex avr.elf

disasm:
	avr-objdump -s -m avr5 -D avr.hex

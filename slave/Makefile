PART=m328p
MCU=atmega328p

PROG=avrispMKII

F_CPU=1000000
FLAGS=-mmcu=$(MCU) -DF_CPU=$(F_CPU)UL -Os -Wall

LFUSE=
HFUSE=
EFUSE=

SRC=main.c
HDR=

default: avr.hex

%.hex: %.elf
	avr-objcopy -j .text -j .data -O ihex $< $@

avr.elf: $(SRC) $(HDR)
	avr-g++ $(FLAGS) $(SRC) -o avr.elf

flash: avr.hex
	avrdude -F -p $(PART) -c $(PROG) -P usb -U flash:w:avr.hex

fuse:
	avrdude -p $(PART) -c $(PROG) -P usb -U lfuse:w:$(LFUSE):m -U hfuse:w:$(HFUSE):m -U efuse:w:$(EFUSE):m

clean:
	rm avr.hex avr.elf

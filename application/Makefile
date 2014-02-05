NAME=application

PART=m328p
MCU=atmega328p
PROG=avrispMKII
F_CPU=8000000

FLAGS=-mmcu=$(MCU) -DF_CPU=$(F_CPU)UL -Os -Wall -Wl,--section-start=.data=0x800144 -I/usr/include/ -L/usr/lib/AVRI2CBootloader/ -lAVRI2CBootloaderAVR -Wl,-u,tag_TWI

SRC= main.c
HDR=

all: $(NAME).hex

%.hex: %.elf
	avr-objcopy -j .text -j .data -O ihex $< $@

$(NAME).elf: $(SRC) $(HDR)
	avr-gcc $(FLAGS) $(SRC) -o $(NAME).elf

clean:
	rm $(NAME).hex $(NAME).elf

disasm:
	avr-objdump -s -m avr5 -D $(NAME).hex

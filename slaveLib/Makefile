LNAME=AVRI2CBootloader
LIBFILE=lib$(LNAME)AVR.a

PART=m328p
MCU=atmega328p

F_CPU=8000000
FLAGS=-I./include/ -mmcu=$(MCU) -DF_CPU=$(F_CPU)UL -Os -Wall -Wl,--section-start=.data=0x800140

SRC=src/application.c
OBJ = $(SRC:.c=.o)

all: $(LIBFILE)

$(LIBFILE): $(SRC)
	avr-gcc $(FLAGS) -c $(SRC) -o $(OBJ)
	avr-ar rcs $(LIBFILE) $(OBJ)

install:
	mkdir -p /usr/include/$(LNAME)/
	mkdir -p /usr/include/$(LNAME)/avr/
	cp include/* /usr/include/$(LNAME)/avr/
	mkdir -p /usr/lib/$(LNAME)/
	cp $(LIBFILE) /usr/lib/$(LNAME)/

clean:
	rm $(LIBFILE)

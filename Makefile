PROGRAMMER = usbasp
PORT       = USB
AVRDUDE    = /mnt/c/bin/WinAVR-20100110/bin/avrdude.exe
COMPILE    = avr-gcc -Wall -Os -mmcu=attiny13a

all: clean build upload

colors.h: gradient.go
	go run gradient.go > colors.h

flash.hex: xmas21.c colors.h
	$(COMPILE) -c xmas21.c -o xmas21.o
	$(COMPILE) -o xmas21.elf xmas21.o
	avr-objcopy -j .text -j .data -O ihex xmas21.elf flash.hex
	avr-size --format=avr --mcu=attiny13a xmas21.elf

upload:
	$(AVRDUDE) -v -p ATtiny13 -c usbasp -U flash:w:flash.hex:i

disassemble: xmas21.elf
	avr-objdump -d xmas21.elf

clean:
	rm xmas21.o
	rm xmas21.elf
	rm flash.hex

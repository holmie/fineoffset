all:
	avr-gcc -mmcu=atmega168 -std=c99 -Wall -Os -o fineoffset.elf fineoffset.c
	avr-objcopy -j .text -j .data -O ihex fineoffset.elf fineoffset.hex

install:
	avr-gcc -mmcu=atmega168 -std=c99 -Wall -Os -o fineoffset.elf fineoffset.c
	avr-objcopy -j .text -j .data -O ihex fineoffset.elf fineoffset.hex
	avrdude -p m168 -c usbasp -U flash:w:fineoffset.hex

clean:
	rm *.elf
	rm *.hex

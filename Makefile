all:
	avr-gcc -mmcu=atmega168 -std=c99 -Wall -Os -o fineoffset168.elf fineoffset168.c
	avr-objcopy -j .text -j .data -O ihex fineoffset168.elf fineoffset168.hex

atmega168:
	avr-gcc -mmcu=atmega168 -std=c99 -Wall -Os -o fineoffset168.elf fineoffset168.c
	avr-objcopy -j .text -j .data -O ihex fineoffset168.elf fineoffset168.hex

install328:
	avr-gcc -mmcu=atmega328p -std=c99 -Wall -Os -o fineoffset328.elf fineoffset328.c
	avr-objcopy -j .text -j .data -O ihex fineoffset328.elf fineoffset328.hex
	avrdude -c arduino -b 57600 -P /dev/ttyUSB0 -p atmega328p -vv -U flash:w:fineoffset328.hex

atmega328:
	avr-gcc -mmcu=atmega328 -std=c99 -Wall -Os -o fineoffset328.elf fineoffset328.c
	avr-objcopy -j .text -j .data -O ihex fineoffset328.elf fineoffset328.hex


install:
	avr-gcc -mmcu=atmega168 -std=c99 -Wall -Os -o fineoffset168.elf fineoffset168.c
	avr-objcopy -j .text -j .data -O ihex fineoffset168.elf fineoffset168.hex
	avrdude -p m168 -c usbasp -U flash:w:fineoffset168.hex

clean:
	rm *.elf
	rm *.hex

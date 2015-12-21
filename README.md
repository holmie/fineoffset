fineoffset
==========

ATMega168 as a wireless 433MHz temperature sensor.


How to use:
===========
Flash to atmega168, connect 433 MHz TX to output PC2 - receive with rtl_433 (github.com/merbanan/rtl_433) or compatible WH2 weather station

this is just a proof of concept, it can push valid packets from an ATMega168 to a receiver, you need to connect a temperature sensor

do readings and modify the variables accordingly.



Other crap
==========
bitwise.c was used for testing bit shifting without having to flash the atmega eeprom every time

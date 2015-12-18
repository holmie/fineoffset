# fineoffset


problem...

problem 1) koden er shit..  det går bra

problem 2)?? temperatur og device er 12 bit, lagres i uint16_t...

problem 3) må regne crc8 på payload (som er deviceid, temperatur, luftfuktighet) - hvordan kombinere de i en uint32_t (er det lurt?) og regne crc8 på det...?

(The CRC-8 polynomial used is x8 + x5 + x4 + 1.)

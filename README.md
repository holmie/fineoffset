# fineoffset

upload to atmega168, connect 433 MHz TX to output PC2 - receive with rtl_433 (github.com/merbanan/rtl_433)

problem: have to calculate crc8 of payload (which is deviceid, temperature, humidity) (32 bits/4 bytes) - now stored in uint32_t payload.. wise?

(The CRC-8 polynomial used is x8 + x5 + x4 + 1.)


bitwise.c was used for testing bit shifting without having to flash the atmega eeprom every time

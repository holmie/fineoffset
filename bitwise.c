#include <stdio.h>
#include <stdint.h>
#define CRC_POLY 0x31

/* Tried using this for testing */

uint8_t crc = 0;		// global, holds the crc

void send_uint8_bitstring(uint8_t intarr, int skip) {
	char bitstring[30] = "";

	for (int i = 7; i >= skip; i--) {
		if ((intarr & (1<<i)) >> i == 0) {
			printf("send_zero(); ");
		} else {
			printf("send_one(); ");
		}
		sprintf(bitstring, "%s%d", bitstring, (intarr & (1<<i)) >> i);
	}
	printf("\n%s\n", bitstring);
}


/* this is somewhat ripped from merbanan/rtl_433/src/util.c */
uint8_t crc8(uint32_t payload, unsigned nBytes, uint8_t polynomial, uint8_t init) {
    uint8_t remainder = init;	
    unsigned byte, bit;

	uint8_t current_byte;

    for (byte = 0; byte < nBytes; ++byte) {
		// 32-(8*(0+1))
		current_byte = payload >> (32-(8*(byte+1)));
        remainder ^= current_byte;
        for (bit = 0; bit < 8; ++bit) {
            if (remainder & 0x80) {
                remainder = (remainder << 1) ^ polynomial;
            }
            else {
                remainder = (remainder << 1);
            }
        }
    }
    return remainder;
}

uint8_t crc8_update(uint8_t b)
{
    uint8_t do_xor;
    uint8_t reg;

    reg = crc;
    do_xor = (reg & 0x80);

    reg <<=1;
    reg |= b;

    if (do_xor)
    {
        reg ^= CRC_POLY;
    }

    crc = reg;
}

void send_uint32_bitstring(uint16_t intarr, int skip) {
	char bitstring[40] = "";

	for (int i = (31-skip); i >= 0; i--) {
		if ((intarr & (1<<i)) >> i == 0) {
			printf("send_zero(); ");
		} else {
			printf("send_one(); ");
		}
		sprintf(bitstring, "%s%d", bitstring, (intarr & (1<<i)) >> i);
	}
	printf("\n%s\n", bitstring);
}


void send_uint16_bitstring(uint16_t intarr, int skip) {
	char bitstring[30] = "";

	for (int i = (15-skip); i >= 0; i--) {
		if ((intarr & (1<<i)) >> i == 0) {
			printf("send_zero(); ");
		} else {
			printf("send_one(); ");
		}
		sprintf(bitstring, "%s%d", bitstring, (intarr & (1<<i)) >> i);
	}
	printf("\n%s\n", bitstring);
}

int main () {
	// protocol:
	// 8 bits of preamble (255) (0b11111111)
	// 12 bits of device id (0b0100 0101 1100)
	// 12 bits of temperature (0b0000 1000 1111)
	// 8 bits of humidity (0b01000111) = 71
	// 8 bits of crc8 (0b1001 1000)				this is the valid CRC for this packet
	
	uint8_t preamble = 255;
	uint16_t deviceid = 1116;
	uint16_t temperature = 251;
	uint8_t humidity = 69;
	crc = 0;

	// payload =  		 0000 0000 0000 0000 0000 0000 0000 0000
	uint32_t payload = 0 | deviceid << 20 | temperature << 8 | humidity;
	// deviceid =		 0000 0000 0000 0000 0000 1011 0111 0110
	// deviceid << 20 =	 1011 0111 0110 0000 0000 0000 0000 0000
	// payload 		  =  1011 0111 0110 0000 0000 0000 0000 0000
	// temperature    =  0000 0000 0000 0000 0000 0000 1111 1011
    // payload        =  1011 0111 0110 0000 1111 1011 0000 0000
	// humidity       =  xxxx xxxx xxxx xxxx xxxx xxxx 0100 0101
	// payload        =  1011 0111 0110 0000 1111 1011 0100 0101
	// actual:			 1011 0111 0110 0000 1111 1011 0100 0101
	// confirmed working.

	crc = crc8(payload, 4, 0x31, 0);

	printf("This should be 10011000\n");
	printf("It is: \n");
	send_uint8_bitstring(crc, 0);
	printf("\n\n");

	printf("Payload: %u\n", payload);
	printf("Hello\n");
/*
	send_uint8_bitstring(preamble, 0);
	send_uint16_bitstring(deviceid, 4);
	send_uint16_bitstring(temperature, 4);
	send_uint8_bitstring(humidity, 0);
	*/
}

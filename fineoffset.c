#define F_CPU 8000000UL
#define BAUD 9600

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <util/setbaud.h>

void send_zero() {
	PORTC &= ~(1<<PC2);
	_delay_ms(1);
	PORTC |= (1<<PC2);
	_delay_ms(1.5);
	PORTC &= ~(1<<PC2);
}

void send_one() {
	PORTC &= ~(1<<PC2);
	_delay_ms(1);
	PORTC |= (1<<PC2);
	_delay_ms(0.5);
	PORTC &= ~(1<<PC2);
}

void send_uint8_bitstring(uint8_t intarr, int skip) {
    for (int i = 7; i >= skip; i--) {
        if ((intarr & (1<<i)) >> i == 0) {
			send_zero();
        } else {
			send_one();
        }
    }
}

void send_uint16_bitstring(uint16_t intarr, int skip) {
    for (int i = (15-skip); i >= 0; i--) {
        if ((intarr & (1<<i)) >> i == 0) {
			send_zero();
        } else {
			send_one();
        }
    }
}


void send_crc8 () {
	// 8 bits of CRC-8 over the payload, except the preamble
	// x8+x5+x4+1
	send_one(); send_zero(); send_zero(); send_one();
	send_one(); send_zero(); send_zero(); send_zero();
}

int main () {
//	initUART();
	DDRC |=1<<PC2;  /* PC2 will now be the output pin */

	while (1) {
//		writeString("Sending temperature\r\n");
		uint8_t preamble = 255;
		uint16_t deviceid = 1116;
		uint16_t temperature = 251;
		uint8_t humidity = 69;
//		uint8_t crc8;

/*		uint32_t payload = (uint32_t) deviceid << 32 | (uint32_t) temperature;
		payload = payload | humidity;
*/

		send_uint8_bitstring(preamble, 0);
		send_uint16_bitstring(deviceid, 4);
		send_uint16_bitstring(temperature, 4);
		send_uint8_bitstring(humidity, 0);
		send_crc8();			// 0b10011000

		_delay_ms(3000);


	}
}

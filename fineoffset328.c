/*
 *
 * 433MHz TX should be connected to PD7
 * 433MHz VCC should be connected to PB0
 *
 */

#define F_CPU 16000000UL
#define BAUD 9600

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>

void send_zero() {
	PORTD &= ~(1<<PD7);
	_delay_ms(1);
	PORTD |= (1<<PD7);
	_delay_ms(1.5);
	PORTD &= ~(1<<PD7);
}

void send_one() {
	PORTD &= ~(1<<PD7);
	_delay_ms(1);
	PORTD |= (1<<PD7);
	_delay_ms(0.5);
	PORTD &= ~(1<<PD7);
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

void sender_on () {
	DDRB |=1<<PB0;
	PORTB |=1<<PB0;
}

void sender_off () {
	DDRB &= ~(1<<PB0);
	PORTB &= ~(1<<PB0);
}

int main () {
	//DDRC |=1<<PC2;  /* PC2 will now be the output pin */
	DDRD |=1<<PD7;

	uint8_t preamble = 255;

	uint16_t deviceid;
	uint16_t temperature;
	uint8_t humidity;
	uint8_t crc = 0x0;

	sender_off();

	while (1) {
		deviceid = 1116;
		humidity = 69;
		temperature = 251;
		uint32_t payload = 0 | deviceid << 20 | temperature << 8 | humidity;
		crc = crc8(payload, 4, 0x31, 0);		// crc of payload, 4 bytes, polynomial 0x31 x8+x5+x1+1

		sender_on();
		for (int i = 0; i <= 3; i++) {			// send it three times
			send_uint8_bitstring(preamble, 0);
			send_uint16_bitstring(deviceid, 4);
			send_uint16_bitstring(temperature, 4);
			send_uint8_bitstring(humidity, 0);
			send_uint8_bitstring(crc, 0);
			_delay_ms(0.5);
		}
		sender_off();

		_delay_ms(5000);

	}
}

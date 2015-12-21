/*
 *
 * Program to read a TSL230R light sensor with S3 and S4 high (100x division)
 * connected to INT0.    433MHz TX should be connected to PC2
 * It sends an estimated light value with the fineoffset protocol to a matching receiver.
 *
 */

#define F_CPU 8000000UL
#define BAUD 9600

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>
#include <util/crc16.h>

int counter;

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

ISR(INT0_vect) {
	counter++;
}

int main () {
	DDRC |=1<<PC2;  /* PC2 will now be the output pin */

	uint8_t preamble = 255;
	uint16_t deviceid;
	uint16_t temperature;
	uint8_t humidity;
	uint8_t crc8 = 0x0;

	uint8_t runcount = 0;

	sei();

	while (1) {


		deviceid = 1116;
		// will use the temperature field to send the light level
		humidity = runcount;
		// never figured out how to calculate this, check disabled in the receiver for now..
		crc8 = 0b10011000;

		// reset global interrupt counter
		counter = 0;

		// enable interrupts
		DDRD = 0;		// all inputs on DDRD
		PORTD |= (1<<PD2);
		EIMSK |= _BV(INT0);  // Enable INT0
		EICRA |= _BV(ISC01); // Trigger on falling edge of INT0
		_delay_ms(500);
/*		EIMSK &= _BV(INT0);
		EICRA &= _BV(ISC01);
		PORTD &= ~(1<<PD2);
*/
		// set the temperature to the current counter value
		temperature = counter;

/*		uint32_t payload = deviceid << 20 | temperature << 8 | humidity; */
    // deviceid =        0000 0000 0000 0000 0000 1011 0111 0110
    // deviceid << 20 =  1011 0111 0110 0000 0000 0000 0000 0000
    // payload        =  1011 0111 0110 0000 0000 0000 0000 0000
    // temperature    =  0000 0000 0000 0000 0000 0000 1111 1011
    // payload        =  1011 0111 0110 0000 1111 1011 0000 0000
    // humidity       =  xxxx xxxx xxxx xxxx xxxx xxxx 0100 0101
    // payload        =  1011 0111 0110 0000 1111 1011 0100 0101
    // actual:           1011 0111 0110 0000 1111 1011 0100 0101
    // confirmed working.

		for (int i = 0; i <= 3; i++) {			// send it three times
			send_uint8_bitstring(preamble, 0);
			send_uint16_bitstring(deviceid, 4);
			send_uint16_bitstring(temperature, 4);
			send_uint8_bitstring(humidity, 0);
			send_uint8_bitstring(crc8, 0);
			_delay_ms(0.5);
		}

		runcount++;
		if (runcount > 98) { runcount = 0; }

		_delay_ms(10000);


	}
}

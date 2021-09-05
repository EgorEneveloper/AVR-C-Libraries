#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "DHT11.h"

#ifndef DHTXX_TIMEOUT
	#define DHTXX_TIMEOUT 200
#endif

#if DHTXX_TIMEOUT > 255
	#error "DHTXX_TIMEOUT cannot take values greater than 255!"
#endif

/*
- 20ms start signal
- 20 - 40us response timeout
- 80us presence acknowledgement
- 80us of pause

- 50us of sync impulse
- 28us / 70us data impulse
*/

static uint8_t DHT11_ReadByte( volatile uint8_t *port, volatile uint8_t *direction, volatile uint8_t *portin, uint8_t mask, uint8_t *dest ) {
	uint8_t data = 0;
	uint8_t timeoutcnt = 0;
	uint8_t i = 0;

	//Disable interrupts
	cli();

	//Turn pin into input
	*direction &= ~mask;
	*port &= ~mask;

	//Get 8 bits
	for ( i = 128; i; i >>= 1 ) {
		//Wait for the sync part to finish
		timeoutcnt = 0;
		while ( !( *portin & mask ) ) {
			if ( timeoutcnt++ > DHTXX_TIMEOUT ) { return DHTXX_ERROR_COMM; }
			_delay_us( 1 );
		}

		_delay_us( 30 );
		if ( *portin & mask ) data |= i;

		//Wait for the data part to finish
		timeoutcnt = 0;
		while ( *portin & mask ) {
			if ( timeoutcnt++ > DHTXX_TIMEOUT ) { return DHTXX_ERROR_COMM; }
			_delay_us( 1 );
		}
	}

	*dest = data;
	return DHTXX_ERROR_OK;
}

uint8_t DHT11_Read( volatile uint8_t *port, volatile uint8_t *direction, volatile uint8_t *portin, uint8_t mask, uint8_t *temperature, uint8_t *humidity ) {
	uint8_t data[5]; //Data received from sensor
	uint8_t cs = 0; //Checksum
	uint8_t ec = 0;
	uint8_t i;

	//Send start signal
	*direction &= ~mask;
	*port &= ~mask;
	*direction |= mask;

	_delay_ms( 18 );

	//Turn pin into input, disable interrupts and wait for acknowledgement
	cli();
	*direction &= ~mask;
	_delay_us(70);

	if ( *portin & mask ) { return DHTXX_ERROR_COMM; } //Communication check 1
	_delay_us( 80 );

	
	if ( !( *portin & mask ) ) { return DHTXX_ERROR_COMM; } //Communication check 2
	_delay_us( 40 );

	for ( i = 0; i < 5; i++ ) { ec = DHT11_ReadByte( port, direction, portin, mask, &data[i] ); if(ec) { return ec; } } //Read data from sensor

	//Checksum calculation
	for ( i = 0; i < 4; i++ ) { cs += data[i]; }
	if ( cs != data[4] ) { return DHTXX_ERROR_CHECKSUM; }
	
	*humidity = data[0];
	*temperature = data[2];

	return DHTXX_ERROR_OK;
}

uint8_t DHT11_Convert( volatile uint8_t *port, volatile uint8_t *direction, volatile uint8_t *portin, uint8_t mask ) {
	//Send start signal
	*direction &= ~mask;
	*port &= ~mask;
	*direction |= mask;

	_delay_ms( 18 );

	//Turn pin into input, disable interrupts and wait for acknowledgement
	cli();
	*direction &= ~mask;
	_delay_us(80);

	//Communication check 1
	if ( *portin & mask ) { return DHTXX_ERROR_COMM; }
	
	return DHTXX_ERROR_OK;
}

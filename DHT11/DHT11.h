#ifndef DHT11_H
	#define DHT11_H

	#include <inttypes.h>

	#define DHTXX_ERROR_OK 0
	#define DHTXX_ERROR_COMM 1
	#define DHTXX_ERROR_CHECKSUM 2
	#define DHTXX_ERROR_OTHER 3
	#define DHTXX_OK DHTXX_ERROR_OK

	#define DHTXX_MUL 10

	extern uint8_t DHT11_Read( volatile uint8_t *port, volatile uint8_t *direction, volatile uint8_t *portin, uint8_t mask, uint8_t *temperature, uint8_t *humidity );
	extern uint8_t DHT11_Convert( volatile uint8_t *port, volatile uint8_t *direction, volatile uint8_t *portin, uint8_t mask );

#endif

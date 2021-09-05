/*
 * NRF_24.c
 *
 * Created: 07.02.2021 12:01:40
 * Author : Egor
 */ 
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "SPI.h"
#include "NRF_24.h"
#include "MySerial.h"

static uint8_t text[5] = "ready";
static uint8_t package[RF24_PackageSize] = { 0, 7, 9, 8, 5, 7, 2, 6, 44, 26, 7, 9, 8, 5, 7, 2, 6, 44, 2, 6, 26, 21, 22, 23, 44, 25, 26, 27, 28, 29, 30, 31 };

int main(void) {
	spi_init();
	Serial_Begin();
	
	if (RF24_Begin(25)) {
		for (uint8_t i = 0; i < 5; ++i){ Serial_Write_char(text[i]); }
		Serial_Write_NewLine();
		_delay_ms(500);
	}
	
	//RF24_Setup_Transmitter();
	RF24_Setup_Receiver();
	RF24_ASSERT_CE;
	
    while (1) {
		
		if (RF24_Receive(package, RF24_PackageSize)) {
			for (uint8_t i = 0; i < RF24_PackageSize; ++i) { Serial_Write_AsCharArray(package[i]); }
			RF24_ASSERT_CE;
			_delay_ms(500);
		}
		
		//RF24_Send(package, RF24_PackageSize);
		_delay_ms(500);
    }
}


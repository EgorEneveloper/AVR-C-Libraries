/*
 *  SPI.c
 *  About: AtMega SPI library
 *  Created: 02.02.2021 16:51:36
 *  Author: Zotov Egor
 */ 

#ifndef F_CPU
	#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include "SPI.h"

extern void spi_init(void) {
	SPI_DDR |= (1<<SPI_MOSI)|(0<<SPI_MISO)|(1<<SPI_SCK)|(1<<SPI_SS);
	SPI_PORT |= (1<<SPI_MOSI)|(1<<SPI_MISO)|(1<<SPI_SCK)|(1<<SPI_SS);
	
	/*разрешение spi,старший бит вперед,мастер, режим 0*/
	SPCR = (1<<SPE)|(0<<DORD)|(1<<MSTR)|(0<<CPOL)|(0<<CPHA)|(1<<SPR1)|(0<<SPR0);
	SPSR = (0<<SPI2X);
}
extern uint8_t spi_SendRecvByte(uint8_t data) {
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}
extern void spi_WriteArrw(uint8_t * datain, uint8_t len) {
	for (uint8_t i = 0; i < len; ++i) { spi_SendRecvByte(datain[i]); }
}
extern void spi_ReadArrw(uint8_t * dataout, uint8_t len) {
	for (uint8_t i = 0; i < len; ++i) { dataout[i] = spi_SendRecvByte(0xFF); }
}
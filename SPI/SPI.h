/*
 *  SPI.h
 *  About: AtMega SPI library
 *  Created: 02.02.2021 16:51:36
 *  Author: Zotov Egor
 */ 

#ifndef SPI_H_
	#define SPI_H_
	
	#define SPI_PORT    PORTB
	#define SPI_DDR     DDRB
	#define SPI_SS		DDB2
	#define SPI_MOSI    DDB3
	#define SPI_MISO    DDB4
	#define SPI_SCK     DDB5
	
	#define SPI_SS_BEGIN SPI_PORT &= ~(1 << SPI_SS)
	#define SPI_SS_END SPI_PORT |= (1 << SPI_SS)
	
	extern void spi_init(void);
	extern uint8_t spi_SendRecvByte(uint8_t data);
	extern void spi_ReadArrw(uint8_t * dataout, uint8_t len);
	extern void spi_WriteArrw(uint8_t * datain, uint8_t len);
#endif /* SPI_H_ */
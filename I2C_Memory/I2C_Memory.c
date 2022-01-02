/*
 *	I2C_Memory.c
 *	This is a library for I2C Serial EEPROM chips
 *	Created: 26.12.2021 15:34:54
 *  Author: Zotov Egor
 */ 

#ifndef F_CPU
	#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "TWI.h"
#include "I2C_Memory.h"

I2CMEM I2CMEM_Addr_New(uint8_t BitA2, uint8_t BitA1, uint8_t BitA0, uint8_t AddressBytesNum)  {
	I2CMEM new_I2CMEM = { 0xA0 | (BitA2 << 3) | (BitA1 << 2) | (BitA0 << 1), AddressBytesNum };
	return new_I2CMEM;
}
void I2CMEM_WriteByte(I2CMEM memory, unsigned dataAddr, uint8_t data) {
	TWI_Start();
	TWI_WriteByte(memory.chipaddr);
	while(memory.lenaddr--) { TWI_WriteByte(dataAddr >> (8 * memory.lenaddr)); }
	TWI_WriteByte(data);
	TWI_Stop();
}
void I2CMEM_WriteArray(I2CMEM memory, unsigned dataAddr, uint8_t* data, uint8_t len) {
	TWI_Start();
	TWI_WriteByte(memory.chipaddr);
	while(memory.lenaddr--) { TWI_WriteByte(dataAddr >> (8 * memory.lenaddr)); }
	while(len--) { TWI_WriteByte(*(data++)); }
	TWI_Stop();
}
void I2CMEM_ReadArray(I2CMEM memory, unsigned startAddr, uint8_t* data, uint8_t len) {
	TWI_Start();
	TWI_WriteByte(memory.chipaddr);
	while(memory.lenaddr--) { TWI_WriteByte(startAddr >> (8 * memory.lenaddr)); }
	TWI_Start();
	TWI_WriteByte((memory.chipaddr) | 1);
	while(len--) { TWI_ReadByte(data++, len); }
	TWI_Stop();
}
uint8_t I2CMEM_ReadByte(I2CMEM memory, unsigned dataAddr) {
	uint8_t data;
	I2CMEM_ReadArray(memory, dataAddr, &data, 1);
	return data;
}
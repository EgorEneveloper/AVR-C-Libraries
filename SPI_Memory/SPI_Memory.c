/*
 * AA_EEPROM.c
 * This is a library for 25AA1024 SPI EEPROM chip
 * Created: 02.02.2021 20:23:47
 * Author: Zotov Egor
 */ 

#ifndef F_CPU
	#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "SPI.h"
#include "SPI_Memory.h"

static void EEPROM_SS_BEGIN(SPIMEM* memory) { *memory->PORT &= ~memory->mask_SS; }
static void EEPROM_SS_END(SPIMEM* memory) { *memory->PORT |= memory->mask_SS; }
static void SPIMEM_SetAddr(SPIMEM* memory, unsigned long addr) {
	uint8_t addr_cur = memory->addr_bytelen;
	while(addr_cur--) {
		SPDR = (addr >> (addr_cur << 3)) & 0xFF;
		while(!(SPSR & (1<<SPIF)));
	}
}

extern SPIMEM New_SPIMEM(unsigned long Type, volatile uint8_t* SPIMEM_DDR, volatile uint8_t* SPIMEM_PORT, uint8_t SS_mask) {
	SPIMEM new_SPIMEM = { Type, SPIMEM_DDR, SPIMEM_PORT, SS_mask, Type & 0xFF };
	
	*SPIMEM_DDR |= SS_mask;
	*SPIMEM_PORT |= SS_mask;
	
	return new_SPIMEM;
}
extern void SPIMEM_SendByte(SPIMEM* memory, uint8_t data) {
	EEPROM_SS_BEGIN(memory);
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
	EEPROM_SS_END(memory);
}
extern void SPIMEM_Write_StatusReg(SPIMEM* memory, uint8_t registr) {
	EEPROM_SS_BEGIN(memory);
	SPDR = SPIMEM_WSTATUS;
	while(!(SPSR & (1<<SPIF)));
	SPDR = registr;
	while(!(SPSR & (1<<SPIF)));
	EEPROM_SS_END(memory);
}
extern uint8_t SPIMEM_ReadStatusReg(SPIMEM* memory) {
	EEPROM_SS_BEGIN(memory);
	SPDR = SPIMEM_RSTATUS;
	while(!(SPSR & (1<<SPIF)));
	SPDR = 0xFF;
	while(!(SPSR & (1<<SPIF)));
	uint8_t data = SPDR;
	
	EEPROM_SS_END(memory);
	return data;
}
extern uint8_t SPIMEM_IsConnected(SPIMEM* memory) {
	return SPIMEM_ReadStatusReg(memory) == 0xFF ? 0 : 1;
}
extern uint8_t SPIMEM_ReadByte(SPIMEM* memory, unsigned long addr) {
	EEPROM_SS_BEGIN(memory);
	SPDR = SPIMEM_READREG;
	while(!(SPSR & (1<<SPIF)));
	
	SPIMEM_SetAddr(memory, addr);
	
	SPDR = 0xFF;
	while(!(SPSR & (1<<SPIF)));
	uint8_t data = SPDR;
	
	EEPROM_SS_END(memory);
	return data;
}
extern void SPIMEM_Read(SPIMEM* memory, unsigned long addr, uint8_t* dataout, unsigned long len) {
	EEPROM_SS_BEGIN(memory);
	SPDR = SPIMEM_READREG;
	while(!(SPSR & (1<<SPIF)));
	
	SPIMEM_SetAddr(memory, addr);
	
	for (unsigned long i = 0; i < len; ++i) {
		SPDR = 0xFF;
		while(!(SPSR & (1<<SPIF)));
		dataout[i] = SPDR;
	}
	EEPROM_SS_END(memory);
}
extern void SPIMEM_Write(SPIMEM* memory, unsigned long addr, uint8_t* dataout, unsigned long len) {
	if (memory->type & SPIMEM_MASK_EEPROM) { SPIMEM_SendByte(memory, SPIMEM_WRITEEN); }
	EEPROM_SS_BEGIN(memory);
	SPDR = SPIMEM_WRITEREG;
	while(!(SPSR & (1<<SPIF)));
	
	SPIMEM_SetAddr(memory, addr);
	
	for (unsigned long i = 0; i < len; ++i) {
		SPDR = dataout[i];
		while(!(SPSR & (1<<SPIF)));
	}
	EEPROM_SS_END(memory);
}
extern unsigned long SPIMEM_FindFirstEmpty32ByteBlock(SPIMEM* memory) {
	unsigned long maxaddr = ((memory->type & ~SPIMEM_MASK_EEPROM) >> 8);
	for (unsigned long addr = 0; addr < maxaddr; addr += 32) {
		EEPROM_SS_BEGIN(memory);
		SPDR = SPIMEM_READREG;
		while(!(SPSR & (1<<SPIF)));
		
		SPIMEM_SetAddr(memory, addr);
		
		for (uint8_t i = 0; i < 32; ++i) {
			SPDR = 0xFF;
			while(!(SPSR & (1<<SPIF)));
			if (SPDR != 0xFF) { break; }
			if (i == 31) { EEPROM_SS_END(memory); return addr; }
		}
		EEPROM_SS_END(memory);
	}
	return maxaddr;
}
extern void SPIMEM_Erase(SPIMEM* memory, uint8_t erase_mode, unsigned long addr) { //EEPROM Only
	
	if (!(memory->type & SPIMEM_MASK_EEPROM)) { return; }
	
	EEPROM_SS_BEGIN(memory);
	
	SPDR = erase_mode;
	while(!(SPSR & (1<<SPIF)));
	
	if (erase_mode == SPIMEM_PGERASE || erase_mode == SPIMEM_STERASE) { SPIMEM_SetAddr(memory, addr); }
		
	EEPROM_SS_END(memory);
}
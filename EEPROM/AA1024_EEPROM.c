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
#include "AA1024_EEPROM.h"
#include "SPI.h"

static void EEPROM_SS_BEGIN(EEPROM* chip) { *chip->PORT &= ~chip->mask_SS; }
static void EEPROM_SS_END(EEPROM* chip) { *chip->PORT |= chip->mask_SS; }

extern EEPROM New_EEPROM(volatile uint8_t* EEPROM_DDR, volatile uint8_t* EEPROM_PORT, uint8_t SS_mask) {
	EEPROM new_EEPROM = { EEPROM_DDR, EEPROM_PORT, SS_mask };
	
	*EEPROM_DDR |= SS_mask;
	*EEPROM_PORT |= SS_mask;
	
	return new_EEPROM;
}
void EEPROM_SetAddr(unsigned long addr) {
	SPDR = (addr >> 16) & 0xFF;
	while(!(SPSR & (1<<SPIF)));
	SPDR = (addr >> 8) & 0xFF;
	while(!(SPSR & (1<<SPIF)));
	SPDR = addr & 0xFF;
	while(!(SPSR & (1<<SPIF)));
}
extern void EEPROM_SendByte(EEPROM* chip, uint8_t data) {
	EEPROM_SS_BEGIN(chip);
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
	EEPROM_SS_END(chip);
}
extern void EEPROM_Write_StatusReg(EEPROM* chip, uint8_t registr) {
	EEPROM_SS_BEGIN(chip);
	SPDR = EEPROM_WSTATUS;
	while(!(SPSR & (1<<SPIF)));
	SPDR = registr;
	while(!(SPSR & (1<<SPIF)));
	EEPROM_SS_END(chip);
}
extern uint8_t EEPROM_ReadStatusReg(EEPROM* chip) {
	EEPROM_SS_BEGIN(chip);
	SPDR = EEPROM_RSTATUS;
	while(!(SPSR & (1<<SPIF)));
	SPDR = 0xFF;
	while(!(SPSR & (1<<SPIF)));
	uint8_t data = SPDR;
	
	EEPROM_SS_END(chip);
	return data;
}
extern uint8_t EEPROM_IsConnected(EEPROM* chip) {
	return EEPROM_ReadStatusReg(chip) == 0xFF ? 0 : 1;
}
extern uint8_t EEPROM_ReadByte(EEPROM* chip, unsigned long addr) {
	EEPROM_SS_BEGIN(chip);
	SPDR = EEPROM_READREG;
	while(!(SPSR & (1<<SPIF)));
	
	EEPROM_SetAddr(addr);
	
	SPDR = 0xFF;
	while(!(SPSR & (1<<SPIF)));
	uint8_t data = SPDR;
	
	EEPROM_SS_END(chip);
	return data;
}
extern void EEPROM_Read(EEPROM* chip, unsigned long addr, uint8_t* dataout, uint16_t len) {
	EEPROM_SS_BEGIN(chip);
	SPDR = EEPROM_READREG;
	while(!(SPSR & (1<<SPIF)));
	
	EEPROM_SetAddr(addr);
	
	for (uint16_t i = 0; i < len; ++i) {
		SPDR = 0xFF;
		while(!(SPSR & (1<<SPIF)));
		dataout[i] = SPDR;
	}
	EEPROM_SS_END(chip);
}
extern void EEPROM_Write(EEPROM* chip, unsigned long addr, uint8_t* dataout, uint16_t len) {
	if (EEPROM_IGNOREPROTECT) { EEPROM_SendByte(chip, EEPROM_WRITEEN); _delay_us(15); }
	EEPROM_SS_BEGIN(chip);
	SPDR = EEPROM_WRITEREG;
	while(!(SPSR & (1<<SPIF)));
	
	EEPROM_SetAddr(addr);
	
	for (uint16_t i = 0; i < len; ++i) {
		SPDR = dataout[i];
		while(!(SPSR & (1<<SPIF)));
	}
	EEPROM_SS_END(chip);
}
extern unsigned long EEPROM_FindFirstEmpty32ByteBlock(EEPROM* chip) {
	for (unsigned long addr = 0; addr < EEPROM_MAXADDR; addr += 32) {
		EEPROM_SS_BEGIN(chip);
		SPDR = EEPROM_READREG;
		
		while(!(SPSR & (1<<SPIF)));
		
		EEPROM_SetAddr(addr);
		
		for (uint8_t i = 0; i < 32; ++i) {
			SPDR = 0xFF;
			while(!(SPSR & (1<<SPIF)));
			if (SPDR != 0xFF) { break; }
			if (i == 31) { EEPROM_SS_END(chip); return addr; }
		}
		EEPROM_SS_END(chip);
	}
	return EEPROM_MAXADDR;
}
extern void EEPROM_Erase(EEPROM* chip, uint8_t erase_mode, unsigned long addr) {
	EEPROM_SS_BEGIN(chip);
	
	SPDR = erase_mode;
	while(!(SPSR & (1<<SPIF)));
	
	if (erase_mode == EEPROM_PGERASE || erase_mode == EEPROM_STERASE) { EEPROM_SetAddr(addr); }
		
	EEPROM_SS_END(chip);
}
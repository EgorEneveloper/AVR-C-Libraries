/*
 * AA_EEPROM.h
 * This is a library for 25AA1024 SPI EEPROM chip
 * Created: 02.02.2021 20:23:47
 * Author: Zotov Egor
 */ 

#ifndef AA_EEPROM_H_
	#define AA_EEPROM_H_
	
	typedef struct EEPROM {
		volatile uint8_t* DDR;
		volatile uint8_t* PORT;
		uint8_t mask_SS;
	} EEPROM;
	
	#define EEPROM_IGNOREPROTECT 1 //allow EEPROM_Write() even if protected, otherwise set 0
	#define EEPROM_WRITETIME 6 //time in ms to write data to EEPROM 
	#define EEPROM_PAGESIZE 256
	#define EEPROM_MAXADDR	0x1FFFF
	
	#define EEPROM_READREG	0b00000011 //Read data from memory array beginning at selected address
	#define EEPROM_WRITEREG	0b00000010 //Write data to memory array beginning at selected address
	#define EEPROM_WRITEDIS 0b00000100 //Reset the write enable latch (disable write operations)
	#define EEPROM_WRITEEN	0b00000110 //Set the write enable latch (enable write operations)
	#define EEPROM_RSTATUS	0b00000101 //Read STATUS register
	#define EEPROM_WSTATUS	0b00000001 //Write STATUS register
	#define EEPROM_PGERASE	0b01000010 //Erase one page in memory array
	#define EEPROM_STERASE	0b11011000 //Erase one sector in memory array
	#define EEPROM_CHERASE	0b11000111 //Erase all sectors in memory array
	
	extern EEPROM New_EEPROM(volatile uint8_t* EEPROM_DDR, volatile uint8_t* EEPROM_PORT, uint8_t SS_mask);
	extern void EEPROM_SendByte(EEPROM* chip, uint8_t data);
	extern void EEPROM_Write(EEPROM* chip, unsigned long addr, uint8_t* dataout, uint16_t len);
	extern uint8_t EEPROM_ReadByte(EEPROM* chip, unsigned long addr);
	extern void EEPROM_Erase(EEPROM* chip, uint8_t erase_mode, unsigned long addr);
	extern void EEPROM_Read(EEPROM* chip, unsigned long addr, uint8_t* dataout, uint16_t len);
	extern void EEPROM_Write_StatusReg(EEPROM* chip, uint8_t registr);
	extern uint8_t EEPROM_ReadStatusReg(EEPROM* chip);
	extern uint8_t EEPROM_IsConnected(EEPROM* chip);
	extern unsigned long EEPROM_FindFirstEmpty32ByteBlock(EEPROM* chip);
	
#endif /* AA_EEPROM_H_ */
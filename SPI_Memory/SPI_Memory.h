/*
 * AA_EEPROM.h
 * This is a library for 25AA1024 SPI EEPROM chip
 * Created: 02.02.2021 20:23:47
 * Author: Zotov Egor
 */ 

#ifndef SPIMEMORY_H_
	#define SPIMEMORY_H_
	
	typedef struct SPIMEM {
		unsigned long type;
		volatile uint8_t* DDR;
		volatile uint8_t* PORT;
		uint8_t mask_SS;
		uint8_t addr_bytelen;
	} SPIMEM;
	
	#define SPIMEM_MASK_EEPROM (1UL << 31)
	#define SPIMEM_TYPE_NEW(iseeprom, addrlenbytes, maxaddr)	((iseeprom ? SPIMEM_MASK_EEPROM : 0UL) | (maxaddr << 8) | addrlenbytes)
	
	#define SPIMEM_TYPE_EEPROM1024	SPIMEM_TYPE_NEW(1, 3, 0x1FFFF)
	#define SPIMEM_TYPE_EEPROM512	SPIMEM_TYPE_NEW(1, 2, 0xFFFF)
	#define SPIMEM_TYPE_EEPROM256	SPIMEM_TYPE_NEW(1, 2, 0x7FFF)
	#define SPIMEM_TYPE_EEPROM64	SPIMEM_TYPE_NEW(1, 2, 0x1FFF)
	#define SPIMEM_TYPE_EEPROM16	SPIMEM_TYPE_NEW(1, 2, 0x7FF)
	#define SPIMEM_TYPE_EEPROM8		SPIMEM_TYPE_NEW(1, 2, 0x3FF)
	#define SPIMEM_TYPE_SRAM1024	SPIMEM_TYPE_NEW(0, 3, 0x1FFFF)
	#define SPIMEM_TYPE_SRAM512		SPIMEM_TYPE_NEW(0, 2, 0xFFFF)
	#define SPIMEM_TYPE_SRAM256		SPIMEM_TYPE_NEW(0, 2, 0x7FFF)
	
	#define SPIMEM_EEPROM_WRITETIME 6	//time in ms to write data to EEPROM 
	
	#define SPIMEM_READREG	0b00000011 //Read data from memory array beginning at selected address
	#define SPIMEM_WRITEREG	0b00000010 //Write data to memory array beginning at selected address
	#define SPIMEM_WRITEDIS 0b00000100 //Reset the write enable latch (disable write operations)
	#define SPIMEM_WRITEEN	0b00000110 //Set the write enable latch (enable write operations)
	#define SPIMEM_RSTATUS	0b00000101 //Read STATUS register
	#define SPIMEM_WSTATUS	0b00000001 //Write STATUS register
	#define SPIMEM_PGERASE	0b01000010 //Erase one page in memory array
	#define SPIMEM_STERASE	0b11011000 //Erase one sector in memory array
	#define SPIMEM_CHERASE	0b11000111 //Erase all sectors in memory array
	
	extern SPIMEM New_SPIMEM(unsigned long Type, volatile uint8_t* SPIMEM_DDR, volatile uint8_t* SPIMEM_PORT, uint8_t SS_mask);
	extern void SPIMEM_SendByte(SPIMEM* memory, uint8_t data);
	extern void SPIMEM_Write(SPIMEM* memory, unsigned long addr, uint8_t* dataout, unsigned long len);
	extern uint8_t SPIMEM_ReadByte(SPIMEM* memory, unsigned long addr);
	extern void SPIMEM_Erase(SPIMEM* memory, uint8_t erase_mode, unsigned long addr);
	extern void SPIMEM_Read(SPIMEM* memory, unsigned long addr, uint8_t* dataout, unsigned long len);
	extern void SPIMEM_Write_StatusReg(SPIMEM* memory, uint8_t registr);
	extern uint8_t SPIMEM_ReadStatusReg(SPIMEM* memory);
	extern uint8_t SPIMEM_IsConnected(SPIMEM* memory);
	extern unsigned long SPIMEM_FindFirstEmpty32ByteBlock(SPIMEM* memory);
	
#endif /* SPIMEMORY_H_ */
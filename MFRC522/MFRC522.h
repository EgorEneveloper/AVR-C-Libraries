/*
 * MFRC522.h
 *
 * Created: 18.03.2021 15:22:15
 * Author: Zotov Egor
 */ 
#ifndef MFRC522_H_
	#define MFRC522_H_

	#define F_CPU 8000000UL
	#define MAX_LEN 16
	
	#define MFRC522_PORT PORTB
	#define MFRC522_DDR DDRB
	#define MFRC522_SS DDB0
	
	#define MFRC522_SS_BEGIN MFRC522_PORT &= ~(1 << MFRC522_SS)
	#define MFRC522_SS_END MFRC522_PORT |= (1 << MFRC522_SS)
	
	//Our communication codes that return from MF522
	#define MI_OK 0
	#define MI_NOTAGERR 1
	#define MI_ERR 2

	extern void MFRC522_Init(void);

	extern uint8_t MFRC522_Request(uint8_t reqMode, uint8_t *TagType);
	extern uint8_t MFRC522_Anticoll(uint8_t *serNum);

	extern uint8_t MFRC522_Read(uint8_t blockAddr, uint8_t *recvData);
	extern uint8_t MFRC522_Write(uint8_t blockAddr, uint8_t *writeData);

	extern void Write_MFRC522(uint8_t addr, uint8_t val);
	extern uint8_t Read_MFRC522(uint8_t addr);

	extern uint8_t MFRC522_ToCard(uint8_t command, uint8_t *sendData, uint8_t sendLen, uint8_t *backData, unsigned *backLen);
	extern void MFRC522_CalulateCRC(uint8_t *pIndata, uint8_t len, uint8_t *pOutData);

	extern void MFRC522_SetBitMask(uint8_t reg, uint8_t mask);
	extern void MFRC522_ClearBitMask(uint8_t reg, uint8_t mask);
	extern void MFRC522_AntennaOn(void);

	extern uint8_t MFRC522_ReadCard(uint8_t *CardID);
#endif

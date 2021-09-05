/*
 * MFRC522_DEFS.h
 *
 * Created: 18.03.2021 15:22:15
 * Author: Zotov Egor
 */ 
#include <avr/io.h>
#include <util/delay.h>
#include "SPI.h"
#include "MFRC522_DEFS.h"
#include "MFRC522.h"

extern void Write_MFRC522(uint8_t addr, uint8_t val) {
	MFRC522_SS_BEGIN;
	spi_SendRecvByte((addr << 1) & 0x7E);
	spi_SendRecvByte(val);
	MFRC522_SS_END;
}
extern uint8_t Read_MFRC522(uint8_t addr){
	uint8_t val;
	MFRC522_SS_BEGIN;
	spi_SendRecvByte(((addr << 1) & 0x7E) | 0x80);
	val = spi_SendRecvByte(0x00);
	MFRC522_SS_END;
	return val;
}
extern void MFRC522_Init(void) {
	MFRC522_DDR |= (1 << MFRC522_SS);
	MFRC522_PORT |= (1 << MFRC522_SS);
	MFRC522_SS_END;
	_delay_ms(1);
	
	Write_MFRC522(TModeReg, 0x8D);	//Tauto=1; f(Timer) = 6.78MHz/TPreScaler
	Write_MFRC522(TPrescalerReg, 0x3E);	//TModeReg[3..0] + TPrescalerReg
	Write_MFRC522(TReloadRegL, 30);
	Write_MFRC522(TReloadRegH, 0);
	Write_MFRC522(TxAutoReg, 0x40);	//100%ASK
	Write_MFRC522(ModeReg, 0x3D);	//CRC init
	
	MFRC522_AntennaOn();
}
extern unsigned char MFRC522_Request(uint8_t reqMode, uint8_t *TagType) {
	uint8_t status;
	unsigned backBits;
	Write_MFRC522(BitFramingReg, 0x07);
	TagType[0] = reqMode;
	status = MFRC522_ToCard(PCD_TRANSCEIVE, TagType, 1, TagType, &backBits);
	if ( (status != MI_OK) || (backBits != 0x10) ){ status = MI_ERR; }
	
	return status;
}
extern uint8_t MFRC522_ToCard(uint8_t command, uint8_t *sendData, uint8_t sendLen, uint8_t *backData, unsigned *backLen) {
	uint8_t status = MI_ERR, irqEn = 0x00, waitIRq = 0x00;
	uint8_t lastBits, n;
	unsigned i;
	switch (command){
		case PCD_AUTHENT:	//verify card password
			irqEn = 0x12;
			waitIRq = 0x10;
			break;
		case PCD_TRANSCEIVE:	//send data in the FIFO
			irqEn = 0x77;
			waitIRq = 0x30;
			break;
		default:
			break;
	}
	Write_MFRC522(CommIEnReg, irqEn | 0x80); //Allow interruption
	MFRC522_ClearBitMask(CommIrqReg, 0x80);	//Clear all the interrupt bits
	MFRC522_SetBitMask(FIFOLevelReg, 0x80);	//FlushBuffer=1, FIFO init
	Write_MFRC522(CommandReg, PCD_IDLE); //NO action;cancel current command ???

	for (i = 0; i < sendLen; ++i){ Write_MFRC522(FIFODataReg, sendData[i]); } //write data into FIFO
		
	Write_MFRC522(CommandReg, command);
	if (command == PCD_TRANSCEIVE){ MFRC522_SetBitMask(BitFramingReg, 0x80); }
	
	i = 2000;
	n = Read_MFRC522(CommIrqReg);
	while ((i != 0) && !(n & 0x01) && !(n & waitIRq)) { n = Read_MFRC522(CommIrqReg); --i; } //wait receive data is finished

	MFRC522_ClearBitMask(BitFramingReg, 0x80);
	if (i != 0) {
		if (!(Read_MFRC522(ErrorReg) & 0x1B) ) {
			status = MI_OK;
			if (n & irqEn & 0x01){ status = MI_NOTAGERR; }

			if (command == PCD_TRANSCEIVE){
				n = Read_MFRC522(FIFOLevelReg);
				lastBits = Read_MFRC522(ControlReg) & 0x07;
				if (lastBits){
					*backLen = (n - 1) * 8 + lastBits; }
					else{ *backLen = n * 8;
				}

				if (n == 0) { n = 1; }
				if (n > MAX_LEN) { n = MAX_LEN; }
				for (i = 0; i < n; ++i){ backData[i] = Read_MFRC522(FIFODataReg); }
			}
		}
		else{ status = MI_ERR; }
	}
	return status;
}
extern uint8_t MFRC522_Anticoll(uint8_t *serNum) {
	uint8_t serNumCheck = 0;
	uint8_t status, i;
	unsigned unLen;
	Write_MFRC522(BitFramingReg, 0x00);
	serNum[0] = PICC_ANTICOLL;
	serNum[1] = 0x20;
	status = MFRC522_ToCard(PCD_TRANSCEIVE, serNum, 2, serNum, &unLen);
	if (status == MI_OK) {
		for (i = 0; i < 4; ++i){ serNumCheck ^= serNum[i]; } //Verify card serial number
		if (serNumCheck != serNum[i]){ status = MI_ERR; }
	}
	return status;
}
extern uint8_t MFRC522_Read(uint8_t blockAddr, uint8_t *recvData) {
	uint8_t status;
	unsigned unLen;
	recvData[0] = PICC_READ;
	recvData[1] = blockAddr;
	MFRC522_CalulateCRC(recvData, 2, &recvData[2]);
	status = MFRC522_ToCard(PCD_TRANSCEIVE, recvData, 4, recvData, &unLen);
	if ( (status != MI_OK) || (unLen != 0x90) ) { status = MI_ERR; }
	return status;
}
extern void MFRC522_CalulateCRC(uint8_t *pIndata, uint8_t len, uint8_t *pOutData){
	uint8_t i, n;
	MFRC522_ClearBitMask(DivIrqReg, 0x04);	//CRCIrq = 0
	MFRC522_SetBitMask(FIFOLevelReg, 0x80);	//Clear FIFO pointer
	
	for (i = 0; i < len; ++i) { Write_MFRC522(FIFODataReg, *(pIndata + i)); }
	Write_MFRC522(CommandReg, PCD_CALCCRC);
	
	i = 0xFF;
	n = Read_MFRC522(DivIrqReg);
	while ((i != 0) && !(n & 0x04)) { n = Read_MFRC522(DivIrqReg); --i; } //wait CRC caculation to finish
	
	pOutData[0] = Read_MFRC522(CRCResultRegL);
	pOutData[1] = Read_MFRC522(CRCResultRegM);
}
extern uint8_t MFRC522_Write(uint8_t blockAddr, uint8_t *writeData) {
	uint8_t status, i;
	uint8_t buff[18];
	unsigned recvBits;
	
	buff[0] = PICC_WRITE;
	buff[1] = blockAddr;
	MFRC522_CalulateCRC(buff, 2, &buff[2]);
	status = MFRC522_ToCard(PCD_TRANSCEIVE, buff, 4, buff, &recvBits);
	if ( (status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A) ) { status = MI_ERR; }
	if (status == MI_OK){
		for (i = 0; i < 16; ++i) { buff[i] = *(writeData + i); }
		MFRC522_CalulateCRC(buff, 16, &buff[16]);
		status = MFRC522_ToCard(PCD_TRANSCEIVE, buff, 18, buff, &recvBits);
		if ( (status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A) ){ status = MI_ERR; }
	}
	return status;
}
extern void MFRC522_SetBitMask(uint8_t reg, uint8_t mask) {
	uint8_t tmp = Read_MFRC522(reg);
	Write_MFRC522(reg, tmp | mask); // set bit mask
}
extern void MFRC522_ClearBitMask(uint8_t reg, uint8_t mask) {
	uint8_t tmp = Read_MFRC522(reg);
	Write_MFRC522(reg, tmp & (~mask)); // clear bit mask
}
extern void MFRC522_AntennaOn(void) {
	uint8_t temp = Read_MFRC522(TxControlReg);
	if (!(temp & 0x03)){ MFRC522_SetBitMask(TxControlReg, 0x03); }
}
extern uint8_t MFRC522_ReadCard(uint8_t *CardID) {
	if ( MFRC522_Request( PICC_REQIDL, CardID ) == MI_OK ) {
		if ( MFRC522_Anticoll( CardID ) == MI_OK ) { return 1; }
	}
	return 0;
}
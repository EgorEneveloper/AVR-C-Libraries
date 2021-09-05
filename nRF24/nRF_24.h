/*
 * nRF_24.h
 *
 * Created: 07.02.2021 12:03:36
 *  Author: Zotov Egor
 */ 

#ifndef NRF_24_H_
	#define NRF_24_H_
	
	typedef struct NRF24L01 {
		volatile uint8_t* DDR;
		volatile uint8_t* PORT;
		volatile uint8_t* PIN;
		uint8_t mask_CE;
		uint8_t mask_CSN;
		uint8_t mask_IRQ;
	} NRF24L01;
	
	#define RF24_PackageSize	32
	
	#define RF24_Channel		7						//unused (example)
	#define Addr_Transmit		((uint8_t*)"TRSMT")		//unused (example)
	#define Addr_Receive		((uint8_t*)"RECIV")		//unused (example)
	
	extern NRF24L01 New_NRF24L01(volatile uint8_t* Radio_DDR, volatile uint8_t* Radio_PORT, volatile uint8_t* Radio_PIN, uint8_t CE_mask, uint8_t CSN_mask, uint8_t IRQ_mask);
	extern uint8_t RF24_ReadBuf(NRF24L01* Radio, uint8_t cmd, uint8_t* buf, uint8_t count);
	extern uint8_t RF24_WriteBuf(NRF24L01* Radio, uint8_t cmd, uint8_t* buf, uint8_t count);
	extern uint8_t RF24_ReadReg(NRF24L01* Radio, uint8_t reg);
	extern uint8_t RF24_WriteReg(NRF24L01* Radio, uint8_t reg, uint8_t val);
	extern uint8_t RF24_ReadRegBuf(NRF24L01* Radio, uint8_t reg, uint8_t* buf, uint8_t count);
	extern uint8_t RF24_WriteRegBuf(NRF24L01* Radio, uint8_t reg, uint8_t* buf, uint8_t count);
	extern uint8_t RF24_GetPayloadLen(NRF24L01* Radio);
	extern uint8_t RF24_Cmd(NRF24L01* Radio, uint8_t cmd);
	extern uint8_t RF24_IsInterrupt(NRF24L01* Radio);
	
	extern uint8_t RF24_Begin(NRF24L01* Radio, uint8_t attempts);
	extern void RF24_Setup_Transmitter(NRF24L01* Radio, uint8_t* Address, uint8_t channel);
	extern void RF24_Setup_Receiver(NRF24L01* Radio, uint8_t* Address, uint8_t channel);
	extern void RF24_Send(NRF24L01* Radio, uint8_t* data, uint8_t size);
	extern uint8_t RF24_Receive(NRF24L01* Radio, uint8_t* buf, uint8_t size);
	
#endif /* NRF_24_H_ */
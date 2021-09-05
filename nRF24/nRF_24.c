/*
 * nRF_24.c
 *
 * Created: 07.02.2021 12:03:36
 *  Author: Zotov Egor
 */ 

#ifndef F_CPU
	#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "SPI.h"
#include "nRFdef.h"
#include "nRF_24.h"

static void RF24_ASSERT_CE(NRF24L01* Radio) { *Radio->PORT |= Radio->mask_CE; }
static void RF24_DEASSERT_CE(NRF24L01* Radio) { *Radio->PORT &= ~Radio->mask_CE; }

static void RF24_ASSERT_CSN(NRF24L01* Radio) { *Radio->PORT &= ~Radio->mask_CSN; }
static void RF24_DEASSERT_CSN(NRF24L01* Radio) { *Radio->PORT |= Radio->mask_CSN; }

extern NRF24L01 New_NRF24L01(volatile uint8_t* Radio_DDR, volatile uint8_t* Radio_PORT, volatile uint8_t* Radio_PIN, uint8_t CE_mask, uint8_t CSN_mask, uint8_t IRQ_mask) {
	NRF24L01 new_NRF24L01 = { Radio_DDR, Radio_PORT, Radio_PIN, CE_mask, CSN_mask, IRQ_mask };
	
	*Radio_DDR |= CE_mask | CSN_mask;
	*Radio_DDR &= ~IRQ_mask;
	
	RF24_DEASSERT_CSN(&new_NRF24L01);
	RF24_DEASSERT_CE(&new_NRF24L01);
	
	return new_NRF24L01;
}
extern uint8_t RF24_ReadBuf(NRF24L01* Radio, uint8_t cmd, uint8_t* buf, uint8_t count) {
	RF24_ASSERT_CSN(Radio);
	uint8_t status = spi_SendRecvByte(cmd);
	while (count--) { *(buf++) = spi_SendRecvByte(0xFF); }
	RF24_DEASSERT_CSN(Radio);
	return status;
}
extern uint8_t RF24_WriteBuf(NRF24L01* Radio, uint8_t cmd, uint8_t* buf, uint8_t count) {
	RF24_ASSERT_CSN(Radio);
	uint8_t status = spi_SendRecvByte(cmd);
	while (count--) { spi_SendRecvByte(*(buf++)); }
	RF24_DEASSERT_CSN(Radio);
	return status;
}
extern uint8_t RF24_ReadReg(NRF24L01* Radio, uint8_t reg) {
	RF24_ASSERT_CSN(Radio);
	spi_SendRecvByte((reg & 31) | R_REGISTER);
	uint8_t ans = spi_SendRecvByte(0xFF);
	RF24_DEASSERT_CSN(Radio);
	return ans;
}
extern uint8_t RF24_WriteReg(NRF24L01* Radio, uint8_t reg, uint8_t val) {
	RF24_ASSERT_CSN(Radio);
	uint8_t status = spi_SendRecvByte((reg & 31) | W_REGISTER);
	spi_SendRecvByte(val);
	RF24_DEASSERT_CSN(Radio);
	return status;
}
extern uint8_t RF24_ReadRegBuf(NRF24L01* Radio, uint8_t reg, uint8_t* buf, uint8_t count) {
	return RF24_ReadBuf(Radio, (reg & 31) | R_REGISTER, buf, count);
}
extern uint8_t RF24_WriteRegBuf(NRF24L01* Radio, uint8_t reg, uint8_t* buf, uint8_t count) {
	return RF24_WriteBuf(Radio, (reg & 31) | W_REGISTER, buf, count);
}
extern uint8_t RF24_GetPayloadLen(NRF24L01* Radio) {
	RF24_ASSERT_CSN(Radio);
	spi_SendRecvByte(R_RX_PL_WID);
	uint8_t ans = spi_SendRecvByte(0xFF);
	RF24_DEASSERT_CSN(Radio);
	return ans;
}
extern uint8_t RF24_Cmd(NRF24L01* Radio, uint8_t cmd) {
	RF24_ASSERT_CSN(Radio);
	uint8_t status = spi_SendRecvByte(cmd);
	RF24_DEASSERT_CSN(Radio);
	return status;
}
extern uint8_t RF24_IsInterrupt(NRF24L01* Radio) {
	return *Radio->PIN & Radio->mask_IRQ ? 0 : 1;
	//return !(RF24_ReadReg(FIFO_STATUS) & (1 << RX_EMPTY));
}
static uint8_t RF24_Start(NRF24L01* Radio) {
	RF24_DEASSERT_CE(Radio);
	for(uint8_t cnt = 100;;) {
		RF24_WriteReg(Radio, CONFIG, (1 << EN_CRC) | (1 << CRCO) | (0 << PRIM_RX)); // Выключение питания
		if (RF24_ReadReg(Radio, CONFIG) == ((1 << EN_CRC) | (1 << CRCO) | (0 << PRIM_RX))) { break; } //Проверка записанного регистра
		if (!cnt--) { return 0; } // Если после 100 попыток не удалось записать регистр, то выходим с ошибкой
		_delay_ms(1);
	}
	
	RF24_WriteReg(Radio, EN_AA, 0);
	RF24_WriteReg(Radio, EN_RXADDR, 0);
	RF24_WriteReg(Radio, SETUP_AW, SETUP_AW_5BYTES_ADDRESS);
	RF24_WriteReg(Radio, RF_SETUP, RF_SETUP_250KBPS | RF_SETUP_0DBM);
	
	RF24_WriteReg(Radio, CONFIG, (1 << EN_CRC) | (1 << CRCO) | (1 << PWR_UP) | (0 << PRIM_RX));
	return (RF24_ReadReg(Radio, CONFIG) == ((1 << EN_CRC) | (1 << CRCO) | (1 << PWR_UP) | (0 << PRIM_RX))) ? 1 : 0;
}
extern uint8_t RF24_Begin(NRF24L01* Radio, uint8_t attempts) {
	while(attempts--) {
		if (RF24_Start(Radio)) { _delay_ms(2); RF24_ASSERT_CE(Radio); return 1; }
		_delay_ms(500);
	}
	return 0;
}
extern void RF24_Setup_Transmitter(NRF24L01* Radio, uint8_t* Address, uint8_t channel) {
	RF24_WriteReg(Radio, RF_CH, channel);
	RF24_WriteRegBuf(Radio, TX_ADDR, Address, 5);
}
extern void RF24_Setup_Receiver(NRF24L01* Radio, uint8_t* Address, uint8_t channel) {
	//RF24_WriteReg(EN_AA, (1 << ENAA_P0));
	RF24_WriteReg(Radio, RF_CH, channel);
	RF24_WriteReg(Radio, EN_RXADDR, (1 << ERX_P0));
	RF24_WriteRegBuf(Radio, RX_ADDR_P0, Address, 5); //&Addr_Receive[0]
	RF24_WriteReg(Radio, RX_PW_P0, RF24_PackageSize);
	
	RF24_WriteReg(Radio, CONFIG, (1 << EN_CRC) | (1 << CRCO) | (1 << PWR_UP) | (1 << PRIM_RX));
	RF24_WriteReg(Radio, STATUS, (1 << RX_DR) | (1 << TX_DS) | (1 << MAX_RT));
}
extern void RF24_Send(NRF24L01* Radio, uint8_t* data, uint8_t size) {
	RF24_DEASSERT_CE(Radio);
	RF24_WriteReg(Radio, CONFIG, RF24_ReadReg(Radio, CONFIG) & ~((1 << PWR_UP) | (1 << PRIM_RX)));
	RF24_Cmd(Radio, FLUSH_TX);
	
	_delay_ms(10);
	RF24_WriteBuf(Radio, W_TX_PAYLOAD, data, size);
	RF24_WriteBuf(Radio, W_TX_PAYLOAD, data, size);
	
	_delay_ms(10);
	RF24_WriteReg(Radio, CONFIG, RF24_ReadReg(Radio, CONFIG) | (1 << PWR_UP));
	RF24_ASSERT_CE(Radio);
	
	_delay_ms(10);
	RF24_DEASSERT_CE(Radio);
	RF24_WriteReg(Radio, CONFIG, RF24_ReadReg(Radio, CONFIG) & ~(1 << PWR_UP));
}
extern uint8_t RF24_Receive(NRF24L01* Radio, uint8_t* buf, uint8_t size) {
	if (!RF24_IsInterrupt(Radio)) { return 0; }
	
	RF24_DEASSERT_CE(Radio);
	uint8_t state = RF24_Cmd(Radio, NOP);
	RF24_WriteReg(Radio, STATUS, state);
	_delay_ms(10);
	
	uint8_t maxpackets = 4;
	if (((state & (7 << RX_P_NO)) != (7 << RX_P_NO)) && maxpackets--) {
		//if (RF24_GetPayloadLen() > 32) { RF24_Cmd(FLUSH_RX); state = RF24_Cmd(NOP); continue; }
		
		RF24_ReadBuf(Radio, R_RX_PAYLOAD, buf, size);
		RF24_WriteReg(Radio, STATUS, (1 << RX_DR) | (1 << TX_DS) | (1 << MAX_RT));
		RF24_WriteReg(Radio, CONFIG, RF24_ReadReg(Radio, CONFIG) | (1 << PWR_UP) | (1 << PRIM_RX));
		return 1;
	}
	
	RF24_ASSERT_CE(Radio);
	_delay_ms(10);
	return 0;
}
#ifndef F_CPU
	#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Prog_USART.h"

extern void Prog_USART_Begin() {
	USART_DDR |= (1 << USART_TXPIN);
	USART_DDR &= ~(1 << USART_RXPIN);
	SetTxPin_1;
	SetRxPin_0;
}
extern void Prog_USART_Tx(uint8_t data) {
	cli();
	uint8_t i;
	SetTxPin_0;
	_delay_us(BAUD_DELAY);
	for(i = 0; i < DATA_BITS; ++i) {
		if((data >> i) & 0x01) { SetTxPin_1; }
		else { SetTxPin_0; }
		_delay_us(BAUD_DELAY);
	}
	for(i = 0; i < STOP_BITS; ++i) {
		SetTxPin_1;
		_delay_us(BAUD_DELAY);
	}
	sei();
}
extern uint8_t Prog_USART_Rx(uint8_t *data) {
	cli();
	uint8_t i; *data = 0;
	while((1 << USART_RXPIN) & USART_PIN);
	_delay_us(BAUD_DELAY);
	for(i = 0; i < DATA_BITS; ++i) {
		if((1 << USART_RXPIN) & USART_PIN) { *data |= (1 << i); }
		_delay_us(BAUD_DELAY);
	}
	for(i = 0; i < STOP_BITS; ++i){
		if(!((1 << USART_RXPIN) & USART_PIN)) { sei(); return 1; }
		_delay_us(BAUD_DELAY);
	}
	sei();
	return 0;
}
static void Prog_USART_Num_Out(unsigned long num) {
	if (num) { Prog_USART_Num_Out(num / 10); Prog_USART_Tx((num % 10) | '0'); }
}
extern void Prog_USART_TxNum(unsigned long num) {
	if (num) { Prog_USART_Num_Out(num); return; }
	Prog_USART_Tx('0');
}
extern void Prog_USART_Tx_NewLine() { Prog_USART_Tx(13); Prog_USART_Tx(10); }
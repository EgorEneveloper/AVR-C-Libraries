#ifndef PROG_USART_H_
	#define PROG_USART_H_
	
	#define USART_DDR DDRB
	#define USART_PORT PORTB
	#define USART_PIN PINB
	#define USART_TXPIN PB4
	#define USART_RXPIN PB3
	
	#define DATA_BITS 8
	#define STOP_BITS 1
	#define BAUD_DELAY 104 // 1000ms div by baud rate, then written in us; ex 104 for 9600 baud
	
	#define SetTxPin_1 USART_PORT |= (1 << USART_TXPIN)
	#define SetTxPin_0 USART_PORT &= ~(1 << USART_TXPIN)
	#define SetRxPin_1 USART_PORT |= (1 << USART_RXPIN)
	#define SetRxPin_0 USART_PORT &= ~(1 << USART_RXPIN)
	
	extern void Prog_USART_Begin();
	extern void Prog_USART_Tx(uint8_t data);
	extern uint8_t Prog_USART_Rx(uint8_t *data);
	extern void Prog_USART_TxNum(unsigned long num);
	extern void Prog_USART_Tx_NewLine();
#endif /* PROG_USART_H_ */
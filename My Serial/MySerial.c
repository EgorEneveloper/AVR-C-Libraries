#define F_CPU 8000000UL

#include <avr/io.h>
#include <stdint.h>

#include "MySerial.h"

void Serial_Begin(void) {
	UBRR0H = 0;
	UBRR0L = SERIAL_9600_BOD_8MHZ;	/* 9600 BOD 8MHz */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);	/* Enable receiver and transmitter */
	UCSR0C = (3<<UCSZ00);			/* Set frame format: 8data, 1stop bit */
}
uint8_t Serial_Available(void) {
	if ( UCSR0A & (1<<RXC0) ) { return 1; }
	return 0;
}
uint8_t Serial_Read(void) {
	while ( !(UCSR0A & (1<<RXC0)) ) /* Wait for data to be received */
	;
	return UDR0;					/* Get and return received data from buffer */
}
unsigned long Serial_Read_Num(char separator, uint8_t maxnums) {
	unsigned long data = 0;
	
	while (maxnums) {
		uint8_t nchar = Serial_Read();
		if (nchar == separator) { return data; }
		
		data *= 10;
		data += nchar & 0x0F;
		--maxnums;
	}
	return data;
}
void Serial_Write_char(uint8_t data) {
	while ( !( UCSR0A & (1<<UDRE0)) ) /* Wait for empty transmit buffer */
	;
	UDR0 = data; /* Put data into buffer, sends the data */
}
void Num_Out(unsigned long num) {
	if (num) { Num_Out(num / 10); Serial_Write_char((num % 10) + '0'); }
}
void Serial_Write_Num(unsigned long num) {
	if (num) { Num_Out(num); return; }
	Serial_Write_char('0');
}
void Serial_Write_NewLine(void) { Serial_Write_char(13); Serial_Write_char(10); }
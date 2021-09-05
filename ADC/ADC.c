/*
 * ADC.c
 *
 * Created: 04.02.2021 17:14:24
 *  Author: Zotov Egor
 */ 

#include <avr/io.h>
#include "ADC.h"

extern void ADC_init(void) {
	ADMUX |= (1<<REFS0);
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADEN);
}

extern void ADC_BeginConversion() {
	ADCSRA |= (1 << ADSC);
	while(!(ADCSRA & (1 << ADIF)));
}

extern uint16_t ADC_Read(uint8_t ADC_pin) {
	ADMUX = (ADMUX & 0xF0) | (ADC_pin & 0x0F );
	ADC_BeginConversion();
	return ADC;
}
/*
 * HC_SR04.c
 *
 * Created: 28.06.2021 0:07:23
 *  Author: Egor
 */ 

#ifndef F_CPU
	#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "HC_SR04.h"

extern HCSR04 New_HCSR04(volatile uint8_t *SR04_DDR, volatile uint8_t *SR04_Port, volatile uint8_t *SR04_Pin, uint8_t mask_trigger, uint8_t mask_echo) {
	HCSR04 NewSR;
	NewSR.DDR = SR04_DDR; NewSR.PORT = SR04_Port; NewSR.PIN = SR04_Pin;
	NewSR.Mask_Trigger = mask_trigger; NewSR.Mask_Echo = mask_echo;
	*NewSR.DDR |= mask_trigger;
	*NewSR.DDR &= ~(mask_echo);
	return NewSR;
}
extern uint16_t HCSR04_Measure(HCSR04 *SR04, uint16_t units) {
	uint16_t counter = 0;
	*SR04->PORT |= SR04->Mask_Trigger;
	_delay_us(HC_Trigger_Delay_us);
	*SR04->PORT &= ~(SR04->Mask_Trigger);
	while (!(*SR04->PIN & SR04->Mask_Echo)) { _delay_us(1); };
	while (*SR04->PIN & SR04->Mask_Echo) { ++counter; }
	
	counter /= units;
	
	return (counter - (counter >> 3));
}
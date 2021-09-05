/*
 * SG90.c
 *	About: A servo library, which doesn`t require PWM, works on any digital pin
 *	Created: 27.06.2021 0:26:31
 *  Author: Egor
 */ 

#ifndef F_CPU
	#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "SG90.h"

extern void SG90_Delay_us(unsigned long us) {
	while(--us) { _delay_us(1); }
}
extern Servo New_Servo(volatile uint8_t *SG_DDR, volatile uint8_t *SG_Port, uint8_t mask) {
	Servo NewServo = { SG_DDR, SG_Port, mask };
	*NewServo.DDR |= mask;
	return NewServo;
}
extern void Servo_Turn(Servo *servo, unsigned long degrees) {
	degrees = SG90_POS_DELAY * degrees / 180;
	*servo->PORT |= servo->MASK;
	SG90_Delay_us(SG90_MIN_DELAY + degrees);
	*servo->PORT &= ~servo->MASK;
	SG90_Delay_us(SG90_PERIOD - degrees);
}
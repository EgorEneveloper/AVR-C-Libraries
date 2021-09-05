/*
 * SG90.h
 *	About: A servo library, which doesn`t require PWM, works on any digital pin
 *	Created: 27.06.2021 0:26:15
 *  Author: Egor
 */ 

#ifndef SG90_H_
	#define SG90_H_
	
	#define SG90_DDR		DDRD
	#define SG90_PORT		PORTD
	#define SG90_PIN		DDD6
	
	#define SG90_MIN_DELAY	570
	#define SG90_POS_DELAY	572 // Result of 1142us - 570us, possible max delay left
	#define SG90_PERIOD		20000UL
	
	typedef struct Servo {
		volatile uint8_t *DDR;
		volatile uint8_t *PORT;
		uint8_t MASK;
	} Servo;
	
	extern void SG90_Delay_us(unsigned long us);
	extern Servo New_Servo(volatile uint8_t *SG_DDR, volatile uint8_t *SG_Port, uint8_t mask);
	extern void Servo_Turn(Servo *servo, unsigned long degrees);
	
#endif /* SG90_H_ */
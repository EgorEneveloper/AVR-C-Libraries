/*
 * HC_SR04.h
 *
 * Created: 27.06.2021 23:58:01
 *  Author: Egor
 */ 

#ifndef HC_SR04_H_
	#define HC_SR04_H_
	
	#define HC_Trigger_Delay_us		10
	
	#define HC_Measure_cm			58
	#define HC_Measure_inch			148
	
	typedef struct HCSR04 {
		volatile uint8_t *DDR;
		volatile uint8_t *PORT;
		volatile uint8_t *PIN;
		uint8_t Mask_Trigger;
		uint8_t Mask_Echo;
	} HCSR04;
	
	extern HCSR04 New_HCSR04(volatile uint8_t *SR04_DDR, volatile uint8_t *SR04_Port, volatile uint8_t *SR04_Pin, uint8_t mask_trigger, uint8_t mask_echo);
	extern uint16_t HCSR04_Measure(HCSR04 *SR04, uint16_t units);
	
#endif /* HC_SR04_H_ */
/*
 * ADC.c
 *
 * Created: 04.02.2021 17:14:24
 *  Author: Zotov Egor
 */ 

#ifndef ADC_H_
	#define ADC_H_
	
	#define ADC_0 0
	#define ADC_1 1
	#define ADC_2 2
	#define ADC_3 3
	#define ADC_4 4
	#define ADC_5 5
	#define ADC_6 6
	#define ADC_7 7
	#define ADC_8 8
	
	extern void ADC_init(void);
	extern void ADC_BeginConversion();
	extern uint16_t ADC_Read(uint8_t ADC_pin);
	
#endif /* ADC_H_ */
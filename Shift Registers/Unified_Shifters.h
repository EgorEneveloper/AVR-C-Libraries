/*
 * Unified_Shifters.h
 *
 * Created: 25.04.2021 0:20:10
 * Author: Egor
 * Info: Provides easy-interact with 74HC595 and 74HC165 shift registers
 */ 

#ifndef UNIFIED_SHIFTERS_H_
	#define UNIFIED_SHIFTERS_H_
	#define Register_Delay_time_us 1
	
	typedef struct ShiftRegister {
		volatile uint8_t* Reg_DDR;
		volatile uint8_t* Reg_Port;
		volatile uint8_t* Reg_Pin;
		
		uint8_t mask_Update;
		uint8_t mask_Clock;
		uint8_t mask_Data;
	} ShiftRegister;
	
	extern ShiftRegister NewRegister_595(volatile uint8_t* Reg_DDR, volatile uint8_t* Reg_Port, volatile uint8_t* Reg_Pin, uint8_t mask_update, uint8_t mask_clock, uint8_t mask_data);
	extern ShiftRegister NewRegister_165(volatile uint8_t* Reg_DDR, volatile uint8_t* Reg_Port, volatile uint8_t* Reg_Pin, uint8_t mask_setState, uint8_t mask_clock, uint8_t mask_data);
	
	extern void Register_Clock(ShiftRegister* Register);
	extern void Register595_Push_bit(ShiftRegister* Register, uint8_t bit);
	extern void Register595_Write(ShiftRegister* Register, uint8_t byte);
	extern uint8_t Register165_Read(ShiftRegister* Register);
	
#endif /* UNIFIED_SHIFTERS_H_ */
/*
 * Unified_Shifters.h
 *
 * Created: 25.04.2021 0:20:10
 * Author: Egor
 * Info: Provides easy-interact with 74HC595 and 74HC165 shift registers
 */ 

#ifndef F_CPU
	#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "Unified_Shifters.h"

extern ShiftRegister NewRegister_595(volatile uint8_t* Reg_DDR, volatile uint8_t* Reg_Port, volatile uint8_t* Reg_Pin, uint8_t mask_update, uint8_t mask_clock, uint8_t mask_data) {
	ShiftRegister Nreg = { Reg_DDR, Reg_Port, Reg_Pin, mask_update, mask_clock, mask_data };
	uint8_t setup_pins = mask_update | mask_clock | mask_data;
	
	*Reg_DDR |= setup_pins;
	*Reg_Port &= ~setup_pins;
	
	return Nreg;
}
extern ShiftRegister NewRegister_165(volatile uint8_t* Reg_DDR, volatile uint8_t* Reg_Port, volatile uint8_t* Reg_Pin, uint8_t mask_setState, uint8_t mask_clock, uint8_t mask_data) {
	ShiftRegister Nreg = { Reg_DDR, Reg_Port, Reg_Pin, mask_setState, mask_clock, mask_data };
	uint8_t setup_pins = mask_setState | mask_clock | mask_data;
	
	*Reg_DDR |= setup_pins; 
	*Reg_DDR &= ~mask_data;
	*Reg_Port &= ~setup_pins;
	
	return Nreg;
}
extern void Register_Clock(ShiftRegister* Register) {
	*Register->Reg_Port |= Register->mask_Clock;
	_delay_us(Register_Delay_time_us);
	*Register->Reg_Port &= ~Register->mask_Clock;
	_delay_us(Register_Delay_time_us);
}
extern void Register595_Push_bit(ShiftRegister* Register, uint8_t bit) {
	*Register->Reg_Port &= ~Register->mask_Update;
	
	if (bit) { *Register->Reg_Port |= Register->mask_Data; }
	else { *Register->Reg_Port &= ~Register->mask_Data; }
	Register_Clock(Register);
	
	*Register->Reg_Port |= Register->mask_Update;
}
extern void Register595_Write(ShiftRegister* Register, uint8_t byte) {
	*Register->Reg_Port &= ~Register->mask_Update;
	
	for (uint8_t i = 0; i < 8; ++i) {
		if (byte & 1) { *Register->Reg_Port |= Register->mask_Data; }
		else { *Register->Reg_Port &= ~(Register->mask_Data); }
		Register_Clock(Register);
		
		byte >>= 1;
	}
	*Register->Reg_Port |= Register->mask_Update;
}
extern uint8_t Register165_Read(ShiftRegister* Register) {
	*Register->Reg_Port |= Register->mask_Update;
	uint8_t val = 0;
	
	for (uint8_t i = 0; i < 8; ++i) {
		val <<= 1;
		if (*Register->Reg_Pin & Register->mask_Data) { val |= 1; }
		Register_Clock(Register);
	}
	
	*Register->Reg_Port &= ~Register->mask_Update;
	return val;
}
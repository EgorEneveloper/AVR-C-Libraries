/*
 *	HMC5883L.c
 *	About : I2C Compass library
 *	Created: 29.07.2021 17:34:09
 *	Author: Egor
 */ 

#include <inttypes.h>
#include "MyTWI.h"
#include "HMC5883L.h"

static int16_t HMC5883L_Measure(uint8_t MSB_Register_Axis) {
	uint8_t buffer[2];
	uint8_t counter = 2;
	
	TWI_Start();
	TWI_WriteByte(HMC5883L_ADDR_WRITE);
	TWI_WriteByte(MSB_Register_Axis);
	TWI_WriteByte(HMC5883L_ADDR_READ);
	while(counter--) { TWI_ReadByte(&buffer[counter], counter); }
	TWI_Stop();
	
	return (((int16_t)buffer[0] << 8) | (int16_t)buffer[1]);
}
extern void HMC5883L_Begin() {
	TWI_Start();
	TWI_WriteByte(HMC5883L_ADDR_WRITE);		//Start write
	TWI_WriteByte(0x00);					//Write start address
	TWI_WriteByte(0x70);					//Configure register A as 8-average, 15 Hz default, normal measurement
	TWI_WriteByte(0xA0);					//Configure register B for gain
	TWI_WriteByte(0x00);					//Configure register Mode to continuous measurement
	TWI_Stop();
}
extern void HMC5883L_ReadXZY(int16_t* datain) {
	uint8_t buffer[6];
	uint8_t counter = 6;
	
	TWI_Start();
	TWI_WriteByte(HMC5883L_ADDR_WRITE);
	TWI_WriteByte(HMC5883L_REGISTER_MSB_X);
	TWI_Start();
	TWI_WriteByte(HMC5883L_ADDR_READ);
	while(counter--) { TWI_ReadByte(&buffer[counter], counter); }
	TWI_Stop();
	
	datain[0] = (((int16_t)buffer[4] << 8) | (int16_t)buffer[5]);
	datain[1] = (((int16_t)buffer[2] << 8) | (int16_t)buffer[3]);
	datain[2] = (((int16_t)buffer[0] << 8) | (int16_t)buffer[1]);
}
extern int16_t HMC5883L_ReadX() { return HMC5883L_Measure(HMC5883L_REGISTER_MSB_X); }
extern int16_t HMC5883L_ReadY() { return HMC5883L_Measure(HMC5883L_REGISTER_MSB_Y); }
extern int16_t HMC5883L_ReadZ() { return HMC5883L_Measure(HMC5883L_REGISTER_MSB_Z); }
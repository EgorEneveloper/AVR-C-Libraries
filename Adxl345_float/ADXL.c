#include <stdint.h>
#include "MyTWI.h"
#include "ADXL.h"

uint8_t ADXL_dev_addr;

uint8_t ADXL345_Search(){
	uint8_t device = 0x00;
	TWI_ReadBytes(ADXL_dev_addr, ADXL_DEV_REG, 1, &device);
	
	if(device == ADXL_DEVICE)	{ return 1; }
	else						{ return 0; }
}
void ADXL345_Configure(){
	uint8_t conf = ADXL_SELF_TEST_OFF;
	conf |= ADXL_SPI_OFF;
	conf |= ADXL_INT_INVERT_OFF;
	conf |= ADXL_FULL_RES_ON;
	conf |= ADXL_JUSTIFY_OFF;
	conf |= ADXL_RANGE_16G;
	TWI_WriteRegister(ADXL_dev_addr, ADXL_DATA_FORMAT_REG, conf);
}
void ADXL345_PowerOn(){
	uint8_t power = ADXL_AUTO_SLEEP_OFF;
	power |= ADXL_MEASURE_ON;
	power |= ADXL_SLEEP_OFF;
	power |= ADXL_WAKEUP_8HZ;
	TWI_WriteRegister(ADXL_dev_addr, ADXL_POWER_REG, power);
}
uint8_t ADXL345_Begin(uint8_t dev_addr){
	ADXL_dev_addr = dev_addr;
	if(!ADXL345_Search()){ return 0; }
	
	TWI_WriteRegister(ADXL_dev_addr, ADXL_RATE_REG, ADXL_RATE_100);
	ADXL345_Configure();
	ADXL345_PowerOn();
	return 1;
}
void ADXL345_GetXYZ(int *x, int *y, int *z){
	uint8_t axis[6];
	TWI_ReadBytes(ADXL_dev_addr, 0x32, 6, axis);
	*x = (((int)axis[1]) << 8) | axis[0];
	*y = (((int)axis[3]) << 8) | axis[2];
	*z = (((int)axis[5]) << 8) | axis[4];
}
void ADXL345_ReadXYZ(float *x, float *y, float *z){
	int16_t X, Y, Z;
	ADXL345_GetXYZ(&X, &Y, &Z);
	*x = X * MULTIPLIER;
	*y = Y * MULTIPLIER;
	*z = Z * MULTIPLIER;
}
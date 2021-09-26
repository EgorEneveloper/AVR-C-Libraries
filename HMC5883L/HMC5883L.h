/*
 *	HMC5883L.h
 *	About : I2C Compass library
 *	Created: 29.07.2021 17:34:22
 *  Author: Zotov Egor
 */ 

#ifndef HMC5883L_H_
	#define HMC5883L_H_
	
	#define HMC5883L_ADDR_READ			0x3D
	#define HMC5883L_ADDR_WRITE			0x3C
	
	#define HMC5883L_REGISTER_STATUS	0x09
	#define HMC5883L_REGISTER_MSB_X		0x03
	#define HMC5883L_REGISTER_MSB_Z		0x05
	#define HMC5883L_REGISTER_MSB_Y		0x08
	#define HMC5883L_REGISTER_IDENTY	0x0A
	
	extern void HMC5883L_Begin();
	
	extern int16_t HMC5883L_ReadX(void);
	extern int16_t HMC5883L_ReadY(void);
	extern int16_t HMC5883L_ReadZ(void);
	extern void HMC5883L_ReadXZY(int16_t* datain); //data order: X -> Z -> Y
	
#endif /* HMC5883L_H_ */
/*
 * BMP180.h
 *
 * Created: 27.08.2021 17:56:49
 *  Author: Egor
 */ 


#ifndef BMP180_H_
	#define BMP180_H_
	
	#define BMP180_ADDR					0x77
	
	#define BMP180_REGISTER_DIG_AC1		0xAA
	#define BMP180_REGISTER_DIG_AC2		0xAC
	#define BMP180_REGISTER_DIG_AC3		0xAE
	#define BMP180_REGISTER_DIG_AC4		0xB0
	#define BMP180_REGISTER_DIG_AC5		0xB2
	#define BMP180_REGISTER_DIG_AC6		0xB4
	#define BMP180_REGISTER_DIG_B1		0xB6
	#define BMP180_REGISTER_DIG_B2		0xB8
	#define BMP180_REGISTER_DIG_MB		0xBA
	#define BMP180_REGISTER_DIG_MC		0xBC
	#define BMP180_REGISTER_DIG_MD		0xBE
	
	#define BMP180_REGISTER_XLSB		0xF8
	#define BMP180_REGISTER_LSB			0xF7
	#define BMP180_REGISTER_MSB			0xF6
	#define BMP180_REGISTER_CTRL		0xF4
	#define BMP180_REGISTER_CHIPID		0xD0
	#define BMP180_CHIPID_VALUE			0x55
	
	#define BMP180_MEASURE_Temperature				0x2E
	#define BMP180_MEASURE_Pressure_LowPower		0
	#define BMP180_MEASURE_Pressure_Standard		1
	#define BMP180_MEASURE_Pressure_HighRes			2
	#define BMP180_MEASURE_Pressure_UHighRes		3
	
	uint8_t BMP180_begin(void);
	void BMP180_receive(long* temperature, long* pressure, uint8_t PressurePrecision);
#endif /* BMP180_H_ */
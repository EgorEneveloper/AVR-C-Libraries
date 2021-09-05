#ifndef BMP280_H_
	#define BMP280_H_
	
	#include <stdint.h>
	
	#define BMP_ADDR_DEFAULT 0x77
	#define BMP_ADDR_ALTERNT 0x76
	
	#define BMP_REGISTER_DIG_T1 0x88
	#define BMP_REGISTER_DIG_T2 0x8A
	#define BMP_REGISTER_DIG_T3 0x8C

	#define BMP_REGISTER_DIG_P1 0x8E
	#define BMP_REGISTER_DIG_P2 0x90
	#define BMP_REGISTER_DIG_P3 0x92
	#define BMP_REGISTER_DIG_P4 0x94
	#define BMP_REGISTER_DIG_P5 0x96
	#define BMP_REGISTER_DIG_P6 0x98
	#define BMP_REGISTER_DIG_P7 0x9A
	#define BMP_REGISTER_DIG_P8 0x9C
	#define BMP_REGISTER_DIG_P9 0x9E
	
	#define BMP_REGISTER_CHIPID  0xD0
	#define BMP_CHIPID_VALUE  0x58
	
	#define BMP_REGISTER_CONFIG  0xF5
	#define BMP280_REGISTER_PRESSUREDATA 0xF7
	#define BMP280_REGISTER_TEMPDATA     0xFA
	#define BMP280_REGISTER_CONTROL 0xF4
	
	#define BMP280_OVERSAMPLING_T2        0b10
	#define BMP280_OVERSAMPLING_P16        0b101
	#define BMP280_TSB_250                0b11
	
	#define BMP280_MODE_FORCED            0b01
	#define BMP280_FILTER_COEFFICIENT4    0b10
	#define	BMP280_SPI_OFF	0b00
	#define	BMP280_SPI_ON	0b01
	
	#define	BMP280_MEAS			(BMP280_OVERSAMPLING_T2<<5)|(BMP280_OVERSAMPLING_P16<<2)|(BMP280_MODE_FORCED)
	#define	BMP280_CONFIG		(BMP280_TSB_250<<5) | (BMP280_FILTER_COEFFICIENT4<<2)|(BMP280_SPI_OFF)
	
	uint8_t BMP280_begin(uint8_t devaddr);
	uint8_t BMP280_receiveData(int32_t *temperature, float *pressure,float *alt);
	uint8_t BMP280_read8(uint8_t address);
#endif /* BMP280_H_ */
#ifndef ADXL_H_
	#define ADXL_H_
	
	#define ADXL_DEV_REG 0x00
	#define ADXL_POWER_REG 0x2D
	#define ADXL_DEVICE 0xE5
	
	#define ADXL_ADDR_DEFAULT 0xD1
	#define ADXL_ADDR_SEC	  0x1D
	#define ADXL_ADDR_ALTERNT 0x53
	
	#define ADXL_RATE_REG 0x2C
	#define ADXL_RATE_100 0b1010
	#define ADXL_RATE_25 0b1000
	
	#define ADXL_DATA_FORMAT_REG 0x31
	#define ADXL_SELF_TEST_OFF 0b00000000
	#define ADXL_SPI_OFF 0b00000000
	#define ADXL_INT_INVERT_OFF 0b00000000
	#define ADXL_FULL_RES_ON 0b00001000
	#define ADXL_JUSTIFY_OFF 0b00000000
	#define ADXL_RANGE_16G 0b11
	
	#define ADXL_AUTO_SLEEP_OFF 0b00000000
	#define ADXL_MEASURE_ON 0b00001000
	#define ADXL_SLEEP_OFF 0b00000000
	#define ADXL_WAKEUP_8HZ 0b00
	
	//#define MULTIPLIER 0.03828125f reDef for our mission
	#define MULTIPLIER 3.828125f
	
	void ADXL345_ReadXYZ(int16_t *x, int16_t *y, int16_t *z);
	uint8_t ADXL345_Begin(uint8_t dev_addr);
#endif /* ADXL_H_ */
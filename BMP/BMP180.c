/*
 * BMP180.c
 *
 * Created: 27.08.2021 17:56:34
 *  Author: Egor
 */ 

#ifndef F_CPU
	#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

#include "TWI.h"
#include "BMP180.h"

static short dig_AC1;
static short dig_AC2;
static short dig_AC3;
static unsigned short dig_AC4;
static unsigned short dig_AC5;
static unsigned short dig_AC6;
static short dig_B1;
static short dig_B2;
static short dig_MB;
static short dig_MC;
static short dig_MD;

void BMP180_write8(uint8_t registr, uint8_t data) {
	TWI_WriteRegister(BMP180_ADDR, registr, data);
}
uint8_t BMP180_read8(uint8_t registr) {
	uint8_t buffer;
	TWI_ReadBytes(BMP180_ADDR, registr, 1, &buffer);
	return buffer;
}
uint16_t BMP180_readU16(uint8_t registr) {
	uint8_t buffer[2];
	TWI_ReadBytes(BMP180_ADDR, registr, 2, buffer);
	return ((uint16_t)buffer[0] << 8) | buffer[1];
}
int16_t BMP180_readS16(uint8_t registr) {
	return BMP180_readU16(registr);
}
uint32_t BMP180_read32(uint8_t registr) {
	uint32_t data;
	uint8_t buffer[3];
	
	TWI_ReadBytes(BMP180_ADDR, registr, 3, buffer);
	data = buffer[0]; data <<= 8;
	data |= buffer[1]; data <<= 8;
	data |= buffer[2];
	return data;
}
void BMP180_readCalibration() {
	dig_AC1 = BMP180_readS16(BMP180_REGISTER_DIG_AC1);
	dig_AC2 = BMP180_readS16(BMP180_REGISTER_DIG_AC2);
	dig_AC3 = BMP180_readS16(BMP180_REGISTER_DIG_AC3);
	dig_AC4 = BMP180_readU16(BMP180_REGISTER_DIG_AC4);
	dig_AC5 = BMP180_readU16(BMP180_REGISTER_DIG_AC5);
	dig_AC6 = BMP180_readU16(BMP180_REGISTER_DIG_AC6);
	dig_B1 = BMP180_readS16(BMP180_REGISTER_DIG_B1);
	dig_B2 = BMP180_readS16(BMP180_REGISTER_DIG_B2);
	dig_MB = BMP180_readS16(BMP180_REGISTER_DIG_MB);
	dig_MC = BMP180_readS16(BMP180_REGISTER_DIG_MC);
	dig_MD = BMP180_readS16(BMP180_REGISTER_DIG_MD);
}
long BMP180_getUT(void) {
	BMP180_write8(BMP180_REGISTER_CTRL, BMP180_MEASURE_Temperature);
	_delay_us(4500);
	return BMP180_readU16(BMP180_REGISTER_MSB);
}
long BMP180_getUP(uint8_t PressurePrecision) {
	uint8_t buffer[3];
	BMP180_write8(BMP180_REGISTER_CTRL, 0x34 + (PressurePrecision << 6));
	_delay_ms(14);
	TWI_ReadBytes(BMP180_ADDR, BMP180_REGISTER_MSB, 3, buffer);
	return (((((long)buffer[0]) << 16) + (((long)buffer[1]) << 8) + ((long)buffer[2])) >> (8 - PressurePrecision));
}
uint8_t BMP180_begin(void) {
	if (BMP180_read8(BMP180_REGISTER_CHIPID) != BMP180_CHIPID_VALUE) { return 0; }
	BMP180_readCalibration();
	return 1;
}
void BMP180_receive(long* temperature, long* pressure, uint8_t PressurePrecision) {
	long x1 = (BMP180_getUT() - dig_AC6) * dig_AC5 / 32768;
	long x2 = dig_MC * 2048L / (x1 + dig_MD);
	long b5 = x1 + x2;
	*temperature = (b5 + 8) / 16;
	
	long b6 = b5 - 4000;
	x1 = (dig_B2 * ((b6 * b6) >> 12)) >> 11;
	x2 = (dig_AC2 * b6) >> 11;
	long x3 = x1 + x2;
	long b3 = (((dig_AC1 * 4 + x3) << PressurePrecision) + 2) >> 2;
	x1 = (dig_AC3 * b6) >> 13;
	x2 = (dig_B1 * ((b6 * b6) >> 12)) >> 16;
	x3 = ((x1 + x2) + 2) >> 2;
	unsigned long b4 = (dig_AC4 * (unsigned long)(x3 + 32768)) >> 15;
	unsigned long b7 = ((unsigned long)BMP180_getUP(PressurePrecision) - b3) * (50000 >> PressurePrecision);
	long p = (b7 < 0x80000000) ? ((b7 << 1) / b4) : ((b7 / b4) << 1);
	
	x1 = (p >> 8) * (p >> 8);
	x1 = (x1 * 3038) >> 16;
	x2 = (-7357 * p) >> 16;
	*pressure = p + ((x1 + x2 + 3791) >> 4);
	return;
}
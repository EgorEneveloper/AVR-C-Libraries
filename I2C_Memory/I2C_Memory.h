/*
 *	I2C_Memory.h
 *	This is a library for I2C Serial EEPROM chips
 *	Created: 26.12.2021 15:35:12
 *  Author: Zotov Egor
 */ 

#ifndef I2C_MEMORY_H_
	#define I2C_MEMORY_H_
	
	typedef struct I2CMEM {
		uint8_t chipaddr;
		uint8_t lenaddr;
	} I2CMEM;
	I2CMEM I2CMEM_Addr_New(uint8_t BitA2, uint8_t BitA1, uint8_t BitA0, uint8_t AddressBytesNum);
	
	uint8_t I2CMEM_ReadByte(I2CMEM memory, unsigned dataAddr);
	void I2CMEM_WriteByte(I2CMEM memory, unsigned dataAddr, uint8_t data);
	void I2CMEM_WriteArray(I2CMEM memory, unsigned dataAddr, uint8_t* data, uint8_t len);
	void I2CMEM_ReadArray(I2CMEM memory, unsigned startAddr, uint8_t* data, uint8_t len);
	
#endif /* I2C_MEMORY_H_ */
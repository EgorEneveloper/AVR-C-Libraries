#ifndef MYSERIAL_H_
	#define MYSERIAL_H_
	
	#define SERIAL_9600_BOD_8MHZ		51
	#define SEIRAL_38400_BOD_8MHZ		12
	#define SERIAL_256000_BOD_8MHZ		1
	
	void Serial_Begin(void);
	uint8_t Serial_Read(void);
	uint8_t Serial_Available(void);
	unsigned long Serial_Read_Num(char separator, uint8_t maxnums);
	void Serial_Write_char(uint8_t data);
	void Serial_Write_Num(unsigned long num);
	void Serial_Write_NewLine(void);
#endif /* MYSERIAL_H_ */
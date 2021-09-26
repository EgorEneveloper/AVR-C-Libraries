#ifndef MYSERIAL_H_
	#define MYSERIAL_H_
	
	#define SERIAL_BAUD(x)				(((F_CPU / (x * 16UL))) - 1)
	
	void Serial_Begin(unsigned baud);
	uint8_t Serial_Read(void);
	uint8_t Serial_Available(void);
	unsigned long Serial_Read_Num(char separator, uint8_t maxnums);
	void Serial_Write_char(uint8_t data);
	void Serial_Write_Num(long num);
	void Serial_Write_NewLine(void);
#endif /* MYSERIAL_H_ */
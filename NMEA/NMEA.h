/*
 *	NMEA.h
 *
 *	Created: 27.07.2021 23:27:42
 *	Author: Zotov Egor
 */ 

#ifndef NMEA_H_
	#define NMEA_H_
	
	typedef struct NMEA_MESSAGE {
		unsigned long latitude;
		unsigned long longitude;
	} NMEA_MESSAGE;
	
	#define NMEA_BUFFER_SIZE 82
	
	extern uint8_t NMEA_Proceed_Char(uint8_t data);
	extern NMEA_MESSAGE NMEA_GetMessage(void);
	extern uint8_t NMEA_TryReceive(unsigned long maxWait_ms);
	
#endif /* NMEA_H_ */
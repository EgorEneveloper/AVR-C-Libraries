/*
 *	NMEA.c
 *
 *	Created: 27.07.2021 23:27:54
 *	Author: Zotov Egor
 */ 

#include <avr/io.h>
#include <inttypes.h>
#include "MySerial.h"
#include "NMEA.h"

static uint8_t NMEA_Buffer[NMEA_BUFFER_SIZE] = { 0 };
static uint8_t NMEA_CurrentChar = 0; //controls buffer fill
static uint8_t NMEA_CurrentWord = 0; //controls message analysis

static NMEA_MESSAGE NMEA_LastMsg = { 0, 0 };

static unsigned long NMEA_DoubleToUnsigned(double data) {
	for (uint8_t i = 0; i < 12; ++i) { if (data < 100000000UL) { data *= 10; } }
	return data;
}
static uint8_t NMEA_Parse_GEO(uint8_t order_lat, uint8_t order_lon) {
	double degrees = 0, minutes = 0, deciminutes = 0, divider = 1;
	unsigned long coord;
	uint8_t dot_passed = 0;
	
	for (uint8_t i = 0; i < NMEA_CurrentChar; ++i) {
		if (NMEA_Buffer[i] == ',') {
			coord = NMEA_DoubleToUnsigned(degrees + ((minutes + deciminutes) / 60));
			
			if (NMEA_CurrentWord == order_lat && coord) { NMEA_LastMsg.latitude = coord; }
			else if (NMEA_CurrentWord == order_lon && coord) { NMEA_LastMsg.longitude = coord; return 1; }
				
			degrees = 0; minutes = 0; deciminutes = 0; divider = 1; dot_passed = 0;
			++NMEA_CurrentWord; continue;
		}
		if (NMEA_CurrentWord == order_lat) {
			if (NMEA_Buffer[i] == '.') {
				degrees += (NMEA_Buffer[i - 4] & 0x0F); degrees *= 10;
				degrees += (NMEA_Buffer[i - 3] & 0x0F);
				minutes += (NMEA_Buffer[i - 2] & 0x0F); minutes *= 10;
				minutes += (NMEA_Buffer[i - 1] & 0x0F);
				
				dot_passed = 1;
				continue;
			}
			
			if (dot_passed) { divider *= 10; double toAdd = (NMEA_Buffer[i] & 0x0F); toAdd /= divider; deciminutes += toAdd; }
			continue;
		}
		if (NMEA_CurrentWord == order_lon) {
			if (NMEA_Buffer[i] == '.') {
				degrees += (NMEA_Buffer[i - 5] & 0x0F); degrees *= 10;
				degrees += (NMEA_Buffer[i - 4] & 0x0F); degrees *= 10;
				degrees += (NMEA_Buffer[i - 3] & 0x0F);
				minutes += (NMEA_Buffer[i - 2] & 0x0F); minutes *= 10;
				minutes += (NMEA_Buffer[i - 1] & 0x0F);
				
				dot_passed = 1;
				continue;
			}
			
			if (dot_passed) { divider *= 10; double toAdd = (NMEA_Buffer[i] & 0x0F); toAdd /= divider; deciminutes += toAdd; }
			continue;
		}
		if (NMEA_CurrentWord > order_lon) { return 0; }
	}
	return 0;
}
static uint8_t NMEA_Proceed_Msg() {
	NMEA_CurrentWord = 0;
	if (NMEA_Buffer[0] != '$') { return 0; }
	if (NMEA_Buffer[3] == 'G') {
		if (NMEA_Buffer[4] == 'G' && NMEA_Buffer[5] == 'A') { return NMEA_Parse_GEO(2, 4); }
		if (NMEA_Buffer[4] == 'L' && NMEA_Buffer[5] == 'L') { return NMEA_Parse_GEO(1, 3); }
		return 0;
	}
	if (NMEA_Buffer[3] == 'R' && NMEA_Buffer[4] == 'M' && NMEA_Buffer[5] == 'C') { return NMEA_Parse_GEO(3, 5); }
	return 0;
}
extern uint8_t NMEA_Proceed_Char(uint8_t data) {
	uint8_t ans;
	
	if (NMEA_CurrentChar == NMEA_BUFFER_SIZE) { ans = NMEA_Proceed_Msg(); NMEA_CurrentChar = 0; return ans; }
	if (data == '\n') { ans = NMEA_Proceed_Msg(); NMEA_CurrentChar = 0; return ans; }
	
	NMEA_Buffer[NMEA_CurrentChar++] = data;
	return 0;
}
extern uint8_t NMEA_TryReceive(unsigned long maxWait_ms) {
	maxWait_ms *= 256UL;
	
	while (--maxWait_ms) {
		if (Serial_Available()) {
			if (NMEA_Proceed_Char(UDR0)) { return 1; }
		}
	}
	return 0;
}
extern NMEA_MESSAGE NMEA_GetMessage(void) { return NMEA_LastMsg; }
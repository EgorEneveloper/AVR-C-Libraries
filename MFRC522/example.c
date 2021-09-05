#include <avr/io.h>
#include <util/delay.h>
#include "MySerial.h"
#include "SPI.h"
#include "MFRC522.h"

uint8_t outputdata[5];

int main(void) {
	Serial_Begin();
	spi_init();
	MFRC522_Init();
	
    while(1) {
        _delay_ms(500);
        if (MFRC522_ReadCard(outputdata) == 1) {
			for (uint8_t i = 0; i < 5; ++i) { Serial_Write_AsCharArray(outputdata[i]); }
			Serial_Write_NewLine();
		}
    }
}
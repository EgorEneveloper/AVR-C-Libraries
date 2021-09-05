/*
 * TFT.c
 *	About:		A library for ST7735 TFT displays
 *	Created:	16.04.2021 18:02:47
 *  Author:		Zotov Egor
 */ 

#ifndef F_CPU
	#define F_CPU 8000000UL
#endif

#define TFT_CHAR_WIDTH	7
#define TFT_CHAR_HEIGHT	8

#include <avr/io.h>
#include <util/delay.h>
#include "SPI.h"
#include "TFT.h"
#include "TFT_DEFLIST.h"

//Standard numeral characters 0-9, -
const uint8_t TFT_CHAR_0[TFT_CHAR_WIDTH] = { 0x00, 0x3E, 0x45, 0x49, 0x51, 0x3E, 0x00 };
const uint8_t TFT_CHAR_1[TFT_CHAR_WIDTH] = { 0x00, 0x00, 0x21, 0x7F, 0x01, 0x00, 0x00 };
const uint8_t TFT_CHAR_2[TFT_CHAR_WIDTH] = { 0x00, 0x21, 0x43, 0x45, 0x49, 0x31, 0x00 };
const uint8_t TFT_CHAR_3[TFT_CHAR_WIDTH] = { 0x00, 0x22, 0x41, 0x49, 0x49, 0x36, 0x00 };
const uint8_t TFT_CHAR_4[TFT_CHAR_WIDTH] = { 0x00, 0x0C, 0x14, 0x24, 0x7F, 0x04, 0x00 };
const uint8_t TFT_CHAR_5[TFT_CHAR_WIDTH] = { 0x00, 0x72, 0x51, 0x51, 0x51, 0x4E, 0x00 };
const uint8_t TFT_CHAR_6[TFT_CHAR_WIDTH] = { 0x00, 0x3E, 0x51, 0x51, 0x51, 0x0E, 0x00 };
const uint8_t TFT_CHAR_7[TFT_CHAR_WIDTH] = { 0x00, 0x40, 0x40, 0x47, 0x58, 0x60, 0x00 };
const uint8_t TFT_CHAR_8[TFT_CHAR_WIDTH] = { 0x00, 0x36, 0x49, 0x49, 0x49, 0x36, 0x00 };
const uint8_t TFT_CHAR_9[TFT_CHAR_WIDTH] = { 0x00, 0x32, 0x49, 0x49, 0x49, 0x3E, 0x00 };
const uint8_t TFT_CHAR_MINUS[TFT_CHAR_WIDTH] = { 0x00, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00 };

//Array with pointers to characters 0-9
const uint8_t* TFT_CHAR_NUM[10] = { TFT_CHAR_0, TFT_CHAR_1, TFT_CHAR_2, TFT_CHAR_3, TFT_CHAR_4, TFT_CHAR_5, TFT_CHAR_6, TFT_CHAR_7, TFT_CHAR_8, TFT_CHAR_9 };

//Static functions for TFT.c purpose
static void Display_SS_BEGIN(TFT_Display* Display)		{ *Display->PORT &= ~Display->mask_SS; }
static void Display_SS_END(TFT_Display* Display)		{ *Display->PORT |= Display->mask_SS; }
static void Display_MODE_CMD(TFT_Display* Display)		{ *Display->PORT &= ~Display->mask_DC; }
static void Display_MODE_DATA(TFT_Display* Display)		{ *Display->PORT |= Display->mask_DC; }
static int16_t TFT_abs(int16_t val) { return val < 0 ? -val : val; }

//Other functions for USER purpose
extern TFT_Display New_TFT_Display(volatile uint8_t* TFT_DDR, volatile uint8_t* TFT_PORT, uint8_t Reset_mask, uint8_t DC_mask, uint8_t SS_mask) {
	TFT_Display new_Display = { TFT_DDR, TFT_PORT, Reset_mask, DC_mask, SS_mask };
	
	*TFT_DDR |= Reset_mask | DC_mask | SS_mask;
	*TFT_PORT |= Reset_mask | DC_mask | SS_mask;
	
	Display_MODE_CMD(&new_Display);
	
	Display_SS_BEGIN(&new_Display);
	spi_SendRecvByte(TFT_SWRESET);
	Display_SS_END(&new_Display);
	_delay_ms(200);
	
	Display_SS_BEGIN(&new_Display);
	spi_SendRecvByte(TFT_SLPOUT);
	Display_SS_END(&new_Display);
	_delay_ms(200);
	
	Display_SS_BEGIN(&new_Display);
	spi_SendRecvByte(TFT_COLMOD);
	Display_MODE_DATA(&new_Display);
	spi_SendRecvByte(TFT_COLMOD_565);
	Display_SS_END(&new_Display);
	Display_MODE_CMD(&new_Display);
	
	Display_SS_BEGIN(&new_Display);
	spi_SendRecvByte(TFT_DISPON);
	spi_SendRecvByte(TFT_IVNOFF);
	spi_SendRecvByte(TFT_MADCTL);
	Display_MODE_DATA(&new_Display);
	spi_SendRecvByte(0b00000000);
	Display_SS_END(&new_Display);
	
	return new_Display;
}
extern unsigned long TFT_CONVERT_COLOR_666(unsigned long color) { return color; }
extern unsigned long TFT_CONVERT_COLOR_565(unsigned long color) {
	unsigned long convres = ((color >> 16) & 0b11111000) | ((color >> 8) & 0b00000111);
	convres <<= 8;
	return convres | (((color >> 8) & 0b11100000) | (color & 0b00011111));
}
extern void TFT_WRITE_COLOR(unsigned long color) {
	//spi_SendRecvByte(color >> 16);
	spi_SendRecvByte(color >> 8);
	spi_SendRecvByte(color);
}
extern void TFT_WRITE_COLORSEQUENCE(unsigned long color, unsigned amount) {
	for (unsigned i = 0; i < amount; ++i) { TFT_WRITE_COLOR(color); }
}
extern void TFT_SET_RECT(TFT_Display* Display, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
	Display_MODE_CMD(Display);
	spi_SendRecvByte(TFT_CASET);
	Display_MODE_DATA(Display);
	spi_SendRecvByte(x0 >> 8);
	spi_SendRecvByte(x0);
	spi_SendRecvByte(x1 >> 8);
	spi_SendRecvByte(x1);
	Display_MODE_CMD(Display);
	spi_SendRecvByte(TFT_RASET);
	Display_MODE_DATA(Display);
	spi_SendRecvByte(y0 >> 8);
	spi_SendRecvByte(y0);
	spi_SendRecvByte(y1 >> 8);
	spi_SendRecvByte(y1);
}
extern void TFT_DRAW_FILLRECT(TFT_Display* Display, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, unsigned long color) {
	int width = x1 - x0 + 1, height = y1 - y0 + 1;
	Display_SS_BEGIN(Display);
	TFT_SET_RECT(Display, x0, y0, x1, y1);
	Display_MODE_CMD(Display);
	spi_SendRecvByte(TFT_RAMWR);
	Display_MODE_DATA(Display);
	TFT_WRITE_COLORSEQUENCE(color, width * height);
	Display_SS_END(Display);
}
extern void TFT_DRAW_PIXEL(TFT_Display* Display, uint16_t x, uint16_t y, unsigned long color) {
	Display_SS_BEGIN(Display);
	TFT_SET_RECT(Display, x, y, x, y);
	Display_MODE_CMD(Display);
	spi_SendRecvByte(TFT_RAMWR);
	Display_MODE_DATA(Display);
	TFT_WRITE_COLOR(color);
	Display_SS_END(Display);
}
void TFT_Circle_points(TFT_Display* Display, int16_t cx, int16_t cy, int16_t x, int16_t y, unsigned long color) {
	TFT_DRAW_PIXEL(Display, cx + x, cy + y, color);
	TFT_DRAW_PIXEL(Display, cx - x, cy + y, color);
	TFT_DRAW_PIXEL(Display, cx + x, cy - y, color);
	TFT_DRAW_PIXEL(Display, cx - x, cy - y, color);
	TFT_DRAW_PIXEL(Display, cx + y, cy + x, color);
	TFT_DRAW_PIXEL(Display, cx - y, cy + x, color);
	TFT_DRAW_PIXEL(Display, cx + y, cy - x, color);
	TFT_DRAW_PIXEL(Display, cx - y, cy - x, color);
}
extern void TFT_DRAW_CIRCLE(TFT_Display* Display, int16_t cx, int16_t cy, int16_t radius, unsigned long color) {
	int16_t x = 0, y = radius;
	int16_t delta = 3 - (radius << 1);
	TFT_Circle_points(Display, cx, cy, x, y, color);
	while (y >= x) {
		++x;
		if (delta > 0) { --y; delta += ((x - y) * 4) + 10; }
		else { delta += (x * 4) + 6; }
		TFT_Circle_points(Display, cx, cy, x, y, color);
	}
}
extern void TFT_DRAW_LINE(TFT_Display* Display, int16_t x1, int16_t y1, int16_t x2, int16_t y2, unsigned long color) {
	int16_t dx = TFT_abs(x2 - x1), dy = -TFT_abs(y2 - y1);
	int16_t sx = x1 < x2 ? 1 : -1, sy = y1 < y2 ? 1 : -1;
	int16_t err = dx + dy;
	
	while (1) {
		TFT_DRAW_PIXEL(Display, x1, y1, color);
		if (x1 == x2 && y1 == y2) { break; }
		
		int16_t errxy = err * 2;
		if (errxy >= dy) { err += dy; x1 += sx; }
		if (errxy <= dx) { err += dx; y1 += sy; }
	}
}
extern void TFT_WRITE_CHAR(TFT_Display* Display, int16_t x, int16_t y, const uint8_t* ch, unsigned long color, unsigned long background) {
	Display_SS_BEGIN(Display);
	TFT_SET_RECT(Display, y, x, y + TFT_CHAR_HEIGHT - 1, x + TFT_CHAR_WIDTH - 1);
	Display_MODE_CMD(Display);
	spi_SendRecvByte(TFT_RAMWR);
	Display_MODE_DATA(Display);
	for (uint8_t i = 0; i < TFT_CHAR_WIDTH; ++i) {
		for (uint8_t j = 0; j < TFT_CHAR_HEIGHT; ++j) {
			if ((ch[i] << j) & 0b10000000) { TFT_WRITE_COLOR(color); }
			else { TFT_WRITE_COLOR(background); }
		}
	}
	Display_SS_END(Display);
}
extern void TFT_WRITE_NUM(TFT_Display* Display, int16_t x, int16_t y, long num, unsigned long color, unsigned long background) {
	uint8_t isnegative = num < 0;
	if (isnegative) { num = -num; }
	
	do {
		TFT_WRITE_CHAR(Display, x, y, TFT_CHAR_NUM[num % 10], color, background);
		num /= 10;
		x -= TFT_CHAR_WIDTH;
	} while (num);
	
	if (isnegative) { TFT_WRITE_CHAR(Display, x, y, TFT_CHAR_MINUS, color, background); }
}
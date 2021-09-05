/*
 * TFT.h
 *	About:		A library for ST7735 TFT displays
 *	Created:	16.04.2021 18:02:47
 *  Author:		Zotov Egor
 */ 

#ifndef TFT_H_
	#define TFT_H_
	
	typedef struct TFT_Display {
		volatile uint8_t* DDR;
		volatile uint8_t* PORT;
		uint8_t mask_Reset;
		uint8_t mask_DC;
		uint8_t mask_SS;
	} TFT_Display;
	
	extern TFT_Display New_TFT_Display(volatile uint8_t* TFT_DDR, volatile uint8_t* TFT_PORT, uint8_t Reset_mask, uint8_t DC_mask, uint8_t SS_mask);
	extern unsigned long TFT_CONVERT_COLOR_666(unsigned long color);
	extern unsigned long TFT_CONVERT_COLOR_565(unsigned long color);
	
	extern void TFT_WRITE_COLOR(unsigned long color); // Standard color structure - 0xRRGGBB
	extern void TFT_WRITE_COLORSEQUENCE(unsigned long color, unsigned amount);
	extern void TFT_SET_RECT(TFT_Display* Display,uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
	extern void TFT_DRAW_FILLRECT(TFT_Display* Display, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, unsigned long color);
	extern void TFT_DRAW_CIRCLE(TFT_Display* Display, int16_t cx, int16_t cy, int16_t radius, unsigned long color);
	extern void TFT_DRAW_PIXEL(TFT_Display* Display, uint16_t x, uint16_t y, unsigned long color);
	extern void TFT_DRAW_LINE(TFT_Display* Display, int16_t x1, int16_t y1, int16_t x2, int16_t y2, unsigned long color);
	extern void TFT_WRITE_CHAR(TFT_Display* Display, int16_t x, int16_t y, const uint8_t* ch, unsigned long color, unsigned long background);
	extern void TFT_WRITE_NUM(TFT_Display* Display, int16_t x, int16_t y, long num, unsigned long color, unsigned long background);
	
#endif /* TFT_H_ */
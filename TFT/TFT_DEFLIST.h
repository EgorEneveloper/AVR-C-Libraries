/*
 * TFT_DEFLIST.h
 *	About:		Definition list for st7735 TFT displays
 *	Created:	16.04.2021 18:22:04
 *  Author:		Zotov Egor
 */ 

#ifndef TFT_DEFLIST_H_
	#define TFT_DEFLIST_H_
	
	#define TFT_SWRESET		0x01
	#define TFT_SLPOUT		0x11
	#define TFT_DISPOFF		0x28
	#define TFT_DISPON		0x29
	#define TFT_MADCTL		0x36
	#define TFT_IVNOFF		0x21
	#define TFT_CASET		0x2A
	#define TFT_RASET		0x2B
	#define TFT_RAMWR		0x2C
	#define TFT_RAMRD		0x2E
	
	#define TFT_COLMOD		0x3A
	#define TFT_COLMOD_666	0x06
	#define TFT_COLMOD_565	0x05
	
#endif /* TFT_DEFLIST_H_ */
#include "utils.h"
#include "dpf.h"
#include "global.h"
#include "config.h"
#include "lcduser.h"

#pragma codeseg LCDAUX
#pragma constseg LCDAUX

extern void lcd_writeword(unsigned short rgb16);

#if 0
void lcd_demo(unsigned char n) __banked
{
	unsigned char i = 0;

	for (i = 0; i < n; i++) {
		RUB_WATCHDOG();
		clrscreen(RGB565(255, 127, 64));
		delay(200);
		clrscreen(RGB565(127, 255, 64));
		delay(200);
	}
	clrscreen(RGB565(0, 0, 0));
	// delay(1000);
}
#endif

static BYTE last = 0;

void draw_bar(BYTE d, BYTE height)
{
	BYTE u, v;

	if (d < last) {
		u = last;
		v = d;
	} else {
		u = d;
		v = last;
	}

	while (height > u) {
		lcd_writeword(g_bgcol);
		height--;
	}

	while (height > v) {
		lcd_writeword(g_fgcol);
		height--;
	}

	if (v == u) {
		height--;
		lcd_writeword(g_fgcol);
	}
	
	last = d;

	while (height--) {
		lcd_writeword(g_bgcol);
	} 
}

#define DRAW_HEIGHT 64

#if LCD_WIDTH == 128
#define SCOPE_OFFSETX 64
#else
#define SCOPE_OFFSETX 128
#endif

void draw_buf(BYTE n) __banked
{
	__pdata BYTE *buf = g_databuf;
	BYTE i;
	BYTE x;
	i = g_datacount - n;
	i++; i %= n;
	last = g_databuf[i];
	for (x = SCOPE_OFFSETX; x < (SCOPE_OFFSETX + n); x++) {
		// Oldest value:
		g_blit.x0 = x; g_blit.y0 = g_term.offset_y;
		g_blit.x1 = x; g_blit.y1 = g_term.offset_y + DRAW_HEIGHT - 1;
		disp_blit();
		draw_bar(g_databuf[i], DRAW_HEIGHT);
		i++; i %= n;
	}
}

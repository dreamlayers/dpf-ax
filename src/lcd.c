#include "utils.h"
#include "dpf.h"
#include "global.h"
#include "config.h"
#include "lcduser.h"

#pragma codeseg LCDAUX
#pragma constseg LCDAUX

struct blit __data g_blit;

struct lcd __idata g_lcd = {
	DEFAULT_ORIENTATION,
	7 // Brightness
};

static
void delay(int x)
{
	int i = 0;
	while (x--) {
		for (i = 0; i < 1000; i++) {
			_asm
				nop
				nop
			_endasm;
		}
	}
}


static
void write(BYTE d)
{
	// _B_T0IE = 0; // Disable timer0 IRQ
	d;
	_asm
		.include 'dpf.inc'
		mov a, dpl
		lcall otp_lcd_write
	_endasm;
}


void lcd_writeword(unsigned short rgb16)
{
	rgb16;
	_asm
	mov	a, dph
	lcall otp_lcd_write
	mov	a, dpl
	lcall otp_lcd_write
	_endasm;
}

void clrscreen(unsigned short col) __banked
{
	int x, y;
	col;
	g_blit.x0 = 0; g_blit.y0 = 0;
	g_blit.x1 = RESOL_X - 1; g_blit.y1 = RESOL_Y - 1;
	disp_blit();

	y = RESOL_Y;
	do {
		x = RESOL_X;
		do{
			lcd_writeword(col);
		} while (--x);
	} while (--y);
}

void lcd_init(void) __banked
{
	BYTE n;
	BYTE c;
	__code BYTE *p;

	p = init_sequence;

	while ((c = *p++) != CMD_END) {
		switch (c & MASK_CMD) {
		case CMD_CS:
			_LCD_CS = 0;
			break;
		case CMD_RESET:
			_LCD_RST = c & 1;
			break;
		case CMD_MDELAY:
			sleep(*p++);
			break;
		case CMD_SEQCMD:
			_LCD_A0 = 0; // CMD
			n = c & ~MASK_CMD;
			do {
				write(*p++);
			} while (--n);
			break;
		case CMD_SEQDATA:
			n = c & ~MASK_CMD;
			_LCD_A0 = 1; // Data
			do {
				write(*p++);
			} while (--n);
			break;
		}
	}
	lcd_orientation(g_lcd.orientation);
}

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
		lcd_writeword(g_term.bgcol);
		height--;
	}

	while (height > v) {
		lcd_writeword(g_term.col);
		height--;
	}

	if (v == u) {
		height--;
		lcd_writeword(g_term.col);
	}
	
	last = d;

	while (height--) {
		lcd_writeword(g_term.bgcol);
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

#define UDELAY() _asm nop nop nop nop _endasm

static BYTE lcd_read(void)
{
	BYTE v;
	_LCD_RD = 0;
	UDELAY();
	v = p3;
	_LCD_RD = 1;
	return v;
}

/* Attention: These are just stubs for now */
 
void fill_rect(__idata const unsigned char *b) __banked
{
	b;
}

void copy_rect(__idata const unsigned char *b) __banked
{
	unsigned short nlines;
	char dx, dy;

	g_blit.x0 = b[7] | (b[8] << 8);
	g_blit.y0 = b[9] | (b[10] << 8);
	g_blit.x1 = b[11] | (b[12] << 8);
	nlines = b[11] | (b[12] << 8);
	dx = b[13];
	dy = b[14];
}


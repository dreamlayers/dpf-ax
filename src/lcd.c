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

static
void doclrscreen(unsigned short col)
{
	unsigned short i;
	col;

	i = (RESOL_X * RESOL_Y) / 2;	// we assume that this is <= 0xFFFF!
	do {
		_asm
		mov	a, dph
		lcall otp_lcd_write
		mov	a, dpl
		lcall otp_lcd_write
		mov	a, dph
		lcall otp_lcd_write
		mov	a, dpl
		lcall otp_lcd_write
		_endasm;
	} while (--i);
}


void clrscreen(unsigned short col) __banked
{
	g_blit.x0 = 0; g_blit.y0 = 0;
	g_blit.x1 = RESOL_X - 1; g_blit.y1 = RESOL_Y - 1;
	disp_blit();
	doclrscreen(col);
}

void lcd_init(void) __banked
{
#if defined(LCD_CONTROLLER_CUSTOM)
	lcd_custom_init();
#else
	lcd_init_by_table(init_sequence);
#endif
	lcd_orientation(g_lcd.orientation);
}

void lcd_init_by_table(__code unsigned char *p)
{
	BYTE n;
	BYTE c;

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
}

#ifdef EXPERIMENTAL

static BYTE lcd_read(void)
{
	BYTE v;
	_LCD_RD = 0;
	UDELAY();
	v = p3;
	_LCD_RD = 1;
	return v;
}

#endif

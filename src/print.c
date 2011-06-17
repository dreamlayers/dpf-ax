/** Print routines for DPF
 *
 * <hackfin@section5.ch>
 *
 */

// #define USE_BUILDWIN_CRAP

#include "global.h"
#include "utils.h"
#include "print.h"

#include "ax206.h"
#include "dpf.h"

#pragma codeseg PRINTF
#pragma constseg PRINTF

struct term __data g_term = { 0, 0 };

// Debug monitor variables:
volatile BYTE __data g_fakeled;
volatile unsigned short __data g_count[4];
volatile unsigned short __data g_register[4];
volatile unsigned short __data g_adc;
volatile unsigned short __data g_retcode;

void disp_home(void) __banked
{
	g_term.x = 0;
	g_term.y = 0;
}


#ifdef USE_BUILDWIN_CRAP

void disp_address(void)
{
	unsigned short n;
	n = g_term.x * CHAR_WIDTH;
	lcd_cxl = n; lcd_cxh = n >> 8;
	n = lcd_cxl + CHAR_WIDTH - 1;
	lcd_dxl = n; lcd_dxh = n >> 8;
	n = g_term.y * CHAR_HEIGHT;
	lcd_cyl = n; lcd_cyh = n >> 8;
	n = lcd_cyl + CHAR_HEIGHT - 1;
	lcd_dyl = n; lcd_dyh = n >> 8;
	_asm
	.include 'dpf.inc'
	lcall	lcd_address
	_endasm;
}

#else

void disp_address(void)
{
	unsigned short n;
	n = g_term.x * CHAR_WIDTH;
	g_blit.x0 = n;
	n += CHAR_WIDTH - 1;
	g_blit.x1 = n;
	n = g_term.y * CHAR_HEIGHT;
	g_blit.y0 = n;
	n += CHAR_HEIGHT - 1;
	g_blit.y1 = n;
	disp_blit();
}
#endif


void guard_term(void)
{
	if (g_term.x >= NUM_COLS) {
		g_term.y++; g_term.x = 0;
	}
	if (g_term.y >= NUM_LINES) g_term.y = 0;
}

void rawputc(const char c)
{
	disp_address();
	blit_char4x8(c);
	g_term.x++;
	guard_term();
}

void putc(const char c)
{
	if (c < 32) {
		switch (c) {
			case 8: // BS
				g_term.x--; break;
			case 9: // TAB
				g_term.x += 8; break;
			case 10: // LF
				g_term.y++;
				g_term.x = 0;
				break;
			case 13: // CR
				// clr_line(g_term.x, g_term.y);
				g_term.x = 1;
				// clear_eol
				break;
			default:
		}
	} else {
		rawputc(c);
	}
}

void putchar(char c) __banked
{
	putc(c);
}

void putsp(const char __pdata *str) __banked
{
	char c;
	while ( (c = *str++) ) {
		putc(c);
	}
}

void putsi(const char __idata *str)
{
	char c;
	while ( (c = *str++) ) {
		putc(c);
	}
}

void putsc(const __code char *str)
{
	char c;

	while ( (c = *str++) ) {
		putc(c);
	}
}

#define LBYTE const __code BYTE

LBYTE g_hex[] = "0123456789ABCDEF";

void print_dec(unsigned char val) __banked
{
	char __pdata *p;
	unsigned char d = 10;
	BYTE n = 4;
 	p = &g_printbuf[n];
	*p = 0;
	do {
		*(--p) = (val % d) + '0';
		val /= d;
	} while (val && --n);
	putsp(p);
}

void out_short(unsigned short val)
{
	BYTE d = 10;
	char __pdata *p = &g_printbuf[4];
	*p-- =  '\0';
	*p-- = g_hex[val & 0xf];
	*p-- = g_hex[(val >> 4) & 0xf];
	*p-- = g_hex[(val >> 8) & 0xf];
	*p-- = g_hex[(val >> 12) & 0xf];

	putsp(g_printbuf);
}

void print_short(unsigned short val) __banked
{
	out_short(val);
}

void clr_line(BYTE n) __banked
{
	BYTE x;
	x = g_term.x;
	do {
		putc(' ');
	} while (--n);
	g_term.x = x;
}

#define TABLE_SIZE 8192

void term_selfont(BYTE which) __banked
{
	which += g_rgborder;
	g_chartbl_offs[1] = (TABLE_SIZE >> 8) * which;
}


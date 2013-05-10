/** Print routines for DPF
 *
 * <hackfin@section5.ch>
 *
 */

#include "global.h"
#include "utils.h"
#include "print.h"

#include "ax206.h"
#include "dpf.h"

#pragma codeseg PRINTF
#pragma constseg PRINTF

struct term __data g_term = { 0, 0, NUM_COLS_SMALL, NUM_LINES_SMALL };
unsigned short g_fgcol = DEV_SCREEN_COL;
unsigned short g_bgcol = DEV_SCREEN_BGCOL;

// Debug monitor variables:
volatile BYTE __data g_fakeled;
volatile unsigned short __data g_count[4];
volatile unsigned short __data g_register[4];
volatile unsigned short __data g_adc;
volatile unsigned short __data g_retcode;


void disp_address()
{
	unsigned short n;
    BYTE i;
#ifdef BUILD_DEVEL
    i = CHAR_WIDTH_SMALL;
#else    
    i = (g_largefont) ? CHAR_WIDTH_LARGE : CHAR_WIDTH_SMALL;
#endif
    n = g_term.x * i;
	g_blit.x0 = n;
	n += i - 1;
	g_blit.x1 = n;
#ifdef BUILD_DEVEL
	i = CHAR_HEIGHT_SMALL;
#else
	i = (g_largefont) ? CHAR_HEIGHT_LARGE : CHAR_HEIGHT_SMALL;
#endif
	n = g_term.y * i;
	g_blit.y0 = n;
	n += i - 1;
	g_blit.y1 = n;
	disp_blit();
}

void guard_term(void)
{
	if (g_term.x >= g_term.num_cols) {
		g_term.y++; g_term.x = 0;
	}
	if (g_term.y >= g_term.num_lines) g_term.y = 0;
}

void rawputc(const char c)
{
    disp_address();
    blit_char(c);
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

void out_hex(unsigned char val)
{
	char __pdata *p = &g_printbuf[2];
	*p-- =  '\0';
	*p-- = g_hex[val & 0xf];
	*p-- = g_hex[(val >> 4) & 0xf];
	putsp(g_printbuf);
}

void out_short(unsigned short val)
{
	out_hex(val >> 8);
	out_hex(val & 0xFF);
}

void print_short(unsigned short val) __banked
{
	out_short(val);
}

void print_hex(unsigned char val) __banked
{
	out_hex(val);
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
#ifdef BUILD_DEVEL
        g_term.num_cols = NUM_COLS_SMALL;
        g_term.num_lines = NUM_LINES_SMALL;
	//which += g_rgborder;	// rgborder no longer used from 0.4 up
	g_chartbl_offs[1] = (TABLE_SIZE >> 8) * which;
#else
    if (which == FONT_SMALL || which == FONT_SMALL_REVERSE)
    {
        g_largefont = 0;    
        g_term.num_cols = NUM_COLS_SMALL;
        g_term.num_lines = NUM_LINES_SMALL;
        set_color(DEV_SCREEN_COL); set_bgcolor(DEV_SCREEN_BGCOL);
        //which += g_rgborder;	// rgborder no longer used from 0.4 up
        g_chartbl_offs[1] = (TABLE_SIZE >> 8) * which;
    }
    else {
        if (which == FONT_LARGE) {
            set_color(DISP_SCREEN_COL); set_bgcolor(DISP_SCREEN_BGCOL);
        }
        else if (which == FONT_LARGE_REVERSE) {
            set_color(DISP_SCREEN_BGCOL); set_bgcolor(DISP_SCREEN_COL);
        }
        g_largefont = 1;
        g_term.num_cols = NUM_COLS_LARGE;
        g_term.num_lines = NUM_LINES_LARGE;
    }
    guard_term();
#endif
}

#ifndef BUILD_DEVEL
void notice_powerdown(void) __banked
{
	GOTOXY(1, 7);
	putsc(" Weak power!");
	GOTOXY(1, 8);
	putsc(" Turning off.");
}
#endif

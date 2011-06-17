#include "dpf.h"
#include "print.h"
#include "global.h"
#include "utils.h"

#pragma codeseg FLIX

__bit g_flixmode = 0;

void flix_toggle(void) __banked
{
	unsigned short cycles = 256;
	if (!g_flixmode) {
		clrscreen(RGB565(0, 0, 0));
		GOTOXY(NUM_COLS / 2 - 1, NUM_LINES / 2 - 1);
		term_selfont(1);
		putchar (' '); putchar (' ');
		GOTOXY(NUM_COLS / 2 - 1, NUM_LINES / 2);
		putchar (' '); putchar (' ');
		term_selfont(0);
	} else {
		clrscreen(RGB565(255, 255, 255));
	}

	g_flixmode ^= 1;
	tmr1cntl = 0;
	tmr1cnth = 0;
	// These are writeonly:
	tmr1perl = cycles;
	tmr1perh = cycles >> 8;
	cycles = (cycles - (cycles / 4));
	tmr1pwml = cycles;
	tmr1pwmh = cycles >> 8;
}


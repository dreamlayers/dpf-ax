#pragma codeseg PRINTFX
#pragma constseg PRINTFX

#include "ax206.h"
#include "global.h"
#include "dpf.h"
#include "utils.h"
#include "print.h"

#if 0
#define STR_CREDIT "by hackfin and superelchi"
static
void credits_short()
{
	GOTOXY(0, NUM_LINES_SMALL - 1);
	clr_line(NUM_COLS_SMALL);
	GOTOXY((NUM_COLS_SMALL - sizeof(STR_CREDIT)) / 2, NUM_LINES_SMALL - 1);
	puts(STR_CREDIT);
}
#else
static void credits_short() {}
#endif

void print_splash(void) __banked
{
	term_selfont(FONT_SMALL);
	switch (g_config.splash)
	{
		case SPLASH_BLACK: clrscreen(COLOR_BLACK); credits_short(); break;
		case SPLASH_WHITE: clrscreen(COLOR_WHITE); credits_short(); break;
		case SPLASH_IMAGE:
			set_color(COLOR_WHITE); set_bgcolor(COLOR_BLACK);
			g_blit.x0 = 0; g_blit.y0 = 0;
			g_blit.x1 = RESOL_X - 1; g_blit.y1 = RESOL_Y - 1;
			disp_blit();
			_blit_splash(RESOL_X, RESOL_Y);
			credits_short();
			break;
		default: ;
	}
}

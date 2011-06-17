/* Rudimentary Get/Set property handler */

#pragma codeseg INIT

#include "dpf.h"
#include "utils.h"
#include "global.h"
#include "usbuser.h"

RETCODE handle_setprop(__idata BYTE *b) __banked
{
	switch (b[7]) {
		case PROPERTY_BRIGHTNESS:
			g_lcd.brightness = b[9];
			if (g_lcd.brightness == 0) {
				tmr1con = 0;
				_LCD_LED = nOFF;
			} else {
				timer1_config(g_lcd.brightness);
				_LCD_LED = nON;
			}
			break;
		case PROPERTY_ORIENTATION:
			g_lcd.orientation = b[9];
			lcd_orientation(g_lcd.orientation);
			g_refresh = 1;
			break;
		case PROPERTY_FGCOLOR:
			g_term.col = b[9] | (b[10] << 8);
			break;
		case PROPERTY_BGCOLOR:
			g_term.bgcol = b[9] | (b[10] << 8);
			break;
	}
	return 0;
}

RETCODE handle_getprop(__idata BYTE *b) __banked
{
	// Not yet. later.
	return 0;
}

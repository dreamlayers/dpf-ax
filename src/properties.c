/* Rudimentary Get/Set property handler */

#pragma codeseg INIT

#include "dpf.h"
#include "utils.h"
#include "global.h"
#include "usbuser.h"

RETCODE handle_setprop(__idata BYTE *b) __banked
{
	unsigned char brightness;
	switch (b[7]) {
		case PROPERTY_BRIGHTNESS:
			// for backward compatibility:
			// "old" brightness was 0..7, new is 0..21
			brightness = b[9] * 3;
			if (brightness >= MAX_BRIGHTNESS_VALUE)
				brightness = MAX_BRIGHTNESS_VALUE;
			else if (brightness) brightness -= 2;
			g_lcd.brightness = brightness;
			if (g_lcd.brightness == 0) {
				lcd_backlight_off();
			} else {
				lcd_backlight_on();
			}
			break;
		case PROPERTY_ORIENTATION:
			g_lcd.orientation = b[9];
			lcd_orientation(g_lcd.orientation);
			g_refresh = 1;
			break;
		case PROPERTY_FGCOLOR:
			g_fgcol = b[9] | (b[10] << 8);
			break;
		case PROPERTY_BGCOLOR:
			g_bgcol = b[9] | (b[10] << 8);
			break;
	}
	return 0;
}

RETCODE handle_getprop(__idata BYTE *b) __banked
{
	// Not yet. later.
	b;
	return 0;
}

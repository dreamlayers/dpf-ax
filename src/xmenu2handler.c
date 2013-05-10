/**
 * Menu definition
 */

#pragma codeseg XMENU2
#pragma constseg XMENU2

#include "dpf.h"
#include "print.h"
#include "global.h"
#include "xmenu.h"
#include "utils.h"

char hdl_brightness(BYTE evt) __reentrant
{
	if (g_lcd.brightness < 1) g_lcd.brightness = LCD_MAX_BRIGHTNESS_VALUE;
	else if (g_lcd.brightness > LCD_MAX_BRIGHTNESS_VALUE) g_lcd.brightness = 1;
	set_brightness(g_lcd.brightness);

	g_config.brightness = g_lcd.brightness;
	if (evt == KEY_MENU)
		save_config2flash();
	return S_EDIT;
}


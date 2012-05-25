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
	evt;
	if (g_lcd.brightness < 1) g_lcd.brightness = 1;
	if (g_lcd.brightness > PWM_PERIOD) g_lcd.brightness = PWM_PERIOD;
	g_config.brightness = g_lcd.brightness;

	tmr1cntl = tmr1cnth = 0;
	tmr1perl = PWM_PERIOD; tmr1perh = 0x00;
	tmr1pwml = (~g_lcd.brightness & 0x7);
	tmr1pwmh = 0;
	if (evt == KEY_MENU)
		save_config2flash();
	return S_EDIT;
}


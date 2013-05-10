/**
 * Menu definition
 */

#pragma codeseg XMENU7
#pragma constseg XMENU7

#include "dpf.h"
#include "print.h"
#include "global.h"
#include "xmenu.h"
#include "utils.h"

char hdl_contrast(BYTE evt) __reentrant
{
	evt;
#if !defined(LCD_CONTRAST_NONE)
	if (g_config.contrast < 1) g_config.contrast = get_maxcontrast();
	else if (g_config.contrast > get_maxcontrast()) g_config.contrast = 1;
	set_contrast(g_config.contrast);
	if (evt == KEY_MENU)
		save_config2flash();
#endif	
	return S_EDIT;
}


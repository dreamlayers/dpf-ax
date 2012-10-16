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
	set_contrast(g_config.contrast);
	if (evt == KEY_MENU)
		save_config2flash();
	return S_EDIT;
}


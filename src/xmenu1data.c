/**
 * Menu definition
 */

#pragma constseg XMENU1

#include "dpf.h"
#include "print.h"
#include "global.h"
#include "xmenu.h"


extern char hdl_config(BYTE evt) __reentrant;


const MENUAREA XMenu g_splashmenu = {
    "Splash Menu", 0,
    sizeof(items) / sizeof(struct menuitem),
    items
};

const MENUAREA MItem items[] = {
	{ "Black", M_IDATA_BYTE            , SPLASH_BLACK, &g_config.splash, 0, 0 },
	{ "White", M_IDATA_BYTE            , SPLASH_WHITE, &g_config.splash, 0, 0 },
	{ "Image", M_IDATA_BYTE            , SPLASH_IMAGE, &g_config.splash, 0, 0 },
	{ "Back" , M_HANDLER | F_MENUSWITCH, MENU_SETUP  , 0               , hdl_config, 0 },
};  



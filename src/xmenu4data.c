/**
 * Menu 0 (main menu) definition
 */

#pragma constseg XMENU4

#include "dpf.h"
#include "print.h"
#include "global.h"
#include "xmenu.h"


extern char hdl_off(BYTE evt) __reentrant;
extern char hdl_brightness(BYTE evt) __reentrant;
extern char hdl_reboot(BYTE evt) __reentrant;

#if NUM_COLS_LARGE > 14
#define STR_SPLASH "Splashscreen"
#else
#define STR_SPLASH "Splash"
#endif


const MENUAREA XMenu g_setupmenu = {
    "Setup Menu", 0,
    sizeof(items) / sizeof(struct menuitem),
    items
};

const MENUAREA MItem items[] = {
    { STR_SPLASH   , F_MENUSWITCH, MENU_SPLASH   , 0, 0, 0 },
    { "Backlight"  , F_MENUSWITCH, MENU_BACKLIGHT, 0, 0, 0  },
    { "Back"       , F_MENUSWITCH, MENU_MAIN     , 0, 0, 0  },
};  


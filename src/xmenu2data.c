/**
 * Menu definition
 */

#pragma constseg XMENU2

#include "dpf.h"
#include "print.h"
#include "global.h"
#include "xmenu.h"

extern char hdl_brightness(BYTE evt) __reentrant;

#if NUM_COLS_LARGE > 14
#define STR_BL "Set Backlight"
#else
#define STR_BL "Backlight"
#endif

const MENUAREA XMenu g_backlightmenu = {
    STR_BL, 3,
    sizeof(items) / sizeof(struct menuitem),
    items
};

const MENUAREA MItem items[] = {
    { "", M_DIRECT_BYTE | F_MENUSWITCH, MENU_SETUP, &g_lcd.brightness, hdl_brightness, 0 },
};  


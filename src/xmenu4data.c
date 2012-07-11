/**
 * Menu 0 (main menu) definition
 */

#pragma constseg XMENU4

#include "dpf.h"
#include "print.h"
#include "global.h"
#include "xmenu.h"


#if NUM_COLS_LARGE > 14
#define STR_SPLASH "Splashscreen"
#define STR_SERIAL "USB Serial Number"
#define STR_TEST   "Screen Testpattern"
#else
#define STR_SPLASH "Splash"
#define STR_SERIAL "USB Serial#"
#define STR_TEST   "Testscreen"
#endif


const MENUAREA XMenu g_setupmenu = {
    "Setup Menu", 0,
    sizeof(items) / sizeof(struct menuitem),
    items
};

const MENUAREA MItem items[] = {
    { STR_SPLASH   , F_MENUSWITCH, MENU_SPLASH    , 0, 0, 0 },
    { "Backlight"  , F_MENUSWITCH, MENU_BACKLIGHT , 0, 0, 0  },
    { STR_SERIAL   , F_MENUSWITCH, MENU_USBSERIAL , 0, 0, 0  },
    { STR_TEST     , F_MENUSWITCH, MENU_TESTSCREEN, 0, 0, 0  },
    { "Back"       , F_MENUSWITCH, MENU_MAIN      , 0, 0, 0  },
};  


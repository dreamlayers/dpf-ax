/**
 * Menu definition
 */

#pragma constseg XMENU7

#include "dpf.h"
#include "print.h"
#include "global.h"
#include "xmenu.h"
#include "xmenuhook.h"

extern char hdl_contrast(BYTE evt) __reentrant;

#if NUM_COLS_LARGE > 14
#define STR_BL "Set Contrast"
#else
#define STR_BL "Contrast"
#endif

const MENUAREA XMenu g_contrastmenu = {
    STR_BL, 3,
    sizeof(items) / sizeof(struct menuitem),
    items
};

const MENUAREA MItem items[] = {
    { "", M_DIRECT_BYTE | F_MENUSWITCH, MENU_SETUP, &g_config.contrast, hdl_contrast, 0 }, //XHOOK_TESTPATTERN },
};  


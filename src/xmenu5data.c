/**
 * Menu definition
 */

#pragma constseg XMENU5

#include "dpf.h"
#include "print.h"
#include "global.h"
#include "xmenu.h"
#include "xmenuhook.h"

extern char hdl_usbserial(BYTE evt) __reentrant;

#if NUM_COLS_LARGE > 14
#define STR_SL "Set USB Serial Number"
#else
#define STR_SL "USB Serial#"
#endif

const MENUAREA XMenu g_usbserialmenu = {
    STR_SL, 3,
    sizeof(items) / sizeof(struct menuitem),
    items
};

const MENUAREA MItem items[] = {
    { "", M_DIRECT_BYTE | F_MENUSWITCH, MENU_SETUP, &g_config.usbserial, hdl_usbserial, XSTR_REBOOT },
};  


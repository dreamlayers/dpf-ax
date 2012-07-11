/**
 * Menu definition
 */

#pragma constseg XMENU6

#include "dpf.h"
#include "print.h"
#include "global.h"
#include "xmenu.h"
#include "xmenuhook.h"


const MENUAREA XMenu g_testscreenmenu = {
    "", 0,
    sizeof(items) / sizeof(struct menuitem),
    items
};

const MENUAREA MItem items[] = {
	{ " "    , F_MENUSWITCH, MENU_SETUP, 0, 0, XHOOK_TESTPATTERN},
};  


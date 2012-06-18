/**
 * Menu definition
 */

#pragma constseg XMENU3

#include "dpf.h"
#include "print.h"
#include "global.h"
#include "xmenu.h"
#include "xmenustr.h"


const MENUAREA XMenu g_infomenu = {
    "USB Display", 0,
    sizeof(items) / sizeof(struct menuitem),
    items
};

const MENUAREA MItem items[] = {
#ifdef DEBUG_DEV
	{ "Debug"   , F_MENUSWITCH, MENU_MAIN, 0, 0, XSTR_DEBUG},
#endif	
	{ "Version" , F_MENUSWITCH, MENU_MAIN, 0, 0, XSTR_VERSION},
	{ "Credits" , F_MENUSWITCH, MENU_MAIN, 0, 0, XSTR_CREDITS},
#if NUM_LINES_LARGE > 9
	{ "Homepage" , F_MENUSWITCH, MENU_MAIN, 0, 0, XSTR_WWW},
	{ "LCD"     , F_MENUSWITCH, MENU_MAIN, 0, 0, XSTR_LCD },
#endif
	{ "Flash"   , F_MENUSWITCH, MENU_MAIN, 0, 0, XSTR_FLASH},
	{ "Back"    , F_MENUSWITCH, MENU_MAIN, 0, 0, 0},
};  


/**
 * Menu 0 - main menu definition
 */

#pragma constseg XMENU0

#include "dpf.h"
#include "print.h"
#include "global.h"
#include "xmenu.h"


extern char hdl_off(BYTE evt) __reentrant;
extern char hdl_reboot(BYTE evt) __reentrant;


const MENUAREA XMenu g_mainmenu = {
    "Main Menu", 0,
    sizeof(items) / sizeof(struct menuitem),
    items
};

const MENUAREA MItem items[] = {
    { "Info"       , F_MENUSWITCH, MENU_INFO     , 0, 0, 0 },
    { "Setup"      , F_MENUSWITCH, MENU_SETUP    , 0, 0, 0  },
    { "Off"        , M_HANDLER   , 0             , 0, hdl_off, 0 },
    { "Reboot"     , M_HANDLER   , 0             , 0, hdl_reboot, 0 },
    { "Exit"       , M_EXIT      , 0             , 0, 0, 0 }
};  


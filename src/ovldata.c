/** Simple experimental support for overlay data
 * Currently a stub.
 *
 * Normally, data that is shared among modules (strings, etc) go in here.
 * The reason is, that banked call switches destroy code memory.
 * Thus, we need to take care of the overlaying ourselves.
 */

#pragma constseg DATA0

#include "dpf.h"

// For later.
// #ifndef OVLDATA
// #error "You must define OVLDATA for the overlay data area"
// #endif

#include "global.h"

extern char hdl_off(BYTE evt) __reentrant;
extern char hdl_brightness(BYTE evt) __reentrant;
extern char hdl_debug(BYTE evt) __reentrant;
extern char hdl_monitor(BYTE evt) __reentrant;
extern char lcd4linux(BYTE evt) __reentrant;
extern char hdl_reboot(BYTE evt) __reentrant;
extern char hdl_flix(BYTE evt) __reentrant;


const OVLAREA
Menu g_menu[] = {
#ifdef FLIX_MODE
	{ " Flix ", { 0, L_MENU }, M_DIRECT_BYTE | F_ACTION,
		&g_flix.mode, hdl_flix  }
#else
	// First entry is default:
	{ " Exit ", { 1, L_MENU + 1 }, M_EXIT, 0, 0 },
	{ " Off ", { 7, L_MENU + 1 }, M_HANDLER, 0, hdl_off },
	{ " Reboot ", { 12, L_MENU + 1 }, M_HANDLER, 0, hdl_reboot },
	{ " Debug ", { 0, L_MENU }, M_SHOW | F_ACTION, 0, hdl_debug },
	{ " Mon ", { 7, L_MENU }, M_SHOW | F_ACTION, 0, hdl_monitor  },
	{ " Backlight ", { 12, L_MENU }, M_DIRECT_BYTE | F_ACTION,
		&g_lcd.brightness, hdl_brightness  },
#ifdef HAVE_FLIX
	{ " Flix ", { 23, L_MENU }, M_DIRECT_BYTE,
		&g_flix.mode, hdl_flix  },
#endif
#endif
};

const OVLAREA
char g_fakeleddesc[] = "USB PWR EP0 STL LOG ERR ACK ALV";

const OVLAREA char g_rebootmsg[] = "Rebooting...";

#define NUM_MENU sizeof(g_menu) / sizeof(struct menu)

const OVLAREA char g_menusize = NUM_MENU;


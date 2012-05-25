/**
 * Menu string definitions
 */

#pragma codeseg XMENUSTR
#pragma constseg XMENUSTR

#include "dpf.h"
#include "utils.h"
#include "print.h"
#include "global.h"
#include "xmenu.h"
#include "xmenustr.h"

#define _RESOLVE(x) #x
#define STRING(x) _RESOLVE(x)

static __code const char * __code xmenu_strings[] = {
/* -- XSTR_NULL -- */
	"", 0,

/* -- XSTR_FLASH - dynamic! -- */
	0, 0,

/* -- XSTR_LCD -- */
#ifdef LCD_CONTROLLER_CUSTOM
	"Unknown (custom)", 0,
#else
	STRING(_INTERNAL_TAG) " (compatible)", 0,
#endif

/* -- XSTR_CREDITS -- */
#if NUM_COLS_LARGE > 16
	" By hackfin@section5.ch", "     and superelchi",
#else
	"By hackfin &", "    superelchi",
#endif

/* -- XSTR_VERSION -- */
	STRING(BUILD_ID), 0,

/* -- XSTR_WWW -- */
	"http://dpf-ax.sourceforge.net", 0,

/* -- XSTR_REBOOT -- */
#if NUM_COLS_LARGE > 16
	"Changes will be applied", " on next reset/reboot",
#else
	"Reboot/reset", "to apply",
#endif
};

#define NUM_XSTRINGS ((sizeof(xmenu_strings) / sizeof(__code char *)) / 2)

void put_xstring(unsigned char id) __banked
{
	if (id < NUM_XSTRINGS)
	{
		if (id == XSTR_FLASH)
		{
			flash_detect();
			flash_print_size();
		}
		else
		{
			id *= 2;
			puts(xmenu_strings[id]);
			if (xmenu_strings[id+1])
			{
				puts("\n");
				puts(xmenu_strings[id+1]);
			}
		}
	}
}




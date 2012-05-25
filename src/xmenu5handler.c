/**
 * Menu definition
 */

#pragma codeseg XMENU5
#pragma constseg XMENU5

#include "dpf.h"
#include "print.h"
#include "global.h"
#include "xmenu.h"
#include "utils.h"

char hdl_usbserial(BYTE evt) __reentrant
{
	// iSerial String is in code memory, so we have to do write-access via xdata
        //__xdata unsigned char *p = ((__xdata unsigned char *) &g_usbserial[4]) - 0x800;
	evt;
	if (g_config.usbserial < 1) g_config.usbserial = 1;
	if (g_config.usbserial > 99) g_config.usbserial = 99;
	// Delay iSerial change until next reset/reboot
	//*p = (g_config.usbserial / 10) + '0';
	//*(p+2) = (g_config.usbserial % 10) + '0';
	if (evt == KEY_MENU)
		save_config2flash();
	return S_EDIT;
}


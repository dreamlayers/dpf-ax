#pragma codeseg USBAUX

#include "dpf.h"
#include "usb.h"
#include "utils.h"
#include "global.h"

static
void ep1in_commit(BYTE which)
{
	usb_select(R_USBIndexed_InCSR1);
	usb_putc(which);
}

static
RETCODE usb_wait_txe(void)
{
	do {
		if (!g_usb_active) return -1;
		usb_select(R_USBIndexed_InCSR1); // TX status
		usbcon |= USBKS | USBRNW;  // Read & kick on
		while (!_B_USBDONE);
	} while (usb_flag_set(INPRDY));
	return 0;
}

RETCODE mem_read(BYTE __idata *b) __banked
{
	__code BYTE *addr;
	unsigned short len;
	BYTE n;

	addr = (__code BYTE *) get_short(&b[7]);
	len = get_short(&b[9]);

	if (!len) return 0;

	while (len > 64) {
		usb_wait_txe();
		usb_select(R_USB_FIFO1);
		n = 64;
		do {
			usb_putc(*addr++);
		} while (--n);
		ep1in_commit(INPRDY); // Send packet

		len -= 64;
	} 

	usb_wait_txe();
	usb_select(R_USB_FIFO1);
	n = len;
	do {
		usb_putc(*addr++);
	} while (--n);
	ep1in_commit(INPRDY); // Send packet

	return 0;
}



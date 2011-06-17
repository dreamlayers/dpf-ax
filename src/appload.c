/* Application loader. Supports loading of larger stuff */
#include "dpf.h"
#include "utils.h"
#include "global.h"
#include "usb.h"

#pragma codeseg APPLD

static
void usb_ack_rx(void)
{
	usbadr = R_USBIndexed_OutCSR1;
	usb_putc(0);
}

static
RETCODE usb_wait_rxf(void)
{
	do {
		if (!g_usb_active) return -1;
		usbadr = R_USBIndexed_OutCSR1; // TX status
		usbcon |= USBKS | USBRNW;  // Read & kick on
		while (!_B_USBDONE);
	} while (!(usbdata & OPRDY));
	return 0;
}

// #define get_short(x)  ((x)[0]) | ((x[1]) << 8)


void *app_load(BYTE __idata *b) __banked
{
    __xdata BYTE *d;
	BYTE n;
	unsigned short len;
	unsigned short l;

    b = &b[7];

    d = (__xdata BYTE *) get_short(b); b+=2;
    d -= 0x0800;  // Translate into XDATA address
    len = get_short(b); b += 2;

	_B_EA = 0; // interrupts off

	l = len >> 6; // div by 64
	while (l--) {
		n = EP1_PKTSIZE;
		usb_wait_rxf();  // Wait until packet ready
		usbadr = R_USB_FIFO1;
		do {
			*d++ = usb_readbyte();
		} while (--n);
		usb_ack_rx();
	}
	n = len & 0x3f; // remainder
	if (n) {
		usb_wait_rxf();  // Wait until packet ready
		usbadr = R_USB_FIFO1;
		do {
			*d++ = usb_readbyte();
		} while (--n);
		usb_ack_rx();
	}
    l = get_short(b); // Get start address

	_B_EA = 1; // interrupts on

	g_retcode = (unsigned short) execute(l); // And jump into code
	return 0;
}



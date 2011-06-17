#include "ax206.h"
#include "usb.h"
#include "utils.h"
// Debugging only:
#include "global.h"

#pragma codeseg HOME0

volatile
__bit g_usb_event;     // USB event

void usb_isr(void) __interrupt(3) __using(1)
{
	usbadr = R_USB_IntrUSB;
	usb_readbyte();

	// usbadr = R_USB_IntrIn1;
	// g_register[1] = usb_readbyte();

	// usbadr = R_USB_IntrOut1;
	// g_register[2] = usb_readbyte();

	g_usb_event = 1;
}

BYTE usb_readbyte(void)
{
	usbcon |= (USBRNW | USBKS);
	while (!_B_USBDONE);
	return usbdata;
}

void usb_putc(BYTE c)
{
	usbdata = c;
	usbcon &= ~USBRNW;
	usbcon |= USBKS; // Kick start
	while (!_B_USBDONE);
}

/* Printf extra functions */
#pragma codeseg PRINTFX
#pragma constseg PRINTFX

// WARNING: If you move function out of this module, make sure you
// use the banked version of the subroutines for output

#include "ax206.h"
#include "global.h"
#include "dpf.h"
#include "utils.h"
#include "print.h"
#include "usb.h" // readbyte()

static
BYTE strlen(__code char *str)
{
	BYTE n = 0;
	while (*str++) n++;
	return n;
}

/* Note: Can not be banked */

void print_padded(__code char *b)
{
	char c;
	char n = *b++;
	char p = *b++;
	char i;

	if (n == CODE_CENTER) {
		n = p; p = *b++;
		i = (n - strlen(b)) >> 1;
		n -= i;
		i--;
		while (i--) {
			rawputc(p);
		};
	}

	while ( (c = *b++) && n--) {
		rawputc(c);
	}
	while (n-- > 0) {
		rawputc(p);
	};
}

#define _RESOLVE(x) #x
#define STRING(x) _RESOLVE(x)

#if NUM_COLS >= 60
#	define PAD_WIDTH "\071"
#else
#	define PAD_WIDTH "\035"
#endif

void print_splash(void) __banked
{
	__idata char codes[3];
	disp_home();
	rawputc(0x11); print_padded(PAD_WIDTH "\022"); rawputc(0x05); putsc("\n|");
	print_padded(PAD_WIDTH " USB display"); putsc("|\n|");
	print_padded(PAD_WIDTH "  version:" STRING(BUILD_ID)); putsc("|\n|");
	print_padded(PAD_WIDTH "    by hackfin@section5.ch"); putsc("|\n|");
	print_padded(PAD_WIDTH "       and superelchi"); puts("|\n");
	rawputc(0x1a); print_padded(PAD_WIDTH "\022"); rawputc(0x03); putsc("\n\n");
#ifndef LCD_CONTROLLER_CUSTOM
	putsc("LCD: " STRING(_INTERNAL_TAG) " (compatible)\n");
#endif
	putsc("Detecting flash...");
	flash_detect(codes);
	putsc("\n");
	flash_print_size(codes);
	putsc("\n\n\nPress MENU for config mode");
}

void notice_powerdown(void) __banked
{
	GOTOXY(1, 7);
	print_padded(STR_CODE_CENTER PAD_WIDTH " Weak power!");
	GOTOXY(1, 8);
	print_padded(STR_CODE_CENTER PAD_WIDTH " Turning off.");
}

void print_status(void) __banked
{
	BYTE ox, oy;
	BYTE i;

	ox = 1; oy = 7;
	GOTOXY(ox, oy);
	putsc("ctr: ");
	for (i = 0; i < sizeof(g_count) / sizeof(g_count[0]); i++) {
		putc(' '); out_short(g_count[i]);
	}

	oy++;
	GOTOXY(ox, oy);
	putsc("reg: ");
	for (i = 0; i < sizeof(g_register) / sizeof(g_register[0]); i++) {
		putc(' '); out_short(g_register[i]);
	}

	oy += 2;
	GOTOXY(ox, oy); putsc("adc: "); out_short(g_adc >> 6);
	GOTOXY(16, oy); putsc("retcode: "); out_short(g_retcode);
}

#if 0
void print_usbstat(void) __banked
{
#if 0
	BYTE c;
	_B_USBIE = 0; // Disable USB interrupt while accessing USB regs
	putsc("USBC - ");
	usb_select(R_USB_IntrIn1E); c = usb_readbyte();
	putsc("In1E: "); out_short(c);
	usb_select(R_USB_IntrOut1E); c = usb_readbyte();
	putsc(" Out1E:"); out_short(c);
	_B_USBIE = 1;
#endif
	putsc("\n usbcon:"); out_short(usbcon);
}
#endif

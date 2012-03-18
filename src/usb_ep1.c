/* EP1 IN/OUT handling for C8051 SmallScale / Flix
 *
 * (c) 2010 Martin Strubel <strubel-at-section5.ch>
 *
 * AX206 mods by fulfisk
 *
 */

#include "dpf.h"
#include "usb.h"
#include "usbuser.h"
#include "global.h"
#include "spiflash.h"

#include "utils.h"

#pragma codeseg UHANDLER

static bit g_flashlock = 0;

__idata BYTE ep1buf[EP1_PKTSIZE];

/* Somewhat hacky function. Wants pointer to 4 byte length array */
void usb_to_screen(__idata unsigned char *b);

// Macro to retrieve data from SCSI cmd offset
#define GET_BUF(i) ep1buf[i + 15]

static
void ep1in_commit(BYTE which)
{
	usb_select(R_USBIndexed_InCSR1);
	usb_putc(which);
}

static
void ep1out_commit(BYTE which)
{
	usb_select(R_USBIndexed_OutCSR1);
	usb_putc(which);
}

#define usb_ack_rx() ep1out_commit(0);

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

static
RETCODE usb_wait_rxf(void)
{
	do {
		usb_select(R_USBIndexed_OutCSR1); // TX status
		usbcon |= USBKS | USBRNW;  // Read & kick on
		while (!_B_USBDONE);
		if (!g_usb_active) return -1;
	} while (!usb_flag_set(OPRDY));
	return 0;
}

RETCODE usb_putibuf(BYTE __idata *buf, BYTE n)
{
	while (n--) {
		usb_putc(*buf++);
	}
	return 0;
}

static
void usb_nak(void)
{
	usb_select(R_USBIndexed_InCSR1);
	usb_putc(INSDSTL);        // Send stall
	usb_select(R_USBIndexed_OutCSR1);
	usb_putc(OSDSTL);        // Send stall
}

RETCODE send_ack(BYTE retcode, BYTE __idata *buf)
{
	int ret;
	ret = usb_wait_txe();
	usb_select(R_USB_FIFO1);
	buf[3] = 'S';
	usb_putibuf(&buf[0], 4);
	usb_putibuf(&buf[4], 4);
	usb_putibuf(&buf[31], 4);
	usb_putc(retcode);
	ep1in_commit(INPRDY); // Send packet
	return ret;
}

static void spi2usb(BYTE n)
{
	usb_wait_txe();
	usb_select(R_USB_FIFO1);
	do {
		usb_putc(spibuf); spibuf = 0xff; while(spi_busy());
	} while (--n);
	ep1in_commit(INPRDY); // Send packet
}

RETCODE usb_flash_read(void)
{
	BYTE n;
	unsigned long len;
	BYTE __idata *c;
	// Length to read:
	len = get_int3i(&GET_BUF(7));
	
	spi_select();

	n = GET_BUF(6);   // Len of command sequence
	c = &GET_BUF(10); // Start command sequence
	do {
		spibuf = *c++; while (spi_busy());
	} while (--n);
	spibuf = 0; while (spi_busy()); // Dummy write

	while (len > 64) {
		spi2usb(64);
		len -= 64;
	} 

	spi2usb(len);

	spi_deselect();
	return 0;
}

RETCODE user_handler(void)
{
	unsigned long len;
	__idata BYTE *b;

	b = &GET_BUF(0);

	switch (b[6]) {
		case USBCMD_GETPROPERTY:
			return handle_getprop(b);
		case USBCMD_SETPROPERTY:
			return handle_setprop(b);
		case USBCMD_FLASHLOCK:
			g_flashlock = b[7];
			break;
#ifdef EXPERIMENTAL
		case USBCMD_FILLRECT: // Unsupported on most controllers
			fill_rect(b);
			break;
		case USBCMD_COPYRECT: // Unsupported on most controllers
			copy_rect(b);
			break;
#endif
		case USBCMD_BLIT:
			g_blit.x0 = b[7] | (b[8] << 8);
			g_blit.y0 = b[9] | (b[10] << 8);
			g_blit.x1 = b[11] | (b[12] << 8);
			g_blit.y1 = b[13] | (b[14] << 8);
			disp_blit();
			usb_to_screen(&ep1buf[8]);
			break;
		case USBCMD_MEMREAD:
			return mem_read(b);
		case USBCMD_APPLOAD:
			len = (unsigned long) app_load(b);
			return (RETCODE) len & 0xff;
		case USBCMD_PROBE:
			return 0x5a;
		default:
			return 0;
	}
	return 1;
}

static
void spi_pollstatus(void)
{
	BYTE sr;

	do {
		spi_select();
		spibuf = SPM_RDSR; while (spi_busy());
		spibuf = 0xff; while (spi_busy());
		sr = spibuf;
		spi_deselect();
	} while (sr & SPS_WIP);
}

static
void spi_writecmd(BYTE cmd)
{
	spi_select();
	spibuf = cmd;
	while (spi_busy());
	spi_deselect();
}

// FIXME: Implement page programming

void writeflash_fromusb(unsigned long addr)
{
	spi_writecmd(SPM_WREN); // Write enable
	spi_select();
	spibuf = SPM_PP; while (spi_busy());
	spibuf = addr >> 16; while (spi_busy());
	spibuf = addr >> 8; while (spi_busy());
	spibuf = addr; while (spi_busy());
	spibuf = usb_readbyte(); while (spi_busy());
	spi_deselect();
	spi_pollstatus();
}


/* This function must - under all circumstances - remain in this module.
 * Otherwise, the in-system flash upgrade will terribly go wrong.
 */

RETCODE flash_sectorcmd(void)
{
	BYTE n;
	unsigned long end, addr;
	BYTE __idata *c;

	spi_select();

	end = get_int3i(&GET_BUF(7));

	// Do we have a simple command?
	if (!end) {
		n = GET_BUF(6); // command sequence length
		c = &GET_BUF(10); // commands begin here
		do {
			spibuf = *c++;
			while (spi_busy());
		} while (--n);
	// We program.
	} else {
		addr = get_int3i(&GET_BUF(11));
		end += addr; // end address
		// Note: Data length must be multiple of 64 bytes
		do {
			n = EP1_PKTSIZE;
			usb_wait_rxf();  // Wait until data ready
			usb_select(R_USB_FIFO1);
			do {
				writeflash_fromusb(addr);
				addr++;
				if (addr == end) break;
			} while (--n);
			usb_ack_rx();
			RUB_WATCHDOG();
		} while (addr < end);
	}
	spi_deselect();
	spi_pollstatus();
	return 0;
}

char get_lcdprops(void)
{
	usb_wait_txe();
	usb_select(R_USB_FIFO1);
	usb_putc(RESOL_X);
	usb_putc((unsigned short) RESOL_X >> 8);
	usb_putc(RESOL_Y);
	usb_putc((unsigned short) RESOL_Y >> 8);
	usb_putc(0xff);
	ep1in_commit(INPRDY); // Send packet
	return 0;
}

RETCODE handle_cmd_cd(void)
{
	RETCODE ret = 0;
	switch (GET_BUF(5)) {
		case 0:
			// Handler for probe and read functions
			ret = usb_flash_read();
			break;
		case 1:
			break;
		case 2: // Get LCD parameters:
			ret = get_lcdprops();
			break;
		case 3: // Legacy Probe
			ret = PROTOCOL_VERSION; // Protocol version
			break;
		case 4: break;
		case 5:
			break;
		case 6:
			ret = user_handler();
			break;
		default:
			return -1;
	}
	return ret;
}

RETCODE ep1_dispatch(void)
{
	BYTE n;
	RETCODE ret = -1;
	BYTE __idata *b = ep1buf;


	usb_select(R_USBIndexed_OutCount1);
	n = usb_readbyte();

	if (n == 31) {
		usb_select(R_USB_FIFO1);

		while (n--) {
			*b++ = usb_readbyte();
		}
		if (ep1buf[0] == 'U' &&
		    ep1buf[1] == 'S' &&
		    ep1buf[2] == 'B' &&
		    ep1buf[3] == 'C') {
				usb_ack_rx();
				switch (ep1buf[15]) {
					case 0xcb:
						ret = flash_sectorcmd();
						break;
					case 0xcd:
						ret = handle_cmd_cd();
						break;
					default:
						ret = -1;
				}
			if (ret >= 0) {
				g_fakeled &= ~LED_ERR;
				g_fakeled |= LED_ACK;
				send_ack(ret, ep1buf);
				if (g_flashlock) ret = USB_IN_SEQUENCE;
				return ret;
			}
		}
	}
	usb_ack_rx();
	g_fakeled &= ~LED_ACK;
	g_fakeled |= LED_ERR;
	usb_nak();     // Send soft NAK
	return ret;
}

RETCODE handler_usb_ep1(void) __banked
{
	RETCODE ret;
	ret = ep1_dispatch();
	// If we get a USB_IN_SEQUENCE retcode, we stay in this loop.
	// This stops bank switching and allows in-system programming
	while (ret == USB_IN_SEQUENCE) {
		RUB_WATCHDOG();
		usb_select(R_USBIndexed_OutCSR1);
		if (usb_readbyte() & OPRDY) {
			ret = ep1_dispatch();
		}
		// FIXME: Need timeout condition
	}
	return ret;
}


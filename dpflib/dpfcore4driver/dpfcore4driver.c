/* Core DPF access library for AX206 based HW.
 *
 * Includes only code necessary to access a hacked frame.
 * For the full library see http://dpf-ax.sourceforge.net/.
 *
 * 05/2013 superelchi <superelchi AT wolke7.net>
 *
 */
	
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "dpfcore4driver.h"

/*
 ********************************************************************
 * 
 * Include this file in your drivers distribution
 * or cut & paste the following code at the end of your driver.
 *
 ********************************************************************
 */

//###################################################################
// Start dpfcore4driver.c
// See http://dpf-ax.sourceforge.net/
//###################################################################

#include <usb.h>

#define AX206_VID 0x1908		// Hacked frames USB Vendor ID
#define AX206_PID 0x0102		// Hacked frames USB Product ID

#define USBCMD_SETPROPERTY  0x01	// USB command: Set property
#define USBCMD_BLIT         0x12    	// USB command: Blit to screen

/* Generic SCSI device stuff */

#define DIR_IN  0
#define DIR_OUT 1

/* The DPF context structure */
typedef
struct dpf_context {
	usb_dev_handle *udev;
	unsigned int width;
	unsigned int height;
} DPFContext;

static int wrap_scsi(DPFContext *h, unsigned char *cmd, int cmdlen, char out,
	unsigned char *data, unsigned long block_len);

/**
 * Open DPF device.
 * 
 * Device must be string in the form "usbX" or "dpfX", with X = 0 .. number of connected dpfs.
 * The open function will scan the USB bus and return a handle to access dpf #X.
 * If dpf #X is not found, returns NULL.
 *
 * \param dev	device name to open
 * \return		device handle or NULL
 */
DPFAXHANDLE dpf_ax_open(const char *dev)
{
	DPFContext *dpf;
	int index = -1;
	usb_dev_handle *u;

	if (dev && strlen(dev) == 4 && (strncmp(dev, "usb", 3) == 0 || strncmp(dev, "dpf", 3) == 0))
		index = dev[3] - '0';
		
	if (index < 0 || index > 9) {
		fprintf(stderr, "dpf_ax_open: wrong device '%s'. Please specify a string like 'usb0'\n", dev);
		return NULL;
	}

	usb_init();
	usb_find_busses();
	usb_find_devices();

	struct usb_bus *b = usb_get_busses();
	struct usb_device *d = NULL;
	int enumeration = 0;
	int found = 0;

	while (b && !found) {
		d = b->devices;
		while (d) {
			if ((d->descriptor.idVendor == AX206_VID) && (d->descriptor.idProduct == AX206_PID)) { 
				fprintf(stderr, "dpf_ax_open: found AX206 #%d\n", enumeration+1);
				if (enumeration == index) {
					found = 1;
					break;
				}
				else enumeration++;
			}
			d = d->next;
		}
		b = b->next;
	}

	if (!d) {
		fprintf(stderr,"dpf_ax_open: no matching USB device '%s' found!\n", dev);
		return NULL;
	}

	dpf = (DPFContext *) malloc(sizeof(DPFContext));
	if (!dpf) {
		fprintf(stderr, "dpf_ax_open: error allocation memory.\n");
		return NULL;
	}

	u = usb_open(d);
	if (u == NULL) {
		fprintf(stderr,"dpf_ax_open: failed to open usb device '%s'!\n", dev);
		free(dpf);
		return NULL;
	}

	if (usb_claim_interface(u, 0) < 0) {
		fprintf(stderr,"dpf_ax_open: failed to claim usb device!\n");
		usb_close(u);
		free(dpf);
		return NULL;
	}

	dpf->udev = u;

	static unsigned char buf[5];
	static unsigned char cmd[16] = {
		0xcd, 0, 0, 0,
		   0, 2, 0, 0,
		   0, 0, 0, 0,
		   0, 0, 0, 0
	};
	cmd[5] = 2; // get LCD parameters
	if (wrap_scsi(dpf, cmd, sizeof(cmd), DIR_IN, buf, 5) == 0) {
		dpf->width = (buf[0]) | (buf[1] << 8);
		dpf->height = (buf[2]) | (buf[3] << 8);
		fprintf(stderr, "dpf_ax_open: got LCD dimensions: %dx%d\n", dpf->width, dpf->height);
	}
	else {
		fprintf(stderr, "dpf_ax_open: error reading LCD dimensions!\n");
		dpf_ax_close(dpf);
		return NULL;
	}
	return (DPFAXHANDLE) dpf;
}

/**
 *  Close DPF device
 */

void dpf_ax_close(DPFAXHANDLE h)
{
	DPFContext *dpf = (DPFContext *) h;
	
	usb_release_interface(dpf->udev, 0);
	usb_close(dpf->udev);
	free(dpf);
}

/** Get screen width.
 * 
 * \return width in pixel
 */
int dpf_ax_getwidth(DPFAXHANDLE h)
{
	return ((DPFContext *) h)->width;
}

/** Get screen height.
 * 
 * \return height in pixel
 */
int dpf_ax_getheight(DPFAXHANDLE h)
{
	return ((DPFContext *) h)->height;
}

static
unsigned char g_excmd[16] = {
	0xcd, 0, 0, 0,
	   0, 6, 0, 0,
	   0, 0, 0, 0,
	   0, 0, 0, 0
};

/** Blit data to screen.
 *
 * \param buf     buffer to 16 bpp RGB 565 image data
 * \param rect    rectangle tuple: [x0, y0, x1, y1]
 */
void dpf_ax_screen_blit(DPFAXHANDLE h, const unsigned char *buf, short rect[4])
{
	unsigned long len = (rect[2] - rect[0]) * (rect[3] - rect[1]);
	len <<= 1;
	unsigned char *cmd = g_excmd;

	cmd[6] = USBCMD_BLIT;
	cmd[7] = rect[0];
	cmd[8] = rect[0] >> 8;
	cmd[9] = rect[1];
	cmd[10] = rect[1] >> 8;
	cmd[11] = rect[2] - 1;
	cmd[12] = (rect[2] - 1) >> 8;
	cmd[13] = rect[3] - 1;
	cmd[14] = (rect[3] - 1) >> 8;
	cmd[15] = 0;

	wrap_scsi((DPFContext *) h, cmd, sizeof(g_excmd), DIR_OUT, (unsigned char*) buf, len);
}

/** Set backlight
 * 
 * \param value       Backlight value 0..7 (0 = off, 7 = max brightness)
 */
void dpf_ax_setbacklight(DPFAXHANDLE h, int b)
{
	unsigned char *cmd = g_excmd;

	if (b < 0) b = 0;
    if (b > 7) b = 7;

	cmd[6] = USBCMD_SETPROPERTY;
	cmd[7] = 0x01;		// PROPERTY_BRIGHTNESS
	cmd[8] = 0x00;      //PROPERTY_BRIGHTNESS >> 8;
	cmd[9] = b;
	cmd[10] = b >> 8;

	wrap_scsi((DPFContext *) h, cmd, sizeof(g_excmd), DIR_OUT, NULL, 0);
}


static unsigned char g_buf[] = {
	0x55, 0x53, 0x42, 0x43, // dCBWSignature
	0xde, 0xad, 0xbe, 0xef, // dCBWTag
	0x00, 0x80, 0x00, 0x00, // dCBWLength
	0x00, // bmCBWFlags: 0x80: data in (dev to host), 0x00: Data out
	0x00, // bCBWLUN
	0x10, // bCBWCBLength

	// SCSI cmd:
	0xcd, 0x00, 0x00, 0x00,
	0x00, 0x06, 0x11, 0xf8,
	0x70, 0x00, 0x40, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

#define ENDPT_OUT 1
#define ENDPT_IN 0x81

static int wrap_scsi(DPFContext *h, unsigned char *cmd, int cmdlen, char out,
	unsigned char *data, unsigned long block_len)
{
	int len;
	int ret;
	static unsigned char ansbuf[13]; // Do not change size.

	g_buf[14] = cmdlen;
	memcpy(&g_buf[15], cmd, cmdlen);

	g_buf[8] = block_len;
	g_buf[9] = block_len >> 8;
	g_buf[10] = block_len >> 16;
	g_buf[11] = block_len >> 24;

	ret = usb_bulk_write(h->udev, ENDPT_OUT, (const char *)g_buf, sizeof(g_buf), 1000);
	if (ret < 0) return ret;

	if (out == DIR_OUT) {
		if (data) {
			ret = usb_bulk_write(h->udev, ENDPT_OUT, (const char* )data, block_len, 3000);
			if (ret != (int) block_len) {
				fprintf(stderr, "dpf_ax ERROR: bulk write.\n");
				return ret;
			}
		}
	} else if (data) {
		ret = usb_bulk_read(h->udev, ENDPT_IN, (char *) data, block_len, 4000);
		if (ret != (int) block_len) {
			fprintf(stderr, "dpf_ax ERROR: bulk read.\n");
			return ret;
		}
	}
	// get ACK:
	len = sizeof(ansbuf);
	int retry = 0;
	int timeout = 0;
	do {
		timeout = 0;
		ret = usb_bulk_read(h->udev, ENDPT_IN, (char *) ansbuf, len, 5000);
		if (ret != len) {
			fprintf(stderr, "dpf_ax ERROR: bulk ACK read.\n");
			timeout = 1;
		}
		retry++;
	} while (timeout && retry < 5);
	if (strncmp((char *) ansbuf, "USBS", 4)) {
		fprintf(stderr, "dpf_ax ERROR: got invalid reply\n.");
		return -1;
	}
	// pass back return code set by peer:
	return ansbuf[12];
}

//###################################################################
// End dpfcore4driver.c
//###################################################################

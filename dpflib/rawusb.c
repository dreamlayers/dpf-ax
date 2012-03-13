/* Low level USB code to access DPF.
 *
 * (c) 2010, 2011 <hackfin@section5.ch>
 *
 * This currently uses the SCSI command set
 *
 * The reason for this is that we want to access the hacked frame
 * non-root and without having to wait for the SCSI interface to
 * intialize.
 *
 * Later, we'll replace the SCSI command stuff.
 */

#include "dpf.h"
#include "sglib.h"

#include <string.h>
#include <stdio.h>

#define ENDPT_OUT 1
#define ENDPT_IN 0x81

struct known_device {
	char *desc;
	unsigned short vid;
	unsigned short pid;
} g_known_devices[] = {
	{ "AX206 DPF", 0x1908, 0x0102 },
	{ "AX206 DPF (bootloader)", 0x1908, 0x3318 },
	{ 0 , 0, 0 } /* NEVER REMOVE THIS */
};

int handle_error(char *txt)
{
	fprintf(stderr, "Error: %s\n", txt);
	return -1;
}

void usb_flush(usb_dev_handle *dev)
{
	char buf[20];
	usb_bulk_read(dev, ENDPT_IN, buf, 3, 1000);
}

int check_known_device(struct usb_device *d)
{
	struct known_device *dev = g_known_devices;

	while (dev->desc) {
		if ((d->descriptor.idVendor == dev->vid) &&
			(d->descriptor.idProduct == dev->pid)) { 
				fprintf(stderr, "Found %s\n", dev->desc);
				return 1;
		}
		dev++;
	}
	return 0;
}

static struct usb_device *find_dev(int index)
{
	struct usb_bus *b;
	struct usb_device *d;
	int enumeration = 0;

	b = usb_get_busses();

	while (b) {
		d = b->devices;
		while (d) {
			if (check_known_device(d)) {
				if (enumeration == index) return d;
				else enumeration++;
			}

#ifdef DEBUG
			printf("enum: %d index: %d\n", enumeration, index);
			printf("%04x %04x\n",
				   d->descriptor.idVendor,
				   d->descriptor.idProduct);
#endif
			d = d->next;
		}
		b = b->next;
	}
	return NULL;
}

char g_buf[] = {
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

int emulate_scsi(usb_dev_handle *dev, unsigned char *cmd, int cmdlen, char out,
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

	ret = usb_bulk_write(dev, ENDPT_OUT, g_buf, sizeof(g_buf), 1000);
	if (ret < 0) return ret;

	if (out == DIR_OUT) {
		if (data) {
			ret = usb_bulk_write(dev, ENDPT_OUT, (const char* )data,
					block_len, 3000);
			if (ret != block_len) {
				perror("bulk write");
				return ret;
			}
		}
	} else if (data) {
		ret = usb_bulk_read(dev, ENDPT_IN, (char *) data, block_len, 4000);
		if (ret != block_len) {
			perror("bulk data read");
		}
	}
	// get ACK:
	len = sizeof(ansbuf);
	int retry = 0;
	do {
		ret = usb_bulk_read(dev, ENDPT_IN, (char *) ansbuf, len, 5000);
		if (ret != len) {
			perror("bulk ACK read");
			ret = DEVERR_TIMEOUT;
		}
		retry++;
	} while (ret == DEVERR_TIMEOUT && retry < 5);
	if (strncmp((char *) ansbuf, "USBS", 4)) {
		return handle_error("Got invalid reply\n");
	}
	// pass back return code set by peer:
	return ansbuf[12];
}

int dpf_usb_open(int index, usb_dev_handle **u)
{
	struct usb_device *d;
	usb_dev_handle *usb_dev;

	usb_init();
	usb_find_busses();
	usb_find_devices();

	d = find_dev(index);
	if (!d) {
		handle_error("No matching USB device found!");
		return -1;
	}

	usb_dev = usb_open(d);
	if (usb_dev == NULL) {
		handle_error("Failed to open usb device!");
		return -1;
	}
	if (usb_claim_interface(usb_dev, 0) < 0) {
		handle_error("Failed to claim usb device!");
		printf("Possibly you have to 'sudo libhid-detach-device 1908:3318'\n");
		return -1;
	}
	*u = usb_dev;

	if (d->descriptor.idProduct == 0x3318)
		return MODE_USBHID;
	else
		return MODE_USB;
}

int usb_rawwrite(usb_dev_handle *dev, const unsigned char *buf, int len)
{
	return usb_interrupt_write(dev, ENDPT_OUT, (char *) buf, len, 1000);
}

int usb_rawread(usb_dev_handle *dev, unsigned char *buf, int len)
{
	return usb_interrupt_read(dev, ENDPT_IN, (char *) buf, len, 4000);
}

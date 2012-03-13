/** 
 * (c) 2011, <hackfin@section5.ch>
 *
 */

#include "sglib.h"
#include "usbuser.h" // our user defined flash commands
#include "dpf.h"

#include <scsi/scsi.h>
#include <scsi/sg.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>


int do_scsi(int fd, unsigned char *cmd, int cmdlen, char out,
	unsigned char *data, unsigned long block_len)
{
	int error;
	unsigned char sensebuf[32];
	sg_io_hdr_t io_hdr;
    memset(&io_hdr, 0, sizeof(sg_io_hdr_t));

    io_hdr.interface_id = 'S';
    io_hdr.sbp = sensebuf;
    io_hdr.mx_sb_len = sizeof(sensebuf);
	if (data == 0) {
		io_hdr.dxfer_direction = SG_DXFER_NONE;
	} else {
		if (out) {
			io_hdr.dxfer_direction = SG_DXFER_TO_DEV;
		} else {
			io_hdr.dxfer_direction = SG_DXFER_FROM_DEV;
		}
	}
    io_hdr.dxferp = data;
    io_hdr.dxfer_len = block_len;
    io_hdr.cmdp = cmd;
    io_hdr.cmd_len = cmdlen;
    io_hdr.timeout = 5000; // in ms
	error = ioctl(fd, SG_IO, &io_hdr);
	if (error < 0) perror("calling SCSI ioctl()");
	return error;
}


int wrap_scsi(DPFContext *h, unsigned char *cmd, int cmdlen, char out,
	unsigned char *data, unsigned long block_len)
{
	int error;
	switch (h->mode) {
		case MODE_USB:
			error = emulate_scsi(h->dev.udev,
				cmd, cmdlen, out, data, block_len);
			break;
		case MODE_SG:
			error = do_scsi(h->dev.fd, cmd, cmdlen, out, data, block_len);
			break;
		case MODE_USBHID:
			fprintf(stderr, "USBHID mode not (yet) supported\n");
			error = -1;
	}
	return error;
}


/** Vendor command for our hacks */
static
unsigned char g_excmd[16] = {
	0xcd, 0, 0, 0,
	   0, 6, 0, 0,
	   0, 0, 0, 0,
	   0, 0, 0, 0
};


/* device wrapper */

const char idstring[] = "buildwin Photo Frame    1.01";

#define INQ_REPLY_LEN 96

int sgdev_open(const char *portname, int *fd)
{
	int error;

	static
	unsigned char inquiry[] = {
		INQUIRY, 0, 0, 0,
		INQ_REPLY_LEN, 0
	};

	static unsigned char inqbuf[INQ_REPLY_LEN + 2];

	*fd = open(portname, O_RDONLY | O_NONBLOCK );
	error = do_scsi(*fd, inquiry, sizeof(inquiry), DIR_IN, inqbuf,
		INQ_REPLY_LEN);
	
	if (error < 0) {
		fprintf(stderr, "SCSI inquiry failed\n");
		close(*fd); error = DEVERR_OPEN;
	} else 
	if (memcmp(idstring, &inqbuf[8], sizeof(idstring) - 1) != 0) {
		close(*fd); error = DEVERR_OPEN;
		fprintf(stderr, "Not a photo frame. Refuse to open device.\n");
	}
	return error;
}

int probe(DPFHANDLE h)
{
	int ret;

	// We abuse a command that just responds with a '0' status in the
	// original firmware.
	static unsigned char buf[5];


	static
	unsigned char cmd[16] = {
		0xcd, 0, 0, 0,
		   0, 3, 0, 0,
		   0, 0, 0, 0,
		   0, 0, 0, 0
	};

	cmd[5] = 3;
	ret = wrap_scsi(h, cmd, sizeof(cmd), DIR_IN, 0, 0);

	switch (ret) {
		case 0:
			// The original protocol.
			fprintf(stderr,
				"Warning: This firmware can not lock the flash\n");
			break;
		case 1:
			// The improved hack
			h->flags |= FLAG_CAN_LOCK;
			break;
		default:
			perror("Detecting protocol");
	}

	cmd[5] = 2; // get LCD parameters
	ret = wrap_scsi(h, cmd, sizeof(cmd), DIR_IN, buf, 5);
	h->width = (buf[0]) | (buf[1] << 8);
	h->height = (buf[2]) | (buf[3] << 8);
	h->bpp = 2;

	return ret;
}

/* Memory stuff */

static
int mem_read_scsi(DPFContext *h, unsigned char *buf, ADDR offset, int len)
{
	unsigned char *cmd = g_excmd;

	if (h->mode == MODE_USBHID) return DEVERR_UNSUPP;

	cmd[6] = USBCMD_MEMREAD; // memory_read
	cmd[7] = offset;
	cmd[8] = offset >> 8;
	cmd[9] = len;
	cmd[10] = len >> 8;

	return wrap_scsi(h, cmd, sizeof(g_excmd), DIR_IN, buf, len);
}

int mem_write_scsi(DPFContext *h,
	ADDR dst, const unsigned char *src, unsigned short n)
{
	unsigned char *cmd = g_excmd;

	cmd[6] = 0x01; // memory_write
	cmd[7] = dst;
	cmd[8] = dst >> 8;
	cmd[9] = n;
	cmd[10] = n >> 8;

	return wrap_scsi(h, cmd, sizeof(g_excmd), DIR_OUT, (unsigned char *) src, n);
}



/* Flash stuff, SCSI wrapped */

static
int flash_cmd_scsi(DPFContext *h, int command, int cmdlen, ADDR addr)
{
	static
	unsigned char cmd[16] = {
		0xcb, 0, 0, 0, 0, 0,
		1, 0, 0, 0,
		0, 0, 0, 0,
		0, 0
	};

	cmd[10] = command;
	cmd[6] = cmdlen;

	// Sector number or addr:
	cmd[13] = addr;
	cmd[12] = addr >> 8;
	cmd[11] = addr >> 16;

	return wrap_scsi(h, cmd, sizeof(cmd), DIR_IN, 0, 0);
}

int flash_status_scsi(DPFHANDLE h, uint8_t *status)
{
	*status = 0;
	// FIXME : Really get status
	return 0;
}

static
int flash_read_scsi(DPFContext *h, unsigned char *buf, ADDR offset, int len)
{
	static
	unsigned char cmd[16] = {
		0xcd, 0, 0, 0, 0, 0,
		0x04, /* num of bytes: */ 0, 0, 0,
		SPM_READ, /* SPI offset: */ 0, 0, 0,
		0, 0
	};

	cmd[9] = len >> 0;
	cmd[8] = len >> 8;
	cmd[7] = len >> 16;

	cmd[13] = offset >> 0;
	cmd[12] = offset >> 8;
	cmd[11] = offset >> 16;

	return wrap_scsi(h, cmd, sizeof(cmd), DIR_IN, buf, len);
}

static
int flash_writechunk_scsi(DPFContext *h,
	const unsigned char *buf, ADDR offset, int len)
{
	int error;
	static
	unsigned char cmd[16] = {
		0xcb, 0, 0, 0, 0, 0,
		4, /* num of bytes: */ 0, 0, 0,
		SPM_PP, /* SPI offset: */ 0, 0, 0,
		0, 0
	};

	if (len > MAX_CHUNKSIZE) len = MAX_CHUNKSIZE;

	cmd[9] = len >> 0;
	cmd[8] = len >> 8;
	cmd[7] = len >> 16;

	cmd[13] = offset >> 0;
	cmd[12] = offset >> 8;
	cmd[11] = offset >> 16;

	error = wrap_scsi(h, cmd, sizeof(cmd), DIR_OUT,
		(unsigned char*) buf, len);
	if (error < 0) return error;
	return len;
}

int flash_lock_usb(DPFContext *h, char enable)
{
	unsigned char *cmd = g_excmd;

	if (!(h->flags & FLAG_CAN_LOCK)) return DEVERR_UNSUPP;

	printf("Lock flash %d\n", enable);
	cmd[6] = USBCMD_FLASHLOCK; // flash lock
	cmd[7] = enable;

	wrap_scsi(h, cmd, sizeof(g_excmd), DIR_IN, 0, 0);
	return 0;
}

static
int flash_probe_scsi(DPFContext *h, unsigned char *id)
{
	int error;
	static
	unsigned char buf[64];
	static
	unsigned char cmd[16] = {
		0xcd, 0, 0, 0, 0, 0,
		1, sizeof(buf) >> 16, sizeof(buf) >> 8, sizeof(buf) >> 0,
		// flash command sequence:
		SPM_RDID, 0, 0, 0,
		0, 0
	};

	error = wrap_scsi(h, cmd, sizeof(cmd), DIR_IN, buf, sizeof(buf));
	id[0] = buf[0]; id[1] = buf[1]; id[2] = buf[2];
	return error;
}


static
int go_scsi(DPFContext *h, ADDR start)
{
	unsigned char *cmd = g_excmd;
	cmd[6] = 0x02;   // execute
	cmd[7] = start;
	cmd[8] = start >> 8;

	return wrap_scsi(h, cmd, sizeof(g_excmd), DIR_IN, 0, 0);
}

static
int bootstrap_scsi(DPFContext *h,
	ADDR dest, unsigned char *src, unsigned short n, ADDR start)
{
	unsigned char *cmd = g_excmd;

	cmd[6] = USBCMD_APPLOAD;   // Enter bootstrap mode
	cmd[7] = dest;
	cmd[8] = dest >> 8;
	cmd[9] = n;
	cmd[10] = n >> 8;
	cmd[11] = start;
	cmd[12] = start >> 8;

	return wrap_scsi(h, cmd, sizeof(g_excmd), DIR_OUT, src, n);
}

AccessMethods
scsi_methods = {
	.mem_read         = mem_read_scsi,
	.mem_write        = mem_write_scsi,
	.go               = go_scsi,
	.bootstrap        = bootstrap_scsi,
	.flash_probe      = flash_probe_scsi,
	.flash_cmd        = flash_cmd_scsi,
	.flash_status     = flash_status_scsi,
	.flash_read       = flash_read_scsi,
	.flash_writechunk = flash_writechunk_scsi,
	.flash_lock       = flash_lock_usb,
};

////////////////////////////////////////////////////////////////////////////
// SCSI only high level commands:

int dpf_setcol(DPFContext *h, const unsigned char *rgb)
{
	unsigned char *cmd = g_excmd;

	if (h->mode == MODE_USBHID) return DEVERR_UNSUPP;

	cmd[6] = USBCMD_SETPROPERTY;
	cmd[7] = PROPERTY_FGCOLOR;
	cmd[8] = PROPERTY_FGCOLOR >> 8;

	cmd[9] = RGB565_0(rgb[0], rgb[1], rgb[2]);
	cmd[10] = RGB565_1(rgb[0], rgb[1], rgb[2]);

	return wrap_scsi(h, cmd, sizeof(g_excmd), DIR_OUT, NULL, 0);
}

int dpf_screen_blit(DPFContext *h,
	const unsigned char *buf, short rect[4])
{
	unsigned long len = (rect[2] - rect[0]) * (rect[3] - rect[1]);
	len <<= 1;
	unsigned char *cmd = g_excmd;

	if (h->mode == MODE_USBHID) return DEVERR_UNSUPP;

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

	return wrap_scsi(h, cmd, sizeof(g_excmd), DIR_OUT,
		(unsigned char*) buf, len);
}

int dpf_setproperty(DPFContext *h, int token, const DPFValue *value)
{
	unsigned char *cmd = g_excmd;

	if (h->mode == MODE_USBHID) return DEVERR_UNSUPP;

	cmd[6] = USBCMD_SETPROPERTY;
	cmd[7] = token;
	cmd[8] = token >> 8;

	switch (value->type) {
		case TYPE_INTEGER:
			cmd[9] = value->value.integer;
			cmd[10] = value->value.integer >> 8;
			break;
		default:
			break;
	}

	return wrap_scsi(h, cmd, sizeof(g_excmd), DIR_OUT, NULL, 0);
}



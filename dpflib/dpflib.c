/** DPF access library for AX206 based HW
 *
 * 12/2010 <hackfin@section5.ch>
 *
 * This is an ugly hack, because we use existing SCSI vendor specific
 * extensions to pass on our requests to the DPF.
 *
 * One day we might use a proper protocol like netpp.
 *
 */
	
// FIXME: Put all those SCSI commands in one (wrapped) place.

#include "dpf.h"
#include "sglib.h"

#include "flash.h"
#include <fcntl.h>
#include "usbuser.h" // our user defined flash commands
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <scsi/scsi.h>
#include <scsi/sg.h>
#include <sys/ioctl.h>

#ifdef DEBUG
#define DEB(x) x
#else
#define DEB(x)
#endif

/** Vendor command for our hacks */
static
unsigned char g_excmd[16] = {
	0xcd, 0, 0, 0,
	   0, 6, 0, 0,
	   0, 0, 0, 0,
	   0, 0, 0, 0
};

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
	if (h->mode == MODE_USB) {
		error = emulate_scsi(h->dev.udev, cmd, cmdlen, out, data, block_len);
	} else {
		error = do_scsi(h->dev.fd, cmd, cmdlen, out, data, block_len);
	}
	return error;
}

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

static
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
	}

	cmd[5] = 2; // get LCD parameters
	ret = wrap_scsi(h, cmd, sizeof(cmd), DIR_IN, buf, 5);
	h->width = (buf[0]) | (buf[1] << 8);
	h->height = (buf[2]) | (buf[3] << 8);
	h->bpp = 2;

	return ret;
}

/*
static
int dpf_query(DPFHANDLE h)
{
	int ret;
	unsigned char buf[64];	// Do not change size

	static
	unsigned char cmd[16] = {
		0xcd, 0, 0, 0,
		   0, 3, 0, 0,
		   0, 0, 0, 0,
		   0, 0, 0, 0
	};

	return wrap_scsi(h, cmd, sizeof(cmd), DIR_IN, buf, sizeof(buf));
}
*/



int dpf_open(const char *dev, DPFHANDLE *h)
{
	int error = 0;
	DPFContext *dpf;
	int i;
	usb_dev_handle *u;

	int fd;

	if (!dev) {
		fprintf(stderr, "Please specify a string like 'usb0' or a sg device\n");
		return DEVERR_OPEN;
	}

	if (strncmp(dev, "usb", 3) == 0) {
		i = dev[3] - '0';
		u = dpf_usb_open(i);
		if (!u) return DEVERR_OPEN;
		i = MODE_USB;
	} else {
		fprintf(stderr, "Opening generic SCSI device '%s'\n", dev);
		if (sgdev_open(dev, &fd) < 0) return DEVERR_OPEN;
		i = MODE_SG;
	}

	dpf = (DPFHANDLE) malloc(sizeof(DPFContext));
	if (!dpf) return DEVERR_MALLOC;

	dpf->flags = 0;
	dpf->mode = i;

	if (dpf->mode == MODE_USB) {
		dpf->dev.udev = u;
		error = probe(dpf);
		fprintf(stderr, "Got LCD dimensions: %dx%d\n", dpf->width, dpf->height);
	} else {
		dpf->dev.fd = fd;
	}

	*h = dpf;
	return error;
}

void dpf_close(DPFContext *h)
{
	switch (h->mode) {
		case MODE_SG:
			close(h->dev.fd);
			break;
		case MODE_USB:
			usb_release_interface(h->dev.udev, 0);
			usb_close(h->dev.udev);
			break;
	}
	free(h);
}

const char *dev_errstr(int err)
{
	switch (err) {
	case DEVERR_FILE: return "File I/O error";
	case DEVERR_OPEN: return "File open error";
	case DEVERR_HEX: return "Hex file error";
	case DEVERR_CHK: return "Checksum error";
	case DEVERR_IO: return "Common I/O error";
	default: return "Unknown error";
	}
}

/* Flash stuff */

int flash_cmd(DPFContext *h, int command, int cmdlen, ADDR addr)
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


int flash_read(DPFContext *h, unsigned char *buf, ADDR offset, int len)
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
int flash_writechunk(DPFContext *h, const unsigned char *buf, ADDR offset, int len)
{
	static
	unsigned char cmd[16] = {
		0xcb, 0, 0, 0, 0, 0,
		4, /* num of bytes: */ 0, 0, 0,
		SPM_PP, /* SPI offset: */ 0, 0, 0,
		0, 0
	};

	cmd[9] = len >> 0;
	cmd[8] = len >> 8;
	cmd[7] = len >> 16;

	cmd[13] = offset >> 0;
	cmd[12] = offset >> 8;
	cmd[11] = offset >> 16;

	return wrap_scsi(h, cmd, sizeof(cmd), DIR_OUT,
		(unsigned char*) buf, len);
}

int flash_write(DPFContext *h, const unsigned char *buf, ADDR offset, int len)
{
	int i;
	int error = 0;

	for (i = 0; i < len; i += 0x1000) {
		error = flash_cmd(h, SPM_WREN, 1, 0);
		DEB(printf("Write pages at %06x\n", offset));
		error = flash_writechunk(h, buf, offset, 0x1000);
		if (error < 0) break;
		buf += 0x1000; offset += 0x1000;
	}
	return error;
}


int load_ihx(const char *fname, unsigned char *data, 
	unsigned int *buflen, unsigned int reloc)
{
	unsigned char csum_is, csum_need;
	int ret;
	FILE *f;

	static char line[512];
	static unsigned char buf[0x100];
	int count;
	unsigned int addr, len, type;
	unsigned short b;
	unsigned int total = 0;

	DEB(printf("Opening %s...\n", fname));
    f = fopen(fname, "r");
    if (f == NULL) {
        return DEVERR_OPEN; 
    }

    while(1) {
        fgets(line, sizeof(line), f);

        if (feof(f) || ferror(f)) break;

        if ((line[0] != ':') || (strlen(line) < 9)) {
            fprintf(stderr, "invalid line in ihx\n");
            break;
        }

        ret = sscanf(&line[1], "%02x", &len);
        if (ret != 1) { ret = DEVERR_HEX; break; }

        ret = sscanf(&line[3], "%04x", &addr);
        if (ret != 1) { ret = DEVERR_HEX; break; }

        ret = sscanf(&line[7], "%02x", &type);
        if (ret != 1) { ret = DEVERR_HEX; break; }

#ifdef DEBUG
        printf("len %u addr %04x type %u\n", len, addr, type);
#endif

        if (type == 1) break;

        if (type != 0) {
            fprintf(stderr, "ihx: unknown type %u\n", type);
            continue;
        }

        csum_need = len + (addr & 0xff) + (addr >> 8) + type;

		total += len;
		if (total > *buflen) {
			fprintf(stderr, "Buffer length exceeded. IHX file too big.\n");
			ret = DEVERR_HEX;
			break;
		}

		if (len > sizeof(buf)) {
			fprintf(stderr, "Buffer length exceeded. Too long lines.\n");
			ret = DEVERR_HEX;
			break;
		}

        for(count = 0; count < len; count++) {
            ret = sscanf(&line[9 + count * 2], "%02hx", &b);
            if (ret != 1) {
                fprintf(stderr, "hex file: could not parse data!\n");
                break;
            }

            buf[count] = b;
            csum_need += buf[count];
        }

        if (ret != 1) { ret = DEVERR_HEX; break; }

        ret = sscanf(&line[9 + len * 2], "%02hx", &b);
        if (ret != 1) { ret = DEVERR_HEX; break; }

        csum_is = b;
        if (((csum_need+csum_is) & 0xff) != 0x00) {
            fprintf(stderr, "ihx: checksum failure! is: %02x should be:%02x\n",
                   csum_is, csum_need);
            ret = DEVERR_CHK;
			break;
        }

		if (addr < reloc) {
			fprintf(stderr, "Bad relocation value\n");
			ret = DEVERR_HEX;
			break;
		}
		// Copy to data buffer at relocated address
		if (data) {
			DEB(printf("Patching at offset %08x, chunk size %d\n",
				addr - reloc, len));
			memcpy(&data[addr - reloc], buf, len);
		} else {
			DEB(printf("Writing to %04x (CODE: %04x), chunk size %d\n",
				addr - reloc, addr, len));
			dpf_copy(addr - reloc, buf, len);
		}
    }
	*buflen = total;
    fclose(f);
	return ret;
}


int flash_probe(DPFContext *h, unsigned char *id)
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

int flash_erase(DPFContext *h, ADDR addr)
{
	int error;
	error = flash_cmd(h, SPM_RES, 1, 0);
	error = flash_cmd(h, SPM_WREN, 1, 0);
	error = flash_cmd(h, SPM_WRSR, 2, 0); // clr status

	// now erase flash sector:
	error = flash_cmd(h, SPM_WREN, 1, 0);
	error = flash_cmd(h, SPM_FLASH_SE, 4, addr);
	return error;
}

void dpf_flash_lock(DPFContext *h, char enable)
{
	unsigned char *cmd = g_excmd;

	if (!(h->flags & FLAG_CAN_LOCK)) return;

	printf("Lock flash %d\n", enable);
	cmd[6] = USBCMD_FLASHLOCK; // flash lock
	cmd[7] = enable;

	wrap_scsi(h, cmd, sizeof(g_excmd), DIR_IN, 0, 0);
}


int patch_sector(DPFContext *h,
	ADDR reloc, unsigned long addr, const char *hexfile)
{
	int error;
	unsigned short offset;
	static
	unsigned char readbuf[0x10000];
	unsigned int len = sizeof(readbuf);

	offset = addr & 0xffff;
	addr -= offset;

	error = flash_read(h, readbuf, addr, 0x10000);
	if (error < 0) {
		perror("Reading flash");
		return error;
	}

	error = load_ihx(hexfile, &readbuf[offset], &len, reloc);
	if (error < 0) {
		fprintf(stderr, "Failed to load HEX file\n");
		return error;
	}
 // Lock DPF handler so nothing can interfere with the flashing (in case
 // we flash ourselves)
	dpf_flash_lock(h, 1);
	error = flash_cmd(h, SPM_WREN, 1, 0);
	error = flash_cmd(h, SPM_WRSR, 2, 0); // clr status

	error = flash_erase(h, addr);
	if (error < 0) return error;

	error = flash_write(h, readbuf, addr, 0x10000); // clr status
	dpf_flash_lock(h, 0);

	return error;
}

////////////////////////////////////////////////////////////////////////////
// High level functions, generic
// These require a hacked command handler with extended command set.


static DPFHANDLE g_dpf;

int write_mem(DPFContext *h, const char *hexfile)
{
	unsigned int len = 0xc000;
	int error;

	g_dpf = h;
	error = load_ihx(hexfile, 0, &len, 0x800);
	return error;
}

int read_blk(DPFContext *h, unsigned char *buf, ADDR offset, int len)
{
	unsigned char *cmd = g_excmd;

	cmd[6] = USBCMD_MEMREAD; // memory_read
	cmd[7] = offset;
	cmd[8] = offset >> 8;
	cmd[9] = len;
	cmd[10] = len >> 8;

	return wrap_scsi(h, cmd, sizeof(g_excmd), DIR_IN, buf, len);
}

#define CHUNK_SIZE 1024

int read_mem(DPFContext *h, unsigned char *buf, ADDR offset, int len)
{
	int error = 0;
	while (len > CHUNK_SIZE && error >= 0) {
		error = read_blk(h, buf, offset, CHUNK_SIZE);
		offset += CHUNK_SIZE; len -= CHUNK_SIZE; buf += CHUNK_SIZE;
	}
	error = read_blk(h, buf, offset, len);
	
	return error;
}


int code_go(DPFContext *h, ADDR start)
{
	unsigned char *cmd = g_excmd;
	printf("Executing applet..\n");
	cmd[6] = 0x02;   // execute
	cmd[7] = start;
	cmd[8] = start >> 8;

	return wrap_scsi(h, cmd, sizeof(g_excmd), DIR_IN, 0, 0);
}



////////////////////////////////////////////////////////////////////////////
// High level functions, DPF specific

/* Bootstrap mode: Expects contiguous memory block to download, then jumps
 * into start address
 */
int dpf_bootstrap(DPFContext *h,
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

int dpf_copy(ADDR dst, unsigned char *src, unsigned short n)
{
	unsigned char *cmd = g_excmd;

	cmd[6] = 0x01; // memory_write
	cmd[7] = dst;
	cmd[8] = dst >> 8;
	cmd[9] = n;
	cmd[10] = n >> 8;

	return wrap_scsi(g_dpf, cmd, sizeof(g_excmd), DIR_OUT, src, n);
}

/*  Currently unused.
int clr_screen(DPFContext *h, const unsigned char *col)
{
	unsigned char *cmd = g_excmd;

	cmd[6] = USBCMD_PROBE;
	cmd[7] = col[0];
	cmd[8] = col[1];

	return wrap_scsi(h, cmd, sizeof(g_excmd), DIR_IN, 0, 0);
}
*/


int dpf_setcol(DPFContext *h, const unsigned char *rgb)
{
	unsigned char *cmd = g_excmd;

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


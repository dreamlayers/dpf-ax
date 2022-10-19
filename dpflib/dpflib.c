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

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifdef DEBUG
#define DEB(x) x
#else
#define DEB(x)
#endif

extern AccessMethods scsi_methods;
extern AccessMethods hid_methods;


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
	libusb_device_handle *u;

	int fd;

	if (!dev) {
		fprintf(stderr, "Please specify a string like 'usb0' or a sg device\n");
		return DEVERR_OPEN;
	}

	if (strncmp(dev, "usb", 3) == 0) {
		i = dev[3] - '0';
		error = dpf_usb_open(i, &u);
		if (error < 0) return error;
		if (!u) return DEVERR_OPEN;
		i = error; // USB mode
		error = 0;
	} else {
		fprintf(stderr, "Opening generic SCSI device '%s'\n", dev);
		if (sgdev_open(dev, &fd) < 0) return DEVERR_OPEN;
		i = MODE_SG;
	}

	dpf = (DPFHANDLE) malloc(sizeof(DPFContext));
	if (!dpf) return DEVERR_MALLOC;

	dpf->flags = 0;
	dpf->mode = i;

	switch (dpf->mode) {
		case MODE_USB:
			dpf->dev.udev = u;
			error = probe(dpf); // probe AFTER assigning handle!
			fprintf(stderr, "Got LCD dimensions: %dx%d\n",
				dpf->width, dpf->height);
			dpf->methods = scsi_methods;
			break;
		case MODE_USBHID:
			dpf->dev.udev = u;
			dpf->methods = hid_methods;
			break;
		case MODE_SG:
			dpf->dev.fd = fd;
			dpf->methods = scsi_methods;
			break;
		default:
			fprintf(stderr, "Unknown interface mode\n");
			error = -1;
	}

	*h = dpf;
	return error;
}

void dpf_close(DPFContext *h)
{
	switch (h->mode) {
		case MODE_USBHID:
		case MODE_SG:
			close(h->dev.fd);
			break;
		case MODE_USB:
			libusb_release_interface(h->dev.udev, 0);
			libusb_close(h->dev.udev);
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
	case DEVERR_UNSUPP: return "Unsupported feature";
	default: return "Unknown error";
	}
}


int flash_probe(DPFContext *h, unsigned char *id)
{
	return h->methods.flash_probe(h, id);
}

int flash_cmd(DPFContext *h, int command, int cmdlen, ADDR addr)
{
	return h->methods.flash_cmd(h, command, cmdlen, addr);
}


/* Flash functions, API */

int flash_read(DPFContext *h, unsigned char *buf, ADDR offset, int len)
{
	return h->methods.flash_read(h, buf, offset, len);
}

int flash_status_wait(DPFContext *h, uint8_t mask)
{
	int error;
	uint8_t status;
	
	do {
		error = h->methods.flash_status(h, &status);
	} while ((status & mask) && !error);

	return error;
}


int flash_write(DPFContext *h, const unsigned char *buf, ADDR offset, int len)
{
	int n;
	int error = 0;

	while (len > 0) {
		error = h->methods.flash_cmd(h, SPM_WREN, 1, 0);
		DEB(printf("Write pages at %06x\n", offset));
		n = h->methods.flash_writechunk(h, buf, offset, len);
		error = flash_status_wait(h, SPS_WIP);

		if (n < 0) break;
		len -= n; buf += n; offset += n;
	}
	return error;
}

/* Mid level flash */

int load_ihx(DPFContext *h, const char *fname, unsigned char *data, 
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
			h->methods.mem_write(h, addr - reloc, buf, len);
		}
    }
	*buflen = total;
    fclose(f);
	return ret;
}

int flash_erase_full(DPFContext *h)
{
	flash_cmd(h, SPM_RES, 1, 0);
	flash_cmd(h, SPM_WRSR, 2, 0); // clr status
	flash_cmd(h, SPM_WREN, 1, 0);
	flash_cmd(h, SPM_FLASH_BE, 1, 0);
	printf("Erase full flash...\n");

	return flash_status_wait(h, SPS_WIP);
}

int flash_erase(DPFContext *h, ADDR addr)
{
	int error;
	flash_cmd(h, SPM_RES, 1, 0);
	flash_cmd(h, SPM_WREN, 1, 0);
	flash_cmd(h, SPM_WRSR, 2, 0); // clr status

	// now erase flash sector:
	flash_cmd(h, SPM_WREN, 1, 0);
	flash_cmd(h, SPM_FLASH_SE, 4, addr);

	error = flash_status_wait(h, SPS_WIP);
	flash_cmd(h, SPM_WRDI, 1, 0);

	return error;
}

int dpf_flash_lock(DPFContext *h, char enable)
{
	if (h->methods.flash_lock) 
		return h->methods.flash_lock(h, enable);
	else
		return DEVERR_UNSUPP;
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

	error = load_ihx(h, hexfile, &readbuf[offset], &len, reloc);
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


int load_hexfile(DPFContext *h, const char *hexfile)
{
	unsigned int len = 0xc000;
	int error;

	error = load_ihx(h, hexfile, 0, &len, 0x800);
	return error;
}

#define CHUNK_SIZE 1024

int read_mem(DPFContext *h, unsigned char *buf, ADDR src, unsigned short len)
{
	int error = 0;
	if (!h->methods.mem_read) return DEVERR_UNSUPP;

	while (len > CHUNK_SIZE && error >= 0) {
		error = h->methods.mem_read(h, buf, src, CHUNK_SIZE);
		src += CHUNK_SIZE; len -= CHUNK_SIZE; buf += CHUNK_SIZE;
	}
	error = h->methods.mem_read(h, buf, src, len);
	
	return error;
}

int write_mem(DPFContext *h, ADDR dst, const unsigned char *buf, unsigned short len)
{
	return h->methods.mem_write(h, dst, buf, len);
}

int code_go(DPFContext *h, ADDR start)
{
	printf("Executing applet..\n");
	if (h->methods.go)
		return h->methods.go(h, start);
	return DEVERR_UNSUPP;
}



////////////////////////////////////////////////////////////////////////////
// High level functions, DPF specific

/* Bootstrap mode: Expects contiguous memory block to download, then jumps
 * into start address
 */

int dpf_bootstrap(DPFContext *h,
	ADDR dest, unsigned char *src, unsigned short n, ADDR start)
{
	if (h->methods.bootstrap)
		return h->methods.bootstrap(h, dest, src, n, start);
	else
		return DEVERR_UNSUPP;
}




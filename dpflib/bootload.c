// Bootstream generator for Buildwin firmware loader
//
// (c) 2011, <hackfin@section5.ch>
//
//

#include "dpf.h"
#include "usbuser.h" // our user defined flash commands
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

// Jump offsets for USB command:
#define JMP_LOAD     0x0dae
#define JMP_SPILIB   0x11bf
#define JMP_USER     0x1000

#define PAGESIZE 256
#define BUFSIZE (64 - 6)
#define PKTSIZE (64 - 7)

typedef struct {
	unsigned char len;
	unsigned char chk;
	unsigned char jmp[2];
	union {
		// Structures for various backends
		// The default memory loader:
		struct {
			unsigned char offset[2];    // XDATA memory dest address
			unsigned char buf[BUFSIZE]; // Binary data to write
		} loader;
		struct {
			unsigned char opcode;   // SPI operation opcode
			unsigned char wnum;     // Number of bytes to send (Host -> DPF)
			unsigned char rnum;     // Number of bytes to recv (DPF -> Host)
			unsigned char seq[PKTSIZE];  // SPI cmd sequence
		} spi;
	} u;
} UsbMsg;

#define OPCODE_SPISEL  0x01  // assert SELECT
#define OPCODE_CRYPT0  0x02
#define OPCODE_CRYPT1  0x04
#define OPCODE_SPIDES  0x10  // assert DESELECT

int validate(UsbMsg *msg)
{
	unsigned short c = 0;
	unsigned char *b = (unsigned char *) msg;
	unsigned short l = msg->len;

	if (l < 6 || l > 64) return -1;

	while (l--) {
		c += *b++;
	}

	msg->chk -= c;

	return 0;
}

void hexdump(unsigned char *buf, unsigned long n)
{
	int i = 0;
	int c = 0;

	while (i < n) {
		printf("%02x ", buf[i]);
		c++;
		if (c == 16) { c = 0; printf("\n"); }
		i++;
	}
	if (c)
		printf("\n");
}

int transmit(DPFContext *dpf, UsbMsg *m)
{
	int error;
	unsigned char *s = (unsigned char *) m;
	// hexdump(s, m->len);
	error = usb_rawwrite(dpf->dev.udev, s, m->len);
	if (error < 0) {
		perror("Writing to USB");
	}
	return error;
}

int spilib_process(DPFContext *h, UsbMsg *umsg, unsigned char *out)
{
	int error;
	umsg->jmp[0] = JMP_SPILIB & 0xff; umsg->jmp[1] = JMP_SPILIB >> 8;
	umsg->len = 7 + umsg->u.spi.wnum;

	validate(umsg);
	error = transmit(h, umsg);
	if (umsg->u.spi.rnum > 0 && error >= 0) {
		error = usb_rawread(h->dev.udev, out, 64);
	}
	if (error < 0) perror("Reading USB");
	else error = 0;
	return error;
}

int go_hid(DPFContext *dpf, ADDR jmpoffset)
{
	UsbMsg umsg;
	umsg.jmp[0] = jmpoffset; umsg.jmp[1] = jmpoffset >> 8;
	umsg.len = 8;
	validate(&umsg);
	return transmit(dpf, &umsg);
}

int load(DPFContext *dpf, FILE *f, uint16_t jmpoffset)
{
	int size;
	UsbMsg umsg;

	unsigned short offset = jmpoffset - 0x800;

	umsg.jmp[0] = JMP_LOAD & 0xff; umsg.jmp[1] = JMP_LOAD >> 8;
	umsg.chk = 0xce;
	size = fread(umsg.u.loader.buf, 1, BUFSIZE, f);
	if (size < 0) return size;

	while (size > 0) {
		umsg.u.loader.offset[0] = offset;
		umsg.u.loader.offset[1] = offset >> 8;
		umsg.len = sizeof(UsbMsg) - BUFSIZE + size;
		offset += size;
		validate(&umsg);
		transmit(dpf, &umsg);
		size = fread(umsg.u.loader.buf, 1, BUFSIZE, f);
	} 

	// And jump into the code:
	if (jmpoffset != 0x0000) {
		go_hid(dpf, jmpoffset);
	}
	return size;
}
/* Flash stuff, HID wrapped */

static unsigned char s_rbuf[64];

static
int flash_probe_hid(DPFContext *h, unsigned char *id)
{
	int error;
	UsbMsg umsg;
	FILE *f;

	// First, load spilib.bin
	f = fopen("spilib.bin", "rb");

	if (!f) {
		printf("failed to open flash driver\n");
		return -1;
	}
	umsg.u.spi.opcode = OPCODE_SPISEL | OPCODE_SPIDES;

	error = load(h, f, 0x0000); // Load, but do not execute
	fclose(f);
	usleep(500000);
	if (error == 0) {
		umsg.u.spi.wnum = 1;
		umsg.u.spi.rnum = 4;
		umsg.u.spi.seq[0] = SPM_RDID;
		error = spilib_process(h, &umsg, s_rbuf);
		id[0] = s_rbuf[0]; id[1] = s_rbuf[1]; id[2] = s_rbuf[2];
	}
	return error;
}

static
int flash_cmd_hid(DPFContext *h, int command, int cmdlen, ADDR addr)
{
	UsbMsg umsg;
	umsg.u.spi.opcode = OPCODE_SPISEL | OPCODE_SPIDES;
	umsg.u.spi.wnum = cmdlen;
	umsg.u.spi.rnum = 0;
	umsg.u.spi.seq[0] = command;
	umsg.u.spi.seq[1] = addr >> 16;
	umsg.u.spi.seq[2] = addr >> 8;
	umsg.u.spi.seq[3] = addr;

	return spilib_process(h, &umsg, 0);
}

int flash_status_hid(DPFHANDLE h, uint8_t *status)
{
	int error;
	UsbMsg umsg;
	umsg.u.spi.opcode = OPCODE_SPISEL | OPCODE_SPIDES;
	umsg.u.spi.wnum = 1;
	umsg.u.spi.rnum = 1;
	umsg.u.spi.seq[0] = SPM_RDSR;

	error = spilib_process(h, &umsg, s_rbuf);
	*status = s_rbuf[0];
	return error;
}

static
int chunk_read(DPFContext *h, unsigned char *buf, ADDR offset, int len)
{
	int error;
	UsbMsg umsg;

	if (len > PKTSIZE) len = PKTSIZE;

	umsg.u.spi.opcode = OPCODE_SPISEL | OPCODE_SPIDES;
	umsg.u.spi.wnum = 4;
	umsg.u.spi.rnum = PKTSIZE;
	umsg.u.spi.seq[0] = SPM_READ;
	umsg.u.spi.seq[1] = offset >> 16;
	umsg.u.spi.seq[2] = offset >> 8;
	umsg.u.spi.seq[3] = offset;

	error = spilib_process(h, &umsg, buf);
	if (error < 0) return error;

	return len;
}


static
int flash_read_hid(DPFContext *h, unsigned char *buf, ADDR offset, int len)
{
	int n;

	while (len > 0) {
		n = chunk_read(h, buf, offset, len);
		if (n < 0) return n;
		len -= n; offset += n; buf += n;
	}
	return 0;
}

int fill_spimsg(UsbMsg *umsg, const unsigned char *buf,
	unsigned short offset, int len)
{
	int n = PKTSIZE - offset;
	if (len > n) len = n;
	memcpy(&umsg->u.spi.seq[offset], buf, len);
	umsg->u.spi.wnum = offset + len;
	return len;
}

static
int flash_writechunk_hid(DPFContext *h,
	const unsigned char *buf, ADDR offset, int len)
{
	UsbMsg umsg;
	int error;
	int n;

	if (len > PAGESIZE) len = PAGESIZE;

	umsg.u.spi.rnum = 0;
	umsg.u.spi.seq[0] = SPM_PP;
	umsg.u.spi.seq[1] = offset >> 16;
	umsg.u.spi.seq[2] = offset >> 8;
	umsg.u.spi.seq[3] = offset;

	n = fill_spimsg(&umsg, buf, 4, len);
	len -= n; buf += n; offset += n;

	umsg.u.spi.opcode = OPCODE_SPISEL;
	error = spilib_process(h, &umsg, 0);
	if (error < 0) return error;

	umsg.u.spi.opcode = 0;
	while (len > PKTSIZE) {
		n = fill_spimsg(&umsg, buf, 0, PKTSIZE);
		error = spilib_process(h, &umsg, 0);
		if (error < 0) return error;
		len -= n; buf += n; offset += n;
	}

	umsg.u.spi.opcode = OPCODE_SPIDES;
	fill_spimsg(&umsg, buf, 0, len);
	error = spilib_process(h, &umsg, 0);
	if (error < 0) return error;

	return PAGESIZE;
}

int blk_write(DPFContext *h, ADDR dst, const unsigned char *src,
	unsigned short len)
{
	static UsbMsg umsg;

	if (len > BUFSIZE) len = BUFSIZE;

	umsg.jmp[0] = JMP_LOAD & 0xff; umsg.jmp[1] = JMP_LOAD >> 8;
	memcpy(umsg.u.loader.buf, src, len);
	umsg.u.loader.offset[0] = dst;
	umsg.u.loader.offset[1] = dst >> 8;
	umsg.len = sizeof(UsbMsg) - BUFSIZE + len;

	validate(&umsg);
	transmit(h, &umsg);
	return len;
}

int mem_write_hid(DPFContext *h,
	ADDR dst, const unsigned char *src, unsigned short n)
{
	int len;

	while (n > 0) {
		len = blk_write(h, dst, src, n);
		n -= len; src += len; dst += len;
	}
	return n;
}


AccessMethods
hid_methods = {
	.mem_read  = NULL,
	.mem_write = mem_write_hid,
	.go        = go_hid,
	.bootstrap = NULL,
	.flash_probe     = flash_probe_hid,
	.flash_cmd = flash_cmd_hid,
	.flash_status = flash_status_hid,
	.flash_read = flash_read_hid,
	.flash_writechunk = flash_writechunk_hid,
	.flash_lock = NULL,
};


#ifdef TEST
int main(int argc, char **argv)
{
	FILE *f;
	int error;
	DPFHANDLE dpf;
	uint16_t startaddr = 0x1000;

	if (argc >= 2) {

		f = fopen(argv[1], "rb");

		if (!f) {
			printf("failed to open file\n");
			return -1;
		}
		if (argc == 3) {
			startaddr = strtol(argv[2], NULL, 16);
			printf("Starting at %04x\n", startaddr);
		}
	
		error = dpf_open("usb0", &dpf);
		if (error < 0) {
			printf("Failed to open DPF\n");
			return error;
		}
		if (dpf->mode == MODE_USBHID) {
			error = load(dpf, f, startaddr);
		} else {
			printf("Not in BOOTLOAD mode! Press Reset and hold MENU\n");
		}
		dpf_close(dpf);
 	} else {
		printf("Needs boot image argument\n");
	}

	if (error < 0) {
		printf("Error\n");
	}
	return error;
}
#endif

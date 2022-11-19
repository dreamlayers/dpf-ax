/** libdpf header file
 *
 * (c) 2010, 2011 <hackfin@section5.ch>
 *
 */

#include <stdint.h>
#include <libusb.h>
#include "usbuser.h"
#include "spiflash.h"

#define ADDR unsigned int

#define MODE_SG     0x00  ///< generic device mode (original)
#define MODE_USB    0x01  ///< libusb operation mode (hacked)
#define MODE_USBHID 0x02  ///< libusb HID boot mode

#define FLAG_CAN_LOCK 0x80  ///< Has the locking feature (new firmware)

enum {
	DEVERR_FILE = -16,
	DEVERR_OPEN,
	DEVERR_HEX,
	DEVERR_CHK,
	DEVERR_IO,
	DEVERR_MALLOC,
	DEVERR_TIMEOUT,
	DEVERR_UNSUPP,
};

/** The DPF context structure */
struct dpf_context;
#define DPFHANDLE struct dpf_context *

typedef struct {
	int (*mem_read)(DPFHANDLE h, unsigned char *buf, ADDR offset, int len);
	int (*mem_write)(DPFHANDLE h,
		ADDR dst, const unsigned char *buf, unsigned short n);
	int (*go)(DPFHANDLE h, ADDR start);
	int (*bootstrap)(DPFHANDLE h,
		ADDR dest, unsigned char *src, unsigned short n, ADDR start);
	int (*flash_probe)(DPFHANDLE h, unsigned char *id);
	int (*flash_cmd)(DPFHANDLE h, int command, int cmdlen, ADDR addr);
	int (*flash_status)(DPFHANDLE h, uint8_t *status);
	int (*flash_read)(DPFHANDLE h, uint8_t *buf, ADDR offset, int len);
	int (*flash_writechunk)(DPFHANDLE h,
		const uint8_t *buf, ADDR offset, int len);
	int (*flash_lock)(DPFHANDLE h, char enable);
} AccessMethods;

typedef
struct dpf_context {
	unsigned char mode;
	unsigned char flags;
	union {
		libusb_device_handle *udev;
		int fd;
	} dev;
	AccessMethods methods;
	unsigned int width;
	unsigned int height;
	int bpp;
	int proto;
	char* buff;
	unsigned char* oldpix;
	int offx;
	int offy;
} DPFContext;


/** A value proxy for the property API */
typedef struct dpf_proxy {
	union {
		short	integer;
		char   *sequence;
	} value;
	char type;
} DPFValue;

enum {
	TYPE_INTEGER,
	TYPE_STRING,
}; 

/**
 Opens the DPF device. if dev is not NULL, open device, otherwise, look for
 USB device.
 */
int dpf_open(const char *dev, DPFHANDLE *h);

/** Close DPF device */
void dpf_close(DPFHANDLE h);

/** Set color register
 * \param rgb     RGB tuple */
int dpf_setcol(DPFHANDLE h, const unsigned char *rgb);

/** Blit data to screen
 *
 * \param buf     buffer to 16 bpp RGB 565 image data
 * \param rect    rectangle tuple: [x0, y0, x1, y1]
 */

int dpf_screen_blit(DPFHANDLE h, const unsigned char *buf, short rect[4]);

/** Set property on DPF
 * \param token       Property token
 * \param value       Pointer to value
 */
int dpf_setproperty(DPFHANDLE h, int token, const DPFValue *value);

/* USB raw */

int emulate_scsi(libusb_device_handle *d, unsigned char *cmd, int cmdlen, char out,
	unsigned char *data, unsigned long block_len);

const char *dev_errstr(int err);

// Private stuff:
int dpf_usb_open(int index, libusb_device_handle **u);
int sgdev_open(const char *portname, int *fd);
int usb_rawread(libusb_device_handle *dev, unsigned char *buf, int len);
int usb_rawwrite(libusb_device_handle *dev, const unsigned char *buf, int len);
int probe(DPFHANDLE h);

////////////////////////////////////////////////////////////////////////////
// Bootloader functionality

int bl_go(DPFContext *dpf, uint16_t jmpoffset);

////////////////////////////////////////////////////////////////////////////
// FLASH stuff

// Maximum size for flash_read
#define MAX_CHUNKSIZE 0x10000

int read_mem(DPFHANDLE h, unsigned char *buf, ADDR src, unsigned short len);
int write_mem(DPFHANDLE h,
	ADDR dst, const unsigned char *buf, unsigned short len);

int load_hexfile(DPFHANDLE h, const char *hexfile);
int code_go(DPFHANDLE h, ADDR start);

int dpf_bootstrap(DPFHANDLE h,
	ADDR dst, unsigned char *src, unsigned short n, ADDR start);

int flash_cmd(DPFHANDLE h, int command, int cmdlen, ADDR addr);
int flash_probe(DPFContext *h, unsigned char *id);
int flash_erase(DPFHANDLE h, ADDR offset);
int flash_erase_full(DPFHANDLE h);
int flash_write(DPFHANDLE h, const unsigned char *buf, ADDR offset, int len);
int flash_read(DPFHANDLE h, unsigned char *buf, ADDR offset, int len);

int load_ihx(DPFHANDLE h, const char *fname, unsigned char *data, 
	unsigned int *buflen, unsigned int reloc);

int patch_sector(DPFHANDLE h,
	ADDR reloc, unsigned long addr, const char *hexfile);

////////////////////////////////////////////////////////////////////////////
/* DPF specific stuff: */

#define RGB565_0(r, g, b) \
	(( ((r) & 0xf8)		 ) | (((g) & 0xe0) >> 5))
#define RGB565_1(r, g, b) \
	(( ((g) & 0x1c) << 3 ) | (((b) & 0xf8) >> 3))

#define RGB565(r, g, b) { RGB565_0(r, g, b), RGB565_1(r, g, b) }

#define RGB565_S(r, g, b) ((RGB565_0(r, g, b) << 8) | RGB565_1(r, g, b))

int dpfcpy(ADDR dst, unsigned char *src, unsigned short n);

// int clr_screen(DPFHANDLE h, const unsigned char *col);
int write_screen(DPFHANDLE h, const unsigned char *buf, unsigned int len);


// Some internal address offsets. They may change, but so far all types
// seem to have the same
//
// w: word, <n>: length, [LE, BE]
//
// FIXME: Use packed struct later.

// FIXME: Should be 0x0020, once we have the firmware replaced
#define OFFSET_PROPS 0x3f0020   ///< w[2]:LE : Resolution X, Y




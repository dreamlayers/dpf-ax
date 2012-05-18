#include "dpf.h"
#include "print.h"
#include "global.h"
#include "spiflash.h"
#include "config.h" // logging sector config

#pragma codeseg SPIFLASH
#pragma constseg SPIFLASH

#if 1
__bit flash_detected = 0;
__idata unsigned char flashid[3];

void flash_detect() __banked
{
	BYTE n = 3;
	__idata BYTE *p = flashid;
	
	if (flash_detected)
		return;

	spi_select();
	spibuf = SPM_RDID; while (spi_busy());
	spibuf = 0; while (spi_busy()); // Dummy write
	do {
		*p++ = spibuf;
		spibuf = 0xff; // Pump SPI
		while (spi_busy());
	} while (--n);
	spi_deselect();
	flash_detected = 1;
}

#define SECTORS(n, s)  (n * (s / 1024) / 1024)

struct flash_info {
	unsigned char fcode;
	const __code char *desc;
	unsigned short MB;
}

const __code st_spi_flashes[] = {
	{ 0x00, "m25p80", SECTORS( 16, 0x10000) },
	{ 0x14, "m25p80", SECTORS( 16, 0x10000) },
	{ 0x15, "m25p16", SECTORS( 32, 0x10000) },
	{ 0x16, "m25p32", SECTORS( 64, 0x10000) },
	{ 0x17, "m25p64", SECTORS(128, 0x10000) },
};


void flash_print_size() __banked
{
	BYTE n;
	const struct flash_info __code *f = st_spi_flashes;
	n = sizeof(st_spi_flashes) / sizeof(struct flash_info);
	do {
		if (flashid[2] == f->fcode) {
			puts("Type: ");
			puts(f->desc);
			puts(" MB: ");
			print_dec(f->MB);
			return;
		}
		f++;
	} while (--n);
	puts("<undetected: ");
	print_hex(flashid[0]);
	print_hex(flashid[1]);
	print_hex(flashid[2]);
	puts(">");
}

#else

RETCODE flash_detect(__idata BYTE *codes) __banked
{
	BYTE n = 3;
	spi_select();

	spibuf = SPM_RDID; while (spi_busy());
	spibuf = 0; while (spi_busy()); // Dummy write
	do {
		*codes++ = spibuf;
		spibuf = 0xff; // Pump SPI
		while (spi_busy());
	} while (--n);
	spi_deselect();
	return 0;
}

#define SECTORS(n, s)  (n * (s / 1024) / 1024)

struct flash_info {
	unsigned char fcode;
	const __code char *desc;
	unsigned short MB;
}

const __code st_spi_flashes[] = {
	{ 0x00, "m25p80", SECTORS( 16, 0x10000) },
	{ 0x14, "m25p80", SECTORS( 16, 0x10000) },
	{ 0x15, "m25p16", SECTORS( 32, 0x10000) },
	{ 0x16, "m25p32", SECTORS( 64, 0x10000) },
	{ 0x17, "m25p64", SECTORS(128, 0x10000) },
};


void flash_print_size(__idata BYTE *codes) __banked
{
	BYTE n;
	const struct flash_info __code *f = st_spi_flashes;
	n = sizeof(st_spi_flashes) / sizeof(struct flash_info);
	do {
		if (codes[2] == f->fcode) {
			puts("Type: ");
			puts(f->desc);
			puts(" MB: ");
			print_dec(f->MB);
			return;
		}
		f++;
	} while (--n);
	puts("<undetected: ");
	print_short(codes[2]);
	puts(">");
}

#endif

static
void flash_writecmd(BYTE cmd)
{
	spi_select();
	spibuf = cmd;
	while (spi_busy());
	spi_deselect();
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

void flash_do_sequence(__pdata BYTE *seq)
{
	BYTE n;

	spi_select();

	n = *seq++;
	do {
		spibuf = *seq++;
		while (spi_busy());
	} while (--n);
	spi_deselect();
	spi_pollstatus();
}

static unsigned long g_addrptr = 0;

static
__pdata BYTE g_seq[8];

void flash_erasesector(unsigned long addr) __banked
{

	flash_writecmd(SPM_RES);
	flash_writecmd(SPM_WREN);
	flash_writecmd(SPM_WRSR);
	flash_writecmd(SPM_WREN);

	g_seq[0] = 4;
	g_seq[1] = SPM_FLASH_SE;
	g_seq[2] = addr >> 16;
	g_seq[3] = addr >> 8;
	g_seq[4] = addr;
	flash_do_sequence(g_seq);
	g_addrptr = addr;
}

BYTE flash_write(__pdata BYTE *buf, BYTE n) __banked
{

	g_seq[0] = 5;
	g_seq[1] = SPM_PP;
	while (n--) {
		flash_writecmd(SPM_WREN);
		g_seq[2] = g_addrptr >> 16;
		g_seq[3] = g_addrptr >> 8;
		g_seq[4] = g_addrptr;
		g_seq[5] = *buf++;
		flash_do_sequence(g_seq);
		g_addrptr++;
		// Break if we hit end of sector
		if ((g_addrptr & 0xffff) == 0) return (n-1);
	}
	return n;
}

#ifndef BUILD_DEVEL

__pdata BYTE config_buf[sizeof(g_config) + 3];

void save_config2flash() __banked
{
	unsigned char i;
	__idata BYTE *pi = (BYTE *) &g_config;
	__pdata BYTE *pp = &config_buf[0];

	*pp++ = 0x47; *pp++ = 0x11;  	// the magic word!
	i = sizeof(g_config);
	*pp++ = i;			// length of data
	do{
		*pp++ = *pi++;
	} while (--i);

	flash_erasesector(CONFIG_SECTOR);
	flash_write(config_buf, sizeof(config_buf));
}

#endif

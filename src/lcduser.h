/* LCD user configuration commands */

#define CMD_CS	       0x00
#define CMD_RESET      0x10
// Primitive sequences (not used)
#define CMD_WR 	       0x20
#define CMD_RD         0x30
#define CMD_A0         0x40
#define CMD_RXTX       0x50
// #define CMD_UDELAY     0x60
#define CMD_SEQCMD     0x70
// #define CMD_SEQCMDX    0x80
#define CMD_SEQDATA    0x90
// #define CMD_SEQDATAX   0xa0
#define CMD_MDELAY     0xb0
#define CMD_END        0xff

#define HIGH	   0x1
#define LOW        0x0

#define MASK_CMD   0xf0

#define CMD(x) CMD_SEQCMD | 1, _concat(R_LCD_, x)
#define WCMD(x) CMD_SEQCMD | 2, 0x00, _concat(R_LCD_, x)
#define DATA(n) CMD_SEQDATA | n
#define WDATA(x) CMD_SEQDATA | 2, \
	((( (unsigned short) (x)) >> 8) & 0xff), ((x) & 0xff)
#define DELAY(n) CMD_MDELAY, (n)


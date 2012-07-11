/** XMenu string constants */

#define XSTR_NULL     0
// Fixed strings
#define XSTR_LCD      1
#define XSTR_CREDITS  2
#define XSTR_VERSION  3
#define XSTR_WWW      4
#define XSTR_REBOOT   5
// dynamically build strings, graphics, etc.
#define XHOOK_FLASH    0x80 + 1
#define XHOOK_DEBUG    0x80 + 2
#define XHOOK_TESTPATTERN 0x80 + 3

void put_xstring(unsigned char id) __banked;

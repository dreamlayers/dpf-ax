#define BYTE unsigned char
#define RETCODE char

#include "ax206.h"
#include "lcd.h"

typedef unsigned long FlashAddr;

/* OTP routines that we use: */

#define otp_lcdwrite 0x0f25

/* Here is stuff that is used to interface with some of the
 * (not well designed) original firmware modules
 */

#ifdef USE_BUILDWIN_CRAP
#warning "Using DPF internal firmware relicts"
/* Fixed routines in external code memory that we use for testing
 * They will disappear later.
 */

#define abs_tramp_return 0x1934

// Fix up stack and return to caller
#define TRAMPOLINE_RETURN \
_asm ljmp abs_tramp_return _endasm

/* Global variables: */

data at (0x77) unsigned char lcd_cxl;
data at (0x78) unsigned char lcd_cxh;
data at (0x79) unsigned char lcd_cyl;
data at (0x7a) unsigned char lcd_cyh;
data at (0x7b) unsigned char lcd_dxl;
data at (0x7c) unsigned char lcd_dxh;
data at (0x7d) unsigned char lcd_dyl;
data at (0x7e) unsigned char lcd_dyh;
#endif

extern
__xdata unsigned char at (0x0000) g_usbfifo[0x100];
extern
__xdata unsigned char at (0x0100) g_dataram[0x100];


////////////////////////////////////////////////////////////////////////////

// LCD pin wiring:

#define P_VUSBSN   P07  // USB Voltage sense
#define BUT_MENU   P06  // MENU button
#define BUT_IRQ    P05  // UP/DOWN-Button IRQ pin
#define P_VCOMP    P04  // Reference voltage
#define P_VSENSE   P03  // Voltage sense
#define TEMP_NTC   P02  // Temperature NTC
#define FLA_PWR    P01  // Flash power
#define LCD_PWR    P00  // LCD power (for some types)

#define FLA_SCK    P17  // Flash SPI clock
#define FLA_MOSI   P16  // Serial in to flash
#define FLA_MISO   P15  // Serial out from Flash
#define LCD_A0     P14  // LCD A0 (C/D)
#define MIC        P13  // Microphone input
#define LCD_RD     P12  // LCD /Read
#define LCD_WR     P11  // LCD /Write
#define LCD_RST    P10  // LCD Reset

#define BUZZER     P26  // Buzzer driver
#define LCD_LED    P23  // LED port
#define LCD_CS     P21  // LCD Chip select
#define FLA_CS     P20  // Flash chip select

#define BUT_DOWN   P31  // DOWN button
#define BUT_UP     P32  // UP button

#define AUDIO_EN   P40  // Audio I/O Voltage enable

#define _FLA_PWR   _B_P01 // Flash VCC; 0 = on, 1 = off
#define _LCD_RST   _B_P10
#define _LCD_WR    _B_P11
#define _LCD_RD    _B_P12
#define _LCD_A0    _B_P14
#define _LCD_CS    _B_P21
#define _LCD_LED   _B_P23 // LED, 0: on 1: off
  
// Negated GPIO functionality:
#define nON  0
#define nOFF 1

#define _SPI_CS  _B_P20  // Flash chip select

#define ALL_INPUTS  0xff
#define ALL_OUTPUTS 0x00

// Some SPI macros
#define spi_select()   _SPI_CS = 0
#define spi_deselect() _SPI_CS = 1
#define spi_busy()     !_B_SPIPND


/** \file lcd.h
 *
 * Generic header file for LCD access.
 *
 */

#include "config.h"

// DO NOT CHANGE THIS STRUCT!
// All *_blit.s depend on position & length of its members!
/** Blit structure for display access */
struct blit {
	unsigned short x0;  ///< left X value of rectangle
	unsigned short x1;  ///< right X value of rectangle
	unsigned short y0;  ///< top Y value of rectangle
	unsigned short y1;  ///< bottom Y value of rectangle
};

extern struct blit __data g_blit ;

#define RGB565_0(r, g, b) \
	(( ((r) & 0xf8)      ) | (((g) & 0xe0) >> 5))
#define RGB565_1(r, g, b) \
	(( ((g) & 0x1c) << 3 ) | (((b) & 0xf8) >> 3))

#define RGB565(r, g, b) ( (RGB565_0(r, g, b) << 8) | RGB565_1(r, g, b) )

extern
unsigned char g_rgborder;

// We have different command sets:
// LCD_CMDSET_ST
// LCD_CMDSET_OT

/** LCD_* specs are the hardware properties of a LCD screen.
 * Assuming the LCD is oriented such that RGB pixel order applies.
 * Then the specified heights and widths do apply.
 * The RESOL_X and RESOL_Y values are the software resolutions, depending
 * on the screen orientation.
 */

#if defined(LCD_128x128)
#	define LCD_WIDTH  128
#	define LCD_HEIGHT 128
#elif defined (LCD_128x144)
#	define LCD_WIDTH  128L
#	define LCD_HEIGHT 144L
#elif defined (LCD_320x240)
#	define LCD_WIDTH  240L
#	define LCD_HEIGHT 320L
#endif

#ifdef LCD_CONTROLLER_ILI9163B
#include "ili9163.h"
#	ifdef DPFMODEL_pink
#	define _INTERNAL_TAG sitronix
#	else
// Use same sitronix blitting routine:
#	define ili9163_blit sitronix_blit
#	define _INTERNAL_TAG ili9163
#	endif
#	define LCD_OFFSET_X   2
#	define LCD_OFFSET_Y   3
#	define END_INCLUSIVE
#	define LCD_CMDSET_ST
#endif

#ifdef LCD_CONTROLLER_ST77XX
#include "ili9163.h"
// FIXME #include "st7637.h"
#	define _INTERNAL_TAG st77xx
#	define LCD_OFFSET_X   0
#	define LCD_OFFSET_Y   0
#	define END_INCLUSIVE
#	define LCD_CMDSET_ST
#endif

#ifdef LCD_CONTROLLER_ILI9325
#include "ili9320.h"
#	define _INTERNAL_TAG ili9325
#	define LCD_OFFSET_X   0
#	define LCD_OFFSET_Y   0
#	define END_INCLUSIVE
#	define LCD_CMDSET_ILI
#endif

#ifdef LCD_CONTROLLER_OTM3225
#include "otm3225.h"
#	define _INTERNAL_TAG otm3225
#	define LCD_OFFSET_X   0
#	define LCD_OFFSET_Y   0
#	define END_INCLUSIVE
#	define LCD_CMDSET_OT
#endif

#ifdef LCD_CONTROLLER_CUSTOM
#	define _INTERNAL_TAG custom
#	define LCD_CMDSET_CUSTOM
#endif


// This rotation is defined by the typical application. Up means: default.
// Note: the USB connector location may vary.
// This is the logical rotation of the screen. The *physical* rotation
// (see RGB order) might be just reverted.

#define ROT_UP       0   // USB connector left
#define ROT_LEFT     1   // USB connector up
#define ROT_DOWN     2   // USB connector right
#define ROT_RIGHT    3   // USB connector down

// Now we have the pixel order: These values stand for the number of
// clockwise rotations of the screen from the default (by user)
// until pixels appear in RGB order:
#define PIXELORDER(x) (((x) & 0x3) << 1)
#define RGB_UP    0
#define RGB_LEFT  1
#define RGB_DOWN  2
#define RGB_RIGHT 3

#define ROTCODE_UP     0xd0 | PIXELORDER(LCD_ORIENTATION_RGB)
#define ROTCODE_LEFT   0x60 | PIXELORDER(LCD_ORIENTATION_RGB + 1)
#define ROTCODE_DOWN   0x10 | PIXELORDER(LCD_ORIENTATION_RGB + 2)
#define ROTCODE_RIGHT  0xa0 | PIXELORDER(LCD_ORIENTATION_RGB + 3)

#define ST_ROTCODE(x)     (x & 0xf0)
#define RGBORDER(x)       (x & 0x06)

// Assign dimensions according to default rotation:

#if (DEFAULT_ORIENTATION == ROT_RIGHT) || (DEFAULT_ORIENTATION == ROT_LEFT)
#	define RESOL_X LCD_HEIGHT
#	define RESOL_Y LCD_WIDTH
#else
#	define RESOL_X LCD_WIDTH
#	define RESOL_Y LCD_HEIGHT
#endif


// Trick to resolve defines
#define CONCAT(x, y) x##_##y
#define MANGLE(x, y) CONCAT(x, y)

#define CONCAT2(x, y, z) x##_##y##_##z
#define MANGLE2(x, y, z) CONCAT2(x, y, z)

// API
#if LCD_WIDTH != LCD_HEIGHT
#	if (DEFAULT_ORIENTATION == ROT_RIGHT) || (DEFAULT_ORIENTATION == ROT_LEFT)
#		define disp_blit     MANGLE2(_INTERNAL_TAG, landscape, blit)
#	else
#		define disp_blit     MANGLE2(_INTERNAL_TAG, portrait, blit)
#	endif
#else
#	define disp_blit     MANGLE(_INTERNAL_TAG, blit)
#endif
#define init_sequence MANGLE(_INTERNAL_TAG, initseq)

void disp_blit(void);
extern __code unsigned char init_sequence[];

void lcd_orientation(unsigned char which) __banked;
void lcd_st_setrotation(unsigned char rotcode);
void lcd_ot_setorientation(unsigned char which);
void lcd_ili_setorientation(unsigned char which);
void lcd_init_by_table(__code unsigned char *table);
void lcd_custom_setorientation(unsigned char which);
void lcd_custom_init(void);

#ifdef END_INCLUSIVE
#define END_OFFSET_X (LCD_OFFSET_X - 1)
#define END_OFFSET_Y (LCD_OFFSET_Y - 1)
#endif


// Low level functions to write to LCD:
//void write_cmd(unsigned short w);
//void write_data(unsigned short w);

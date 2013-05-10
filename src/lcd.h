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


#define LCD_ORIENTATION_PORTRAIT  0
#define LCD_ORIENTATION_LANDSCAPE 1

#if LCD_WIDTH > LCD_HEIGHT
#	define LCD_ORIENTATION LCD_ORIENTATION_LANDSCAPE
#else
#	define LCD_ORIENTATION LCD_ORIENTATION_PORTRAIT
#endif

// Backlight

#if !defined(LCD_BACKLIGHT_NONE) && !defined(LCD_BACKLIGHT_VALUE) && !defined(LCD_BACKLIGHT_TABLE) && !defined(LCD_BACKLIGHT_CUSTOM)
	// "old style" (< 0.4) dpfmodel.h - translate to (hopefully) sensible 0.4 values
#	ifdef LCD_BACKLIGHT_HIGH
#	    define LCD_BACKLIGHT_TABLE
#    else
#	    define LCD_BACKLIGHT_VALUE
#    endif
#    define LCD_USER_ADJUSTABLE_BRIGHTNESS
#endif

#ifndef LCD_MAX_BRIGHTNESS_VALUE
#	define LCD_MAX_BRIGHTNESS_VALUE     21	// max brightness value
#endif
#ifndef LCD_DEFAULT_BRIGHTNESS_VALUE	// inital brightness value
#	define LCD_DEFAULT_BRIGHTNESS_VALUE LCD_MAX_BRIGHTNESS_VALUE
#endif
#ifndef LCD_TIMER1_PERIOD
#	define LCD_TIMER1_PERIOD 0x00ff
#endif

#if !defined(LCD_TIMER1_PWM_NONE) && !defined(LCD_TIMER1_PWM_P23) && !defined(LCD_TIMER1_PWM_P40)
#	define LCD_TIMER1_PWM_P23
#endif

void set_brightness(unsigned char brightness) __banked;

// Contrast

#if !defined(LCD_CONTRAST_NONE) && !defined(LCD_CONTRAST_TABLE) && !defined(LCD_CONTRAST_CUSTOM)
	// "old style" (< 0.4) dpfmodel.h - translate to (hopefully) sensible 0.4 values
#	ifdef LCD_DEFAULT_CONTRAST_VALUE
#	    define LCD_CONTRAST_CUSTOM
#	    define LCD_MAX_CONTRAST_VALUE (custom_contrasttbl_len)
#    else
#	    define LCD_CONTRAST_NONE
#	    define LCD_MAX_CONTRAST_VALUE 0
#	    define LCD_DEFAULT_CONTRAST_VALUE 0
#    endif
#endif

unsigned char get_maxcontrast() __banked;
void set_contrast(unsigned char contrast) __banked;
void lcd_set_contrast_by_table(unsigned char contrast);

//
// No longer used from version 0.4 up
// This rotation is defined by the typical application. Up means: default.
// Note: the USB connector location may vary.
// This is the logical rotation of the screen. The *physical* rotation
// (see RGB order) might be just reverted.

//#define ROT_UP       0   // USB connector left
//#define ROT_LEFT     1   // USB connector up
//#define ROT_DOWN     2   // USB connector right
//#define ROT_RIGHT    3   // USB connector down

// Now we have the pixel order: These values stand for the number of
// clockwise rotations of the screen from the default (by user)
// until pixels appear in RGB order:
//#define PIXELORDER(x) (((x) & 0x3) << 1)
//#define RGB_UP    0
//#define RGB_LEFT  1
//#define RGB_DOWN  2
//#define RGB_RIGHT 3

//#ifndef LCD_ORIENTATION_RGB
//#	define	LCD_ORIENTATION_RGB RGB_UP
//#endif

//#define ROTCODE_UP     0xd0 | PIXELORDER(LCD_ORIENTATION_RGB)
//#define ROTCODE_LEFT   0x60 | PIXELORDER(LCD_ORIENTATION_RGB + 1)
//#define ROTCODE_DOWN   0x10 | PIXELORDER(LCD_ORIENTATION_RGB + 2)
//#define ROTCODE_RIGHT  0xa0 | PIXELORDER(LCD_ORIENTATION_RGB + 3)

//#define ST_ROTCODE(x)     (x & 0xf0)
//#define RGBORDER(x)       (x & 0x06)

//#ifndef DEFAULT_ORIENTATION
//#	define DEFAULT_ORIENTATION ROT_LEFT
//#endif

#define RESOL_X LCD_WIDTH
#define RESOL_Y LCD_HEIGHT

// Trick to resolve defines
#define CONCAT(x, y) x##_##y
#define MANGLE(x, y) CONCAT(x, y)

#define CONCAT2(x, y, z) x##_##y##_##z
#define MANGLE2(x, y, z) CONCAT2(x, y, z)

// API
#define _INTERNAL_TAG custom

#if LCD_WIDTH != LCD_HEIGHT
#	if LCD_ORIENTATION == LCD_ORIENTATION_LANDSCAPE
#		define disp_blit     MANGLE2(_INTERNAL_TAG, landscape, blit)
#	else
#		define disp_blit     MANGLE2(_INTERNAL_TAG, portrait, blit)
#	endif
#else
#	define disp_blit     MANGLE(_INTERNAL_TAG, blit)
#endif
#define init_sequence MANGLE(_INTERNAL_TAG, initseq)

unsigned char set_backlight(unsigned char brightness) __banked;

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

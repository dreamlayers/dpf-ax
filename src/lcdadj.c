/* LCD brightness / contrast adjust */

#include "global.h"
#include "dpf.h"
#include "lcd.h"
#include "print.h"
#include "utils.h"

#pragma codeseg LCDAUX
#pragma constseg LCDAUX

#ifdef LCD_BACKLIGHT_TABLE
extern __code unsigned char custom_backlighttbl_len;
extern __code unsigned char custom_backlighttbl[];
extern __code unsigned char custom_backlighttbl2_len;
extern __code unsigned char custom_backlighttbl2[];
#endif

#ifdef LCD_BACKLIGHT_CUSTOM
extern void lcd_custom_setbacklight(unsigned char brightness);

void lcd_custom_setbacklight_proxy(unsigned char brightness)
{
	brightness;
	_asm
	push	ar3
	mov	r3,dpl
	lcall	_lcd_custom_setbacklight
	pop	ar3
	_endasm;
}
#endif


void _set_brightness(BYTE brightness)
{
#if defined(LCD_BACKLIGHT_CUSTOM)
	// custom backlight control
	if (brightness > LCD_MAX_BRIGHTNESS_VALUE)
	    brightness = LCD_MAX_BRIGHTNESS_VALUE;
        lcd_custom_setbacklight_proxy(brightness);
#elif defined( LCD_BACKLIGHT_VALUE)
	// backlight control by value
	unsigned int pwm;

	if (brightness == 0)
	    // 0 = backlight off request from timer1_config
	    // nothing to do for _VALUE
	    // timer1_config alread disabled the timer.
	    return;

	if (brightness > LCD_MAX_BRIGHTNESS_VALUE)
	    brightness = LCD_MAX_BRIGHTNESS_VALUE;
	brightness = LCD_MAX_BRIGHTNESS_VALUE - brightness;
	pwm = (LCD_TIMER1_PERIOD / LCD_MAX_BRIGHTNESS_VALUE) * brightness;
	tmr1pwml = pwm & 0xFF;
	tmr1pwmh = pwm >> 8;
	
#elif defined(LCD_BACKLIGHT_TABLE)
	// backlight control by table
	unsigned int per;
	unsigned int pwm;

	if (brightness == 0)
	    // 0 = backlight off request from timer1_config
	    // nothing to do for _TABLE
	    // timer1_config alread disabled the timer.
	    return;

	if (brightness >= custom_backlighttbl_len)
		brightness = custom_backlighttbl_len - 1;
	if (custom_backlighttbl2_len > 0)
	{
	// FREQ + PWM adjustment
		per = custom_backlighttbl[brightness];
		pwm = custom_backlighttbl2[brightness];
	}
	else
	{
	// Fixed FREQ + PWM adjustment
		per = LCD_TIMER1_PERIOD;
		pwm = LCD_TIMER1_PERIOD - custom_backlighttbl[brightness];
	}
	if (pcon & (2 << TMRCSEL_SHFT)){	// 48 MHz ? 
		per *= 2;
		pwm *= 2;
	}
	//} else if ((pcon & TMRCSEL) == (1 << TMRCSEL_SHFT)){	// 12 MHz ? 
	//	per = ++per / 2;
	//	pwm = ++pwm / 2;
	//}


	tmr1perl = per & 0xFF;
	tmr1perh = per >> 8;
	tmr1pwml = pwm & 0xFF;
	tmr1pwmh = pwm >> 8;

	tmr1cntl = 0;
	tmr1cnth = 0;

#else 	// must be LCD_BACKLIGHT_NONE or is undefined
	// --> no backlight control
	brightness;
	return;
#endif
}

void set_brightness(BYTE brightness) __banked
{
	_set_brightness(brightness);
}


#ifdef LCD_CONTRAST_TABLE
extern __code unsigned char custom_contrasttbl_len;
#endif

#if defined(LCD_CONTRAST_CUSTOM)
extern void lcd_custom_setcontrast(unsigned char contrast);

void lcd_custom_setcontrast_proxy(unsigned char contrast)
{
	contrast;
	_asm
	push	ar3
	mov	r3,dpl
	lcall	_lcd_custom_setcontrast
	pop	ar3
	_endasm;
}
#endif

extern __code unsigned char custom_contrasttbl_len;

#if defined(LCD_HAS_CONTRASTTABLE)
extern __code unsigned char custom_contrasttbl2_len;
extern __code unsigned char custom_contrasttbl2_offsets_len;
extern __code unsigned char custom_contrasttbl[];
extern __code unsigned char custom_contrasttbl2[];
extern __code unsigned char custom_contrasttbl2_offsets[];

void lcd_set_contrast_by_table(unsigned char contrast)
{
	unsigned char i;
	unsigned char v;
	__xdata unsigned char *p;
	if (contrast > 0 && contrast <= custom_contrasttbl_len)
		contrast--;
	else
		contrast = 0;
	contrast *= custom_contrasttbl2_offsets_len;

	for (i = 0; i < custom_contrasttbl2_offsets_len; i++)
	{
		v = custom_contrasttbl[contrast++];

		p = (unsigned char __xdata *) &(custom_contrasttbl2 + custom_contrasttbl2_offsets[i] - 0x800);
		*p = v;
	}
	lcd_init_by_table(custom_contrasttbl2);
}
#endif

#if !defined(LCD_CONTRAST_NONE)
unsigned char get_maxcontrast() __banked
{
	return LCD_MAX_CONTRAST_VALUE;
}
#endif

void set_contrast(unsigned char contrast) __banked
{
	if (contrast == 0) contrast++;

#if defined(LCD_CONTRAST_CUSTOM)
	if (contrast > LCD_MAX_CONTRAST_VALUE)
		contrast = LCD_MAX_CONTRAST_VALUE;
	lcd_custom_setcontrast_proxy(contrast);

#elif defined(LCD_CONTRAST_TABLE)
	lcd_set_contrast_by_table(contrast);

#else 	// must be LCD_CONTRAST_NONE or is undefined
	// --> no contrast control
#endif
	g_config.contrast = contrast;
}

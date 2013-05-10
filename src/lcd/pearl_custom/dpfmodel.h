//
// Type: pearl_custom
// Comment: Custom (flash-dump based) version of pearl
//
// Status: supported
// Credit: By superelchi
// Date: 2012-07-28
// Url: http://www.pearl.de/a-HPM1184-5618.shtml
// 
// The signature identify.py detected (OpenWin, Init, Backlight, Contrast)
// CRC: 0x1dac2801, 0xba995aef, 2, 1
//
// 
#define LCD_CONTROLLER_CUSTOM
#define LCD_WIDTH  320L
#define LCD_HEIGHT 240L

#define LCD_DEFAULT_BRIGHTNESS_VALUE 10
#define LCD_USER_ADJUSTABLE_BRIGHTNESS   //brightness adjustable in menu
//#define LCD_MAX_BRIGHTNESS_VALUE 21      //undefine & change, if max != 21

#define LCD_DEFAULT_CONTRAST_VALUE 6
#define LCD_MAX_CONTRAST_VALUE 6
//#define LCD_USER_ADJUSTABLE_CONTRAST     //undefine to make contrast adjustable in menu

//NOTE: - Set LCD_TIMER1_PERIOD = 0 if no backlight pwm signal is required
//        (timer will be stopped, no pwm output on P2.3)
//      - Undefine if _custom_backlighttbl2 used or lcd_custom_setbacklight initializes period
//        (timer pwm output on P2.3, period not initialized)
//
#define LCD_TIMER1_PERIOD 0x0fff  //custom value, standard table value = 0x00ff

//NOTE: uncomment only one of the following 4 values!
//#define LCD_BACKLIGHT_NONE
//#define LCD_BACKLIGHT_VALUE
//#define LCD_BACKLIGHT_TABLE
#define LCD_BACKLIGHT_CUSTOM

// NOTE: uncomment only one of the following 3 values!
//#define LCD_CONTRAST_NONE
#define LCD_CONTRAST_TABLE
//#define LCD_CONTRAST_CUSTOM

//
// Type: zipp_dp605
// Comment: Sold in Australia by Big W / Woolworths.
//
// Status: supported
// Credit: By superelchi (thx CaptainCheeks)
// Date: 2013-03-05
//
// The signature identify.py detected (OpenWin, Init, Backlight, Contrast)
// CRC: 0xd5f769e0, 0x2416f0db, 2, 2
//
//
#define LCD_WIDTH  128L
#define LCD_HEIGHT 128L
#define LCD_CONTROLLER_CUSTOM

#define LCD_DEFAULT_BRIGHTNESS_VALUE 18
#define LCD_USER_ADJUSTABLE_BRIGHTNESS   //brightness adjustable in menu
//#define LCD_MAX_BRIGHTNESS_VALUE 21      //undefine & change, if max != 21

#define LCD_DEFAULT_CONTRAST_VALUE 7
#define LCD_MAX_CONTRAST_VALUE 10        // NOTE from identify.py: value not detected
#define LCD_USER_ADJUSTABLE_CONTRAST     //undefine to make contrast adjustable in menu

//NOTE: - Set LCD_TIMER1_PERIOD = 0 if no backlight pwm signal is required
//        (pwm output on P2.3 disabled, timer1 stopped)
//      - Undefine if _custom_backlighttbl2 used
//        (pwm output on P2.3, timer running, period will be initialized by table)
//      - Undefine if LCD_CONTRAST_CUSTOM
//        (pwm output on P2.3, timer running, period will be initialized by _lcd_custom_setbacklight)
//
//#define LCD_TIMER1_PERIOD 0x00ff  //custom value, standard table value = 0x0000

//NOTE: uncomment only one of the following 4 values!
//#define LCD_BACKLIGHT_NONE
//#define LCD_BACKLIGHT_VALUE
//#define LCD_BACKLIGHT_TABLE
#define LCD_BACKLIGHT_CUSTOM

// NOTE: uncomment only one of the following 3 values!
//#define LCD_CONTRAST_NONE
//#define LCD_CONTRAST_TABLE
#define LCD_CONTRAST_CUSTOM

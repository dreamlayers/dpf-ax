//
// Type: coby_dp180
//
// Status: supported
// Credit: By superelchi (thx johni)
// Date: 2013-03-02
//
// Url: http://www.cobyusa.com/?p=prod&prod_num_id=411&pcat_id=4001
//
// The signature identify.py detected (OpenWin, Init, Backlight, Contrast)
// CRC: 0x20f68ffb, 0x252e7b8e, 2, 2
//
//
#define LCD_WIDTH  160L
#define LCD_HEIGHT 128L
#define LCD_CONTROLLER_CUSTOM

#define LCD_DEFAULT_BRIGHTNESS_VALUE 8
#define LCD_USER_ADJUSTABLE_BRIGHTNESS   //brightness adjustable in menu
#define LCD_MAX_BRIGHTNESS_VALUE 16      //undefine & change, if max != 21

#define LCD_DEFAULT_CONTRAST_VALUE 8
#define LCD_MAX_CONTRAST_VALUE 8         // NOTE from identify.py: value not detected
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

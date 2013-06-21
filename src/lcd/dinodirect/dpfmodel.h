//
// Type: dinodirect
//
// Status: supported
// Credit: By superelchi, thx kodi
// Date: 2013-05-31
//
// Url: http://www.dinodirect.com/2-4-LCD-16MB-Digital-Photo-Frame-White.html
//
// The signature identify.py detected (OpenWin, Init, Backlight, Contrast)
// CRC: 0x174c10b9, 0x79ef9665, 2, 1
//
//
#define LCD_CONTROLLER_CUSTOM
#define LCD_WIDTH  320L
#define LCD_HEIGHT 240L

#define LCD_DEFAULT_BRIGHTNESS_VALUE 20
#define LCD_USER_ADJUSTABLE_BRIGHTNESS   //brightness adjustable in menu
//#define LCD_MAX_BRIGHTNESS_VALUE 21      //undefine & change, if max != 21

#define LCD_DEFAULT_CONTRAST_VALUE 6
#define LCD_MAX_CONTRAST_VALUE 7
//#define LCD_USER_ADJUSTABLE_CONTRAST     //undefine to make contrast adjustable in menu

//NOTE: PWM_NONE - pwm output disabled, timer1 stopped
//      PWM_P23  - pwm output on P2.3, timer1 running
//      PWM_P40  - pwm output on P4.0, timer1 running
//#define LCD_TIMER1_PWM_NONE
#define LCD_TIMER1_PWM_P23
//#define LCD_TIMER1_PWM_P40

//NOTE: Undefine this if there are backlight-flashes on power-on
//      or if backlight stays on after menu|off
//#define LCD_P23_ACTIVE_HIGH

//NOTE: Init value for tmr1per
//      Undefine if _custom_backlighttbl2 used or lcd_custom_setbacklight initializes period
//      (pwm output on P2.3 or P4.0, timer running, period not initialized)
#define LCD_TIMER1_PERIOD 0x00ff  //custom value, standard table value = 0x00ff

//NOTE: uncomment only one of the following 4 values!
//#define LCD_BACKLIGHT_NONE
#define LCD_BACKLIGHT_VALUE
//#define LCD_BACKLIGHT_TABLE
//#define LCD_BACKLIGHT_CUSTOM

// NOTE: uncomment only one of the following 3 values!
//#define LCD_CONTRAST_NONE
#define LCD_CONTRAST_TABLE
//#define LCD_CONTRAST_CUSTOM

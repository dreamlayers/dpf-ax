// Type: dx21334a
//
// Status: supported
// Credit: By superelchi
// Url: http://www.dealextreme.com/p/2-4-lcd-desktop-digital-photo-frame-and-calendar-27-picture-memory-storage-21334
// 
// The signature identify.py detected (OpenWin, Init, Backlight, Contrast)
// CRC: 0xe7bb71fc, 0x53c2379b, 1, 1
//
//
#define LCD_CONTROLLER_CUSTOM
#define LCD_WIDTH  320L
#define LCD_HEIGHT 240L

#define LCD_DEFAULT_BRIGHTNESS_VALUE 11
#define LCD_USER_ADJUSTABLE_BRIGHTNESS   //brightness adjustable in menu
//#define LCD_MAX_BRIGHTNESS_VALUE 21      //undefine & change, if max != 21

#define LCD_DEFAULT_CONTRAST_VALUE 5
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

//#define LCD_TIMER1_PERIOD ---  //_custom_backlighttbl2 used - LCD_TIMER1_PERIOD not used!

//NOTE: uncomment only one of the following 4 values!
//#define LCD_BACKLIGHT_NONE
//#define LCD_BACKLIGHT_VALUE
#define LCD_BACKLIGHT_TABLE
//#define LCD_BACKLIGHT_CUSTOM

// NOTE: uncomment only one of the following 3 values!
//#define LCD_CONTRAST_NONE
#define LCD_CONTRAST_TABLE
//#define LCD_CONTRAST_CUSTOM

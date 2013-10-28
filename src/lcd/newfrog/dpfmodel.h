//
// Type: newfrog
// Comment: No Brightness control. Only on/off supported.
//
// Status: supported
// Credit: By superelchi, thx LUISE
// Date: 2013-06-24
//
// Url: http://www.newfrog.com/p/2-4-inch-usb-digital-time-day-calendar-alarm-clock-photo-frame-viewer-pof16-new-25642.html
//
// The signature identify.py detected (OpenWin, Init, Backlight, Contrast)
// CRC: 0x96f0b477, 0xb0a28fd7, 0, 0
//
//
#define LCD_CONTROLLER_CUSTOM
#define LCD_WIDTH  320L
#define LCD_HEIGHT 240L

//
//NOTE from identify.py: no detectable LCD_Backlight routine!
//
//#define LCD_DEFAULT_BRIGHTNESS_VALUE 11
//#define LCD_USER_ADJUSTABLE_BRIGHTNESS   //brightness adjustable in menu
//#define LCD_MAX_BRIGHTNESS_VALUE 21      //undefine & change, if max != 21

//
// NOTE from identify.py: no detectable LCD_Contrast routine!
//
//#define LCD_DEFAULT_CONTRAST_VALUE 5
//#define LCD_MAX_CONTRAST_VALUE 7
//#define LCD_USER_ADJUSTABLE_CONTRAST     //undefine to make contrast adjustable in menu

//NOTE: PWM_NONE - pwm output disabled, timer1 stopped
//      PWM_P23  - pwm output on P2.3, timer1 running
//      PWM_P40  - pwm output on P4.0, timer1 running

#define LCD_TIMER1_PWM_NONE
//#define LCD_TIMER1_PWM_P23
//#define LCD_TIMER1_PWM_P40

//NOTE: Undefine this if there are backlight-flashes on power-on
//      or if backlight stays on after menu|off
//#define LCD_P23_ACTIVE_HIGH

//NOTE: Init value for tmr1per
//      Undefine if _custom_backlighttbl2 used or lcd_custom_setbacklight initializes period
//      (pwm output on P2.3 or P4.0, timer running, period not initialized)
#define LCD_TIMER1_PERIOD 0x00ff  //standard table value

//NOTE: uncomment only one of the following 4 values!
//#define LCD_BACKLIGHT_NONE                // NOTE from identify.py: have a look at poweroninit.d52!
//#define LCD_BACKLIGHT_VALUE
//#define LCD_BACKLIGHT_TABLE
#define LCD_BACKLIGHT_CUSTOM

// NOTE: uncomment only one of the following 3 values!
#define LCD_CONTRAST_NONE                // NOTE from identify.py: have a look at poweroninit.d52!
//#define LCD_CONTRAST_TABLE
//#define LCD_CONTRAST_CUSTOM

// NOTE: uncomment to swap the buttons for up/down menu navigation
//#define LCD_LRBUTTON_SWAP

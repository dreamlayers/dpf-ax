// Type: dx27893
// Comment: Works with dx #27893 (white) & #27894 (blue)
//
// Status: supported
// Credit: By superelchi
// Date: 2012-07-28
// Url: http://www.dealextreme.com/p/1-5-lcd-rechargeable-digital-usb-photo-frame-keychain-white-107-picture-memory-storage-27893
// Url: http://www.dealextreme.com/p/1-5-lcd-rechargeable-digital-usb-photo-frame-keychain-blue-107-picture-memory-storage-27894
// Url: http://dx.com/p/1-5-lcd-rechargeable-digital-usb-photo-frame-keychain-with-8m-memory-for-140-photos-silver-33624
// Url: http://www.amazon.de/Technaxx-Keypix-Digitaler-Fotorahmen-interner/dp/B0013SJV4O
// Url: http://www.7dayshop.com/7dayshop-pebble-digital-photo-frame-1-5-tft-keyring-version-silver-irresistible-price
// 
// The signature identify.py detected (OpenWin, Init, Backlight, Contrast)
// CRC: 0xd5efd5d9, 0x4d507697, 1, 1
// CRC: 0xd5efd5d9, 0xa483a45c, 1, 1
//
//
#define LCD_CONTROLLER_CUSTOM
#define LCD_WIDTH  128L
#define LCD_HEIGHT 128L

#define LCD_DEFAULT_BRIGHTNESS_VALUE 20
#define LCD_USER_ADJUSTABLE_BRIGHTNESS   //brightness adjustable in menu
//#define LCD_MAX_BRIGHTNESS_VALUE 21      //undefine & change, if max != 21

#define LCD_DEFAULT_CONTRAST_VALUE 5
#define LCD_MAX_CONTRAST_VALUE 7
#define LCD_USER_ADJUSTABLE_CONTRAST     //undefine to make contrast adjustable in menu

//NOTE: PWM_NONE - pwm output disabled, timer1 stopped
//      PWM_P23  - pwm output on P2.3, timer1 running
//      PWM_P40  - pwm output on P4.0, timer1 running
//#define LCD_TIMER1_PWM_NONE
#define LCD_TIMER1_PWM_P23
//#define LCD_TIMER1_PWM_P40

//NOTE: Init value for tmr1per
//      Undefine if _custom_backlighttbl2 used or lcd_custom_setbacklight initializes period
//      (pwm output on P2.3 or P4.0, timer running, period not initialized)
#define LCD_TIMER1_PERIOD 0x00ff  //standard table value

//NOTE: uncomment only one of the following 4 values!
//#define LCD_BACKLIGHT_NONE
#define LCD_BACKLIGHT_VALUE             // Changed from _TABLE to _VALUE. BacklightTable has suspicous values.
//#define LCD_BACKLIGHT_TABLE
//#define LCD_BACKLIGHT_CUSTOM

// NOTE: uncomment only one of the following 3 values!
//#define LCD_CONTRAST_NONE
#define LCD_CONTRAST_TABLE
//#define LCD_CONTRAST_CUSTOM

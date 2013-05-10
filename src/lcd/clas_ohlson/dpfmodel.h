//
// Type: clas_ohlson
//
// Status: supported
// Credit: By superelchi, thx BufferFramed
// Date: 2012-12-05
// Url: http://www.clasohlson.com/fi/Avaimenper%C3%A4,-jossa-digitaalinen-valokuvakehys/Pr383925000
// 
// The CRCs identify.py detected - in the form "OpenWin CRC", "Init(Tbl) CRC"
// CRC: 0x26290621, 0xf5fe6525
//
// 
#define LCD_WIDTH  128L
#define LCD_HEIGHT 128L
#define LCD_CONTROLLER_CUSTOM
// Original fw has broken setcontrast routine - fixed in lcdinit.
#define LCD_DEFAULT_CONTRAST_VALUE 4
#define LCD_DEFAULT_BRIGHTNESS_VALUE 20
// Contrast adjustable in menu
#define LCD_USER_ADJUSTABLE_CONTRAST 1

//
// Type: carrefour_bfv151
//
// Status: supported
// Credit: By superelchi, thx assas1
// Date: 2012-11-02
// Url: http://www.mjshk.com/products/Digital%20Photo%20Frame/page/DPF815A_Max.htm
// Url: http://www.prixing.fr/products/bfv151-1-5-pouce-cadre-photo-porte-cles-carrefour-3606605023771
// 
// The CRCs identify.py detected - in the form "OpenWin CRC", "Init(Tbl) CRC"
// CRC: 0xbe1c4ae1, 0xd088e497
//
// 
#define LCD_WIDTH  128L
#define LCD_HEIGHT 128L
#define LCD_CONTROLLER_CUSTOM
// Original fw has broken setcontrast routine - fixed in lcdinit.
#define LCD_DEFAULT_CONTRAST_VALUE 4
#define LCD_DEFAULT_BRIGHTNESS_VALUE 18
// Contrast adjustable in menu
#define LCD_USER_ADJUSTABLE_CONTRAST 1

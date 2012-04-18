/* Attempt to create a configuration file for various DPF models */

#define LOG_SECTOR 0x3e0000

#if defined (DPFMODEL_white)
#	define LCD_128x128
#	define LCD_CONTROLLER_ILI9163B
#	define LCD_ORIENTATION_RGB RGB_DOWN
#	define BUTTON_SWAP
#	define DEFAULT_ORIENTATION ROT_UP
#elif defined (DPFMODEL_pink)
#	define LCD_128x128
#	define LCD_CONTROLLER_ILI9163B
#	define LCD_ORIENTATION_RGB RGB_DOWN
#	define BUTTON_SWAP
#	define DEFAULT_ORIENTATION ROT_UP
#elif defined (DPFMODEL_blue)
#	define LCD_128x128
#	define LCD_CONTROLLER_ST77XX
#	define LCD_ORIENTATION_RGB RGB_DOWN
#	define DEFAULT_ORIENTATION ROT_UP
#elif defined (DPFMODEL_pearl)
#	define LCD_320x240
#	define LCD_CONTROLLER_OTM3225
#	define LCD_ORIENTATION_RGB RGB_UP
#	define DEFAULT_ORIENTATION ROT_DOWN
#elif defined (DPFMODEL_pearl_landscape)
#	define LCD_320x240
#	define LCD_CONTROLLER_OTM3225
#	define LCD_ORIENTATION_RGB RGB_UP
#	define DEFAULT_ORIENTATION ROT_LEFT // USB is left
#elif defined (DPFMODEL_focal)
#	define LCD_320x240
#	define LCD_CONTROLLER_ILI9325
#	define LCD_ORIENTATION_RGB RGB_UP
#	define DEFAULT_ORIENTATION ROT_DOWN
#elif defined (DPFMODEL_focal_landscape)
#	define LCD_320x240
#	define LCD_CONTROLLER_ILI9325
#	define LCD_ORIENTATION_RGB RGB_UP
#	define DEFAULT_ORIENTATION ROT_LEFT
#elif defined (DPFMODEL_linkdelight)
#	define LCD_320x240
#	define LCD_CONTROLLER_CUSTOM
#	define LCD_ORIENTATION_RGB RGB_UP
#	define DEFAULT_ORIENTATION ROT_LEFT
#	define LCD_OFFSET_X   0
#	define LCD_OFFSET_Y   0
//#	define END_INCLUSIVE
// <<<< INSERT NEW DPFS BEFORE THIS LINE <<<<
#else
#error	"Unrecognized DPF TYPE specification"
#endif

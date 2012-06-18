/** Some global flags/variables */

// This is normally manipulated by the IRQ routines
// extern unsigned char __data at (0x20)
	// G_flags;

extern
struct term {
	unsigned char x;
	unsigned char y;
	unsigned char num_cols;
	unsigned char num_lines;
	unsigned short offset_y;
} __data g_term;

extern unsigned short g_fgcol;
extern unsigned short g_bgcol;

extern
struct lcd {
	unsigned char orientation;      ///< Orientation index
	unsigned char brightness;
} __idata g_lcd;

extern
struct flixcontext {
	unsigned char mode;
} __idata g_flix;

extern
__bit g_refresh;       // Refresh default screen

/* USB stuff */

extern volatile
__bit g_usb_active;

extern volatile
__bit g_usb_event;     // USB event

extern
struct _usbcontext {
	unsigned char mode;
	unsigned char size;
	unsigned char config;
	unsigned char interface;
	unsigned char __code *desc;
} __data g_usb;

/* Shared stuff (strings, etc.)  */

#define OVLAREA __code

/* Menu stuff */

#ifndef BUILD_DEVEL

  #define MENUAREA __code
  extern const OVLAREA char g_usbserial[];

#else //BUILD_DEVEL

  typedef struct menu {
	const char text[12];
	char pos[2];
	unsigned char type_flags;
	__idata void *prop;
	__code char (*handler)(unsigned char evt) __reentrant;
  } Menu;

  /* Menu action codes */

  enum {
	M_SHOW,
	M_DIRECT_SHORT,
	M_DIRECT_BYTE,
	M_HANDLER,
	M_EXIT
  };

  #define TYPE_MASK  0x0f
  #define F_ACTION   0x20
  #define F_EDITABLE 0x10

  // Menu layout lines:
  #define L_MENU   0
  #define L_EDITOR 3

  extern const OVLAREA Menu g_menu[];
  extern const OVLAREA char g_fakeleddesc[];
  extern const OVLAREA char g_rebootmsg[];
  extern const OVLAREA char g_menusize;

#endif //BUILD_DEVEL

/* Some more global flags */

extern volatile
__bit g_wkup;

extern volatile
__bit g_alarm;    // Wakeup, if set (alarm)

extern
__bit g_log;      // Log, if set

/* Button and pin I/O events and status */

extern
struct inputs {
/** The current I/O value, sampled at timer1 frequency */
	volatile unsigned char level;
/** Event field, I/O changed (debounced) */
	volatile unsigned char evt;
} __idata g_button;


/* Splash screen defines */
#define SPLASH_BLACK 	0x00
#define SPLASH_WHITE 	0x01
#define SPLASH_IMAGE 	0x02

/* Non-volatile config, stored in flash */
extern
struct config_flash {
	unsigned char splash;
	unsigned char brightness;
	unsigned char usbserial;
} __idata g_config;

/* Data logging & buffering */

extern
__pdata unsigned char g_databuf[0x40];
extern unsigned char g_datacount;

/* DEBUG_DEV support */

#ifdef DEBUG_DEV
#define DEBUGBUF_SIZE  0x20
extern __pdata unsigned char g_debugbuf[];
extern unsigned char g_debugcount;

#define DEBUG_CHAR(c) 	 if (g_debugcount < DEBUGBUF_SIZE - 1) \
				g_debugbuf[g_debugcount++] = c & 0x7F; 
#define DEBUG_HEX(c, h)  if (g_debugcount < DEBUGBUF_SIZE - 3)   \
				*(g_debugbuf+g_debugcount) = c | 0x80;   \
				*(g_debugbuf+g_debugcount+1) = h & 0xFF; \
				*(g_debugbuf+g_debugcount+2) = h >> 8;   \
				g_debugcount += 3;
#define DEBUG_CLEARBUF()  g_debugcount = 0;
#endif

// Event and timer counter enums:

enum {
	TIMER = 0,
	SECONDS,
	MONITOR,
	LOG
};

extern volatile
unsigned short __data g_count[4];

extern volatile
unsigned short __data g_adc;

extern volatile
unsigned short __data g_retcode;

extern volatile
unsigned short __data g_register[4];

// Pseudo status LEDs:

extern volatile
unsigned char __data g_fakeled;
#define LED_USB  0x01
#define LED_PWR  0x02
#define LED_EP0A 0x04
#define LED_STL  0x08
#define LED_LOG  0x10
#define LED_ERR  0x20
#define LED_ACK  0x40
#define LED_ALV  0x80


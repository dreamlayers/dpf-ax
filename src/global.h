/** Some global flags/variables */

// This is normally manipulated by the IRQ routines
// extern unsigned char __data at (0x20)
	// G_flags;

extern
struct term {
	unsigned char x;
	unsigned char y;
	unsigned short col;
	unsigned short bgcol;
	unsigned short offset_y;
} __data g_term;

extern
struct lcd {
	unsigned char orientation;      ///< Orientation index
	unsigned char brightness;
} __idata g_lcd;

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

/* Menu stuff */

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

#define OVLAREA __code

// Menu layout lines:
#define L_MENU   0
#define L_EDITOR 3

extern const OVLAREA Menu g_menu[];
extern const OVLAREA char g_fakeleddesc[];
extern const OVLAREA char g_rebootmsg[];
extern const OVLAREA char g_menusize;

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

/* Data logging & buffering */

extern
__pdata unsigned char g_databuf[0x40];
extern
unsigned char g_datacount;
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


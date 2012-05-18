/** XMenu flags/variables */

typedef struct menuitem {
	MENUAREA const char *text;
	const unsigned char type_flags;
	const unsigned char param;
	__idata const void *prop;
	__code const char (*handler)(unsigned char evt) __reentrant;
	const unsigned char xstr_id;
} MItem;

typedef struct xmenu {
    MENUAREA const char *title;
    const unsigned char line;
    const char num_items;
    MENUAREA const MItem* item;
} XMenu;

/* Menus */
#define MENU_MAIN	0
#define MENU_SPLASH	1
#define MENU_BACKLIGHT	2
#define MENU_INFO	3
#define MENU_SETUP	4
    
/* Menu action codes */

enum {
	M_SHOW,		// do not change, must be first!
	M_DIRECT_BYTE,
	M_IDATA_BYTE,
	M_HANDLER,
	M_EXIT
};

#define TYPE_MASK     0x0f
#define F_MENUSWITCH  0x40
#define F_ACTION      0x20
//#define F_EDITABLE 0x10

#define L_EDITOR 8

/* display_menu params */

#define SHOW_MENU   0
#define UPDATE_MENU 1

extern const MENUAREA XMenu g_mainmenu;
extern const MENUAREA XMenu g_splashmenu;
extern const MENUAREA XMenu g_backlightmenu;
extern const MENUAREA XMenu g_infomenu;
extern const MENUAREA XMenu g_setupmenu;
extern MENUAREA const char infostr_version[];

#define MENU_PTR g_mainmenu

extern const OVLAREA char g_rebootmsg[];

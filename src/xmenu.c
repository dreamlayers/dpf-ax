/* Menu handling */

#include "global.h"
#include "dpf.h"
#include "print.h"
#include "utils.h"
#include "xmenu.h"
#include "xmenustr.h"

#pragma codeseg MENU
#pragma constseg MENU

__bit g_refresh = 0;
char curitem = 0, previtem = 0;
char firstitem_line;

static void config2flash()
{
	save_config2flash();
}

static void font_black() { term_selfont(FONT_LARGE_BLACK); }
static void font_white() { term_selfont(FONT_LARGE_WHITE); }

static void cls()
{
	clrscreen(WHITE);
	font_black();
}

char hdl_brightness(BYTE evt) __reentrant
{
	evt;
	if (g_lcd.brightness < 1) g_lcd.brightness = 1;
	if (g_lcd.brightness > PWM_PERIOD) g_lcd.brightness = PWM_PERIOD;
	g_config.brightness = g_lcd.brightness;

	tmr1cntl = tmr1cnth = 0;
	tmr1perl = PWM_PERIOD; tmr1perh = 0x00;
	tmr1pwml = (~g_lcd.brightness & 0x7);
	tmr1pwmh = 0;
	if (evt == KEY_MENU)
		config2flash();
	return S_EDIT;
}

char hdl_off(BYTE evt) __reentrant
{
	evt;
	turn_off();
	return S_RUN;
}

char hdl_reboot(BYTE evt) __reentrant
{
	evt;
	cls();
	GOTOXY(1, NUM_LINES_LARGE / 2);
	puts(g_rebootmsg);
	// Shut down nicely after a blink using the watchdog
	wdtcon &= ~WDTPSR;
	wdtcon |= WDTPND | WDTEN | 6; // reboot after 0.512 seconds
	pcon |= SELRTC; // Switch to RTC clock source
	_asm nop nop nop _endasm;
	_B_T3ON = 0;   // Power down

	return -1; // We never get here. If we do, we failed.
}

char hdl_config(BYTE evt) __reentrant
{
	if (evt == EVT_EDIT)
		config2flash();
	return evt;
}

#if NUM_COLS_LARGE > 16
#define START_COL 1
#else
#define START_COL 0
#endif

#if NUM_LINES_LARGE > 9
#define INFO_OFFSET 1
#else
#define INFO_OFFSET 1
#endif

void display_menuitem(char item)
{
    MENUAREA MItem *m = MENU_PTR.item + item;
    
    if (!m->text[0]) return;
    GOTOXY(START_COL, firstitem_line + item);
    if (item == curitem) font_white(); else font_black();
    clr_line(g_term.num_cols - START_COL);
    g_term.y--;
    if (((m->type_flags & TYPE_MASK) == M_IDATA_BYTE) && (m->param == ((__idata unsigned char *) m->prop)[0]))
		puts("*");
	else
		g_term.x++;
    puts(m->text);
    font_black();
    if (m->xstr_id)
    {
	GOTOXY(1, firstitem_line + MENU_PTR.num_items + INFO_OFFSET);
	if (item == curitem)
		put_xstring(m->xstr_id);
	else
	{
		clr_line(g_term.num_cols);
		clr_line(g_term.num_cols);
	}
    }

}

void display_menu(BYTE update)
{
	char i;

	if (update == UPDATE_MENU) {
		display_menuitem(previtem);
	} else {
#if NUM_LINES_LARGE > 9
		firstitem_line = MENU_PTR.line + 2;
#else
		firstitem_line = MENU_PTR.line + 1;
#endif
		if (MENU_PTR.title[0]) {
			GOTOXY(START_COL + 1, MENU_PTR.line);
			puts(MENU_PTR.title);
		}
		for (i = 0; i < MENU_PTR.num_items; i++) {
		    if (i != curitem)
			display_menuitem(i);
		}
	}
	display_menuitem(curitem);
	previtem = curitem;
}

void draw_menu()
{
	cls();
	display_menu(SHOW_MENU);
}

MainState handle_edit(BYTE evt)
{
	char t;
	MainState s = S_EDIT;
	MENUAREA MItem *m = &MENU_PTR.item[curitem];
	__idata const BYTE *prop = m->prop;

	t = m->type_flags;
	switch (t & TYPE_MASK) {
		case M_SHOW:
			if (m->handler)
				s = (MainState) m->handler(evt);
			break;
		case M_DIRECT_BYTE:
			switch (evt) {
				case KEY_UP:
					( (BYTE *) m->prop)[0]++;
					break;
				case KEY_DOWN:
					( (BYTE *) m->prop)[0]--;
					break;
				case KEY_MENU:
					s = S_MENU;
				default:
					;
			}
			if (m->handler) m->handler(evt);
			GOTOXY(4, firstitem_line); print_short(( (BYTE *) m->prop)[0]);
			break;
		case M_IDATA_BYTE:
			( (__idata unsigned char *) m->prop)[0] = m->param;
			if (m->handler) m->handler(evt);
			if (!(t & F_MENUSWITCH))
				draw_menu();
			s = S_MENU;
			break;
		case M_HANDLER:
			s = (MainState) m->handler(evt);
			break;
		case M_EXIT:
			print_splash();
			return S_RUN;
		default:
			return S_MENU;
	}
	return s;
}


MainState load_menu(unsigned char menu)
{
	switch (menu)
	{
		case MENU_SPLASH: BANK_LOAD(g_splashmenu); break;
		case MENU_BACKLIGHT: BANK_LOAD(g_backlightmenu); break;
		case MENU_INFO: BANK_LOAD(g_infomenu); break;
		case MENU_SETUP: BANK_LOAD(g_setupmenu); break;

		//case MENU_MAIN:
		default:
			BANK_LOAD(g_mainmenu); break;
	}
	curitem = previtem = 0;
	draw_menu();
	if (MENU_PTR.item[0].text[0] == 0)
		return handle_edit(EVT_REFRESH);
		
	return S_MENU;
}


MainState handle_event(MainState state, BYTE evt) __banked
{
	MENUAREA MItem *m = &MENU_PTR.item[curitem];

	if (evt == EVT_REFRESH) {
		draw_menu();
	}

	switch (state) {
		case S_RUN:
			if (evt & KEY_MENU) {
				state = load_menu(MENU_MAIN);
			}
			break;
		case S_MENU:
			switch (evt) {
				case KEY_MENU:
					state = handle_edit(EVT_EDIT);
				if (m->type_flags & F_MENUSWITCH)
					state = load_menu(m->param);
					break;
				case KEY_DOWN:
					curitem--;
					if (curitem < 0) curitem = MENU_PTR.num_items - 1;
					display_menu(UPDATE_MENU);
					break;
				case KEY_UP:
					curitem++;
					if (curitem == MENU_PTR.num_items) curitem = 0;
					display_menu(UPDATE_MENU);
					break;
			}
			// Auto action when entering menu
			if (state == S_MENU && m->type_flags & F_ACTION) {
				if (evt != KEY_MENU) {
					handle_edit(0);
				}
			}
			break;
		case S_EDIT:
			state = handle_edit(evt);
			if (state == S_MENU)
				if (m->type_flags & F_MENUSWITCH)
					state = load_menu(m->param);
				else
					display_menu(SHOW_MENU);
			break;
	}
	return state;
}

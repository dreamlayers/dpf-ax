/* Menu handling */

#include "global.h"
#include "dpf.h"
#include "print.h"
#include "utils.h"

#pragma codeseg MENU
#pragma constseg MENU

__bit g_refresh = 0;

extern
__bit g_debug;
extern
__bit g_scope;

void menu_clr(char i)
{
	g_term.x = 0;
	while (i < 12) {
		g_term.y = i; clr_line(NUM_COLS_SMALL);
		i++;
	}
}

char hdl_monitor(BYTE evt) __reentrant
{
	evt;
	if (evt == EVT_EDIT) {
		g_scope ^= 1;
		menu_clr(L_EDITOR);
	}
	if (g_scope) draw_buf(64);
	return S_MENU;
}

char hdl_debug(BYTE evt)
{
	evt;
	if (evt == EVT_EDIT) {
		g_debug ^= 1;
		menu_clr(L_EDITOR);
	}
	if (!g_debug) {
		menu_clr(L_EDITOR);
		GOTOXY(1, L_EDITOR);
		print_dump();
	}
	return S_MENU;
}

char hdl_brightness(BYTE evt) __reentrant
{
	evt;
	if (g_lcd.brightness < 1) g_lcd.brightness = 1;
	if (g_lcd.brightness > PWM_PERIOD) g_lcd.brightness = PWM_PERIOD;

	tmr1cntl = 0; tmr1cnth = 0;
	tmr1perl = PWM_PERIOD; tmr1perh = 0x00;
	tmr1pwml = (~g_lcd.brightness & 0x7);
	tmr1pwmh = 0;
	return S_EDIT;
}

char hdl_off(BYTE evt) __reentrant
{
	evt;
	turn_off();
	return S_RUN;
}

#ifdef FLIX_MODE
char hdl_flix(BYTE evt) __reentrant
{
	flix_mode(evt);
	return S_MENU;
}
#endif

char hdl_reboot(BYTE evt) __reentrant
{
	evt;
	menu_clr(L_EDITOR);
	GOTOXY(4, 6);
	puts(g_rebootmsg);
	// Shut down nicely after a blink using the watchdog
	wdtcon &= ~WDTPSR;
	wdtcon |= WDTPND | WDTEN | 6; // reboot after 0.512 seconds
	pcon |= SELRTC; // Switch to RTC clock source
	_asm nop nop nop _endasm;
	_B_T3ON = 0;   // Power down

	return -1; // We never get here. If we do, we failed.
}

void display_menu(char selection)
{
	static
	BYTE prevsel = 0;

	BYTE n;
	int i;
	const OVLAREA Menu *m;

	if (selection & F_UPDATE) {
		selection &= ~F_UPDATE;
		term_selfont(FONT_SMALL);
		m = g_menu + prevsel;
		GOTOXY(m->pos[0], m->pos[1]);
		puts(m->text);
		term_selfont(FONT_SMALL_REVERSE);
		m = g_menu + selection;
		GOTOXY(m->pos[0], m->pos[1]);
		puts(m->text);
	} else {
		for (i = 0; i < g_menusize; i++) {
			m = g_menu + i;
			GOTOXY(m->pos[0], m->pos[1]);
			if (i == selection) {
				n = FONT_SMALL_REVERSE;
			} else {
				n = FONT_SMALL;
			}
			term_selfont(n);
			puts(m->text);
		}
	}
	prevsel = selection;
	term_selfont(FONT_SMALL);
}

MainState handle_edit(char selection, BYTE evt)
{
	char t;
	MainState s = S_EDIT;
	OVLAREA Menu *m;
	m = &g_menu[selection];

	t = m->type_flags;
	switch (t & TYPE_MASK) {
		case M_SHOW:
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
					menu_clr(L_EDITOR);
					return S_MENU;
				default:
					menu_clr(L_EDITOR);
			}
			if (m->handler) m->handler(evt);
			// If we don't just show (0), select:
			if (evt)
				term_selfont(FONT_SMALL_REVERSE);
			GOTOXY(8, L_EDITOR + 1); print_short(( (BYTE *) m->prop)[0]);
			term_selfont(FONT_SMALL);
			break;
		case M_HANDLER:
			s = (MainState) m->handler(evt);
			if (s == S_MENU) return s;
			// draw:

			break;
		case M_EXIT:
			if (m->handler) {
				m->handler(evt);
			} else {
				menu_clr(L_MENU);
			}
			return S_RUN;
		default:
			menu_clr(L_MENU);
			g_fakeled |= LED_ERR;
			return S_RUN;
	}
	return s;
}

void draw_menu(BYTE selection)
{
	BYTE n = 7;
	disp_home();
// We assume that we have no more than 80 columns..
	do {
		clr_line(NUM_COLS_SMALL);
	} while (--n);
	display_menu(selection);
}

MainState handle_event(MainState state, BYTE evt) __banked
{
	static char selection = 0;

	if (evt == EVT_REFRESH) {
		draw_menu(selection);
	}

	switch (state) {
		case S_RUN:
			if (evt & KEY_MENU) {
				state = S_MENU; 
				g_fakeled &= ~LED_ERR;
				draw_menu(selection);
			}
			break;
		case S_MENU:
			switch (evt) {
				case KEY_MENU:
					state = S_EDIT;
					state = handle_edit(selection, EVT_EDIT);
					break;
				case KEY_DOWN:
					selection--;
					if (selection < 0) selection = g_menusize - 1;
					display_menu(selection | F_UPDATE);
					break;
				case KEY_UP:
					selection++;
					if (selection == g_menusize) selection = 0;
					display_menu(selection | F_UPDATE);
					break;
			}
			// Auto action when entering menu
			if (state == S_MENU && g_menu[selection].type_flags & F_ACTION) {
				if (evt != KEY_MENU) {
					handle_edit(selection, 0);
				}
			}
			break;
		case S_EDIT:
			state = handle_edit(selection, evt);
			if (state == S_MENU) 
				display_menu(selection);
			break;
	}
	return state;
}




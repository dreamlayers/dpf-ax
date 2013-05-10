#include "ax206.h"

#include "dpf.h"
#include "config.h"
#include "global.h"
#include "utils.h"
#include "usb.h"
#include "lcd.h"
#include "print.h"

#pragma codeseg MAIN
#pragma constseg MAIN

volatile __bit g_usb_active;

#ifdef BUILD_DEVEL
__bit g_debug = 0;
__bit g_scope = 0;
__bit g_log = 0;  // log, if set
#endif

#ifdef DEBUG_DEV
__pdata unsigned char g_debugbuf[DEBUGBUF_SIZE];
unsigned char g_debugcount = 0;
#endif

struct inputs __idata g_button = {
	0, 0 
};

struct config_flash __idata g_config = {
	1, //SPLASH_DEVMODE
};

/* Note: Addressing can work different on other screens.
 *
 */


extern void usb_isr(void);

#ifdef BUILD_DEVEL

static
void fill(unsigned short rgb16)
{
	rgb16;
	_asm
	mov	a, dph
	lcall lcd_write
	mov	a, dpl
	lcall lcd_write
	_endasm;
}

/* Fake LED on the screen bottom */

const
unsigned short g_colors[] = {
	RGB565(32, 32, 32),
	RGB565(0, 255, 0),
	RGB565(255, 0, 0),
	RGB565(255, 255, 0),
	RGB565(255, 0, 0),
	RGB565(0, 255, 255),
	RGB565(255, 0, 0),
	RGB565(255, 120, 0),
	RGB565(127, 127, 255),
};


void fake_led(BYTE which, BYTE col) __banked
{
	int n;
	unsigned short c;
	_asm
		.include 'dpf.inc'
	_endasm;
	n = 2 + 16 * which;
	g_blit.y0 = RESOL_Y - 5;
	g_blit.y1 = RESOL_Y - 2;
	g_blit.x0 = n;
	g_blit.x1 = n + 7;
	disp_blit();
	c = g_colors[col];
	n = 32;
	while (n--) {
		fill(c);
	}
}

void do_leds(void)
{
	BYTE i;
	BYTE c;

	c = 1;
	for (i = 0; i < 8; i++) {
		if (g_fakeled & c) {
			fake_led(i, i+1);
		} else {
			fake_led(i, 0);
		}
		c <<= 1;
	}
}
#endif  //ifdef BUILD_DEVEL


void sleep_interruptible(void)
{
	BYTE save;
	save = ie;

	ie = EA | T0IE;

	wkedg = 0;
	wkpnd = 0; // Clear pending wakeup events
	wken = ~(WEDP); // Allow Wakeup from USB DP

	pcon |= COREEN;    // IDLE

	// Now wait until we get woken up...

	ie = save;
}

static
BYTE strlen(__code char *str)
{
	BYTE n = 0;
	while (*str++) n++;
	return n;
}


void umain(void) __banked
{
	static PwrState pstate = S_GOOD;
	static MainState prevstate = S_RUN, state = S_RUN;
	BYTE evt;
	char i;
	unsigned short count = 0;
	g_usb_active = 0;
	g_fakeled = 0;

	_B_EA = 0; // Mask all Interrupts

	_B_USBIE = 0; // Disable USB interrupt for now
	// Load User IRQ handlers and resident blitting routines:
	BANK_LOAD(timer0_isr);
	_B_EA = 1;

	// Clear counters:
	for (i = 0; i < sizeof(g_count) / sizeof(unsigned short); i++) {
		g_count[i] = 0;
	}

	// We disable the timer interrupts for now, as we don't have
	// proper handlers yet. Otherwise, they'll really slow us down.
	// ie &= ~(T0IE | T1IE | T2IE );

	g_lcd.brightness = LCD_DEFAULT_BRIGHTNESS_VALUE;
	g_lcd.orientation = LCD_ORIENTATION;
	// Initialize RTC, Clocks, LCD and ports:
	init_all();
	sleep(100);

#ifdef BUILD_DEVEL
	clrscreen(RGB565(0, 0, 70));
	// Load XSEG data:
	BANK_LOAD(g_menu);
#else
	// Load XSEG overlay data:
	BANK_LOAD(g_rebootmsg);
	// Read g_config from flash and init brightness, contrast, ...
	init_config();
#endif
	term_init();
	disp_home();
	print_splash();
	sleep(100);
	lcd_backlight_on();
#ifdef FLIX_MODE
	g_refresh = 1;
	g_debug = 1;
	// Automatically enter FLIX menu
	state = S_MENU;
	state = handle_event(state, EVT_EDIT);
#endif

	if (g_button.level & P_VUSBSN) {
		usb_init();
		g_fakeled |= LED_USB;
		g_usb_active = 1;
	}

#ifdef BUILD_DEVEL
	// Clear data buffer:
	for (i = 0; i < sizeof(g_databuf); i++) {
		g_databuf[i] = 0;
	}
#endif
	g_register[SECONDS] = 0;
	g_register[TIMER] = 20;
	g_register[MONITOR] = 250;

	count = g_count[TIMER];
	g_bgcol = DEV_SCREEN_BGCOL;
	g_fgcol = DEV_SCREEN_COL;
	g_term.offset_y = 32;

	while (1) {
		// Event key changed and key pressed (Press = LOW)
		RUB_WATCHDOG();

		evt = g_button.evt & ~g_button.level;
		if (evt) {
			g_button.evt = 0; // We ack the event
			// g_fakeled |= evt;
			state = handle_event(state, evt);
			if (evt & P_VUSBSN) {
				// TODO: Disable USB for power saving
				// usb_exit();
				pcon &= ~TMRCSEL;
				//use 24 MHz for compatibility with buildwin
				//and easier brightness adjust
				//pcon |= 1 << TMRCSEL_SHFT; // Select12 MHz
				g_fakeled &= ~LED_USB;
				g_usb_active = 0;
				set_brightness(g_lcd.brightness);
			}

		}
		evt = g_button.evt & g_button.level; // IO went high
		// Did we just plug in USB?
		if (evt & P_VUSBSN) {
			usb_init();
			pcon &= ~TMRCSEL;
			pcon |= 2 << TMRCSEL_SHFT; // Select48 MHz
			g_fakeled |= LED_USB;
			g_usb_active = 1;
			set_brightness(g_lcd.brightness);
		}

		// Refresh of some screen items, when g_refresh set, force
		if (STATE_LEAVE(prevstate, state, S_RUN) || g_refresh) {
#ifdef BUILD_DEVEL
			GOTOXY(0, g_term.num_lines - 2);
			puts(g_fakeleddesc);
#endif
			if (g_refresh) handle_event(state, EVT_REFRESH);
			g_refresh = 0;
		}

		if ((g_count[TIMER] - count) > g_register[TIMER] && (state != S_RUN)) {
			// Retrieve ADC value:
#ifndef NO_BATTERY_CHECK			
			i = NORMALIZE_ADCVAL(g_adc); 
			if (i < VBAT_THRESH_WARN) {
				g_fakeled ^= LED_PWR; // blink
				if (i < VBAT_THRESH_PWDN) {
					pstate = S_DOWN;
					clrscreen(RGB565(255, 0, 0));
					notice_powerdown();
					sleep(3000);
#ifdef BUILD_DEVEL
					g_log = 0;
#endif
					shutdown(PWR_DEEPSLEEP);
					//clrscreen(RGB565(0, 0, 70));
					disp_home(); print_splash(); lcd_backlight_on();
					g_refresh = 1;
				} else if (pstate != S_LOW) {
					pstate = S_LOW;
					GOTOXY(0, g_term.num_lines - 4);
					clr_line(g_term.num_cols - 1);
					puts("!! LOW POWER !!");
				}
				pstate = S_LOW;
			} else
			if (pstate != S_GOOD) {
				g_fakeled &= ~LED_PWR;
				pstate = S_GOOD;
				GOTOXY(0, g_term.num_lines - 4);
				clr_line(g_term.num_cols - 1);
				puts("Power good");
			}
#endif
			count = g_count[TIMER];
#ifdef BUILD_DEVEL
			if (g_debug) print_status();
			if (g_count[MONITOR] == 0) {
				g_count[MONITOR] = g_register[MONITOR];
				g_databuf[g_datacount] = i;
				if (g_scope) draw_buf(64);
				g_datacount++;
				g_datacount %= sizeof(g_databuf);
			}
			g_count[MONITOR]--;
			do_leds();
			// Mask the non-sticky bits:
			g_fakeled &= (LED_USB | LED_PWR | LED_ALV | LED_ERR | LED_LOG);
			g_fakeled ^= LED_ALV; // heartbeat
			if (g_log) g_fakeled |= LED_LOG;
			else       g_fakeled &= ~LED_LOG;
#endif  //ifdef BUILD_DEVEL
		}

		// Somewhat working with USB DP interrupt
		// sleep_interruptible();

		if (g_usb_active) {
			usbadr = R_USB_IntrUSB;
			if (usb_readbyte() & IRST) {
				usb_config();
			} else {
				usb_poll(); // Poll EP1
			}
		// } else
		// if (state == S_RUN) {
			// TODO: We could go into powersave, if we like.
		}
		prevstate = state;
	}
}

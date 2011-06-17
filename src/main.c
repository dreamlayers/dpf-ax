#include "ax206.h"

#include "dpf.h"
#include "config.h" // DEFAULT_ORIENTATION
#include "global.h"
#include "utils.h"
#include "usb.h"
#include "print.h"

#pragma codeseg MAIN
#pragma constseg MAIN

volatile __bit g_usb_active;
__bit g_debug = 0;
__bit g_scope = 0;
__bit g_log = 0;  // log, if set

struct inputs __idata g_button;

/* Note: Addressing can work different on other screens.
 *
 */


extern void usb_isr(void);

static
void fill(unsigned short rgb16)
{
	rgb16;
	_asm
	mov	a, dph
	lcall otp_lcd_write
	mov	a, dpl
	lcall otp_lcd_write
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

#if 0
void clr(unsigned short col)
{
	int x, y;
	col;
	g_blit.x0 = 0; g_blit.y0 = 0;
	g_blit.x1 = RESOL_X - 1; g_blit.y1 = RESOL_Y - 1;
	disp_blit();

	for (y = 0; y < RESOL_X; y++) {
		for (x = 0; x < RESOL_Y; x++) {
			_asm
			mov a, dph
			lcall otp_lcdwrite
			mov a, dpl
			lcall otp_lcdwrite
			_endasm;
		}
	}
}
#endif

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
	// set_orientation(ROT_DOWN);
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

	init_ports();

	// Should go once we do a full initialization here.
	lcd_orientation(g_lcd.orientation);

	clrscreen(RGB565(0, 0, 70));
	term_init();
	timer1_config(g_lcd.brightness); // Init backlight

	// Activate watchdog to reset after ~2 seconds:
	
	wdtcon &= ~WDTPSR;
	wdtcon |= WDTPND | WDTEN | 7;

	// We disable the timer interrupts for now, as we don't have
	// proper handlers yet. Otherwise, they'll really slow us down.
	// ie &= ~(T0IE | T1IE | T2IE );

	ie |= T0IE | RAWIE;

	// Load XSEG data:
	BANK_LOAD(g_menu);

	disp_home(); print_splash();

	if (g_button.level & P_VUSBSN) {
		usb_init();
		g_fakeled |= LED_USB;
		g_usb_active = 1;
	}

	// Clear data buffer:
	for (i = 0; i < sizeof(g_databuf); i++) {
		g_databuf[i] = 0;
	}

	// Clear counters:
	for (i = 0; i < sizeof(g_count) / sizeof(unsigned short); i++) {
		g_count[i] = 0;
	}

	g_register[SECONDS] = 0;
	g_register[TIMER] = 20;
	g_register[MONITOR] = 250;

	count = g_count[TIMER];
	g_term.bgcol = RGB565(0, 60, 40);
	g_term.col = RGB565(0, 255, 0);
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
				pcon |= 1 << TMRCSEL_SHFT; // Select12 MHz
				g_fakeled &= ~LED_USB;
				g_usb_active = 0;
			}

		}
		evt = g_button.evt & g_button.level; // IO went high
		// Did we just plug in USB?
		if (evt & P_VUSBSN) {
			usb_init();
			g_fakeled |= LED_USB;
			g_usb_active = 1;
		}

		// Refresh of some screen items, when g_refresh set, force
		if (STATE_LEAVE(prevstate, state, S_RUN) || g_refresh) {
			GOTOXY(0, NUM_LINES - 2);
			puts(g_fakeleddesc);
			if (g_refresh) handle_event(state, EVT_REFRESH);
			g_refresh = 0;
		}

		if ((g_count[TIMER] - count) > g_register[TIMER] && (state != S_RUN)) {
			// Retrieve ADC value:
			i = NORMALIZE_ADCVAL(g_adc); 
			if (i < VBAT_THRESH_WARN) {
				g_fakeled ^= LED_PWR; // blink
				if (i < VBAT_THRESH_PWDN) {
					pstate = S_DOWN;
					clrscreen(RGB565(255, 0, 0));
					notice_powerdown();
					sleep(3000);
					g_log = 0;
					shutdown(PWR_DEEPSLEEP);
					clrscreen(RGB565(0, 0, 70));
					g_refresh = 1;
				} else if (pstate != S_LOW) {
					pstate = S_LOW;
					GOTOXY(0, NUM_LINES - 4);
					clr_line(NUM_COLS - 1);
					puts("!! LOW POWER !!");
				}
				pstate = S_LOW;
			} else
			if (pstate != S_GOOD) {
				g_fakeled &= ~LED_PWR;
				pstate = S_GOOD;
				GOTOXY(0, NUM_LINES - 4);
				clr_line(NUM_COLS - 1);
				puts("Power good");
			}

			count = g_count[TIMER];
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

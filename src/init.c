/* Initialization routines that are needed mostly only once */

#include "print.h"
#include "global.h"
#include "utils.h"
#include "dpf.h"
#include "flash.h"

#pragma codeseg INIT
#pragma constseg INIT

void mdelay(BYTE x);

void term_init(void) __banked
{
	_asm
		mov	_g_chartbl_offs, #chartbl_offs
		mov	_g_chartbl_offs + 1, #(chartbl_offs >> 8)
		mov	_g_chartbl_offs + 2, #(chartbl_offs >> 16)
	_endasm;
	term_selfont(0);
}

void timer1_config(BYTE brightness) __banked
{
	tmr1cntl = 0;
	tmr1cnth = 0;
	// These are writeonly:
	tmr1perl = PWM_PERIOD; tmr1perh = 0x00;
	tmr1pwml = PWM_PERIOD - (brightness & 0x7);
	tmr1pwmh = 0;
	tmr1con = (3 << 4) | T1POS1 | T1POEN | T1ON;
}

void timer0_config()
{
	tmr0cnt = 0; // Reset counter
	tmr0pr = 41; // Period
	tmr0psr = 3; // Prescaler
	tmr0con = T0OS | T0CS | T0ON; // Turn on
}

void adc_config(BYTE enable)
{
	switch (enable) {
		case 0:
			adccon &= ~ADCEN;
			return;
		case 1:
			adcrate = 24 - 1;
			break;
		case 2:
			adcrate = TMREN | 1;
			break;
	}
	pie &= ~(P03); // Disable P03
	adccon |= ADCEN | 3; // Select P03 for analog in
}

void sleep(unsigned short ticks) __banked
{
	BYTE reg;
	reg = ie;

	tmr2cntl = 0;
	tmr2cnth = 0;
	tmr2perl = ticks;
	tmr2perh = ticks >> 8;

	_B_T2P = 0; // Make sure we're not pending

	ie = EA | T2IE;
	tmr2con =  T2S | (5 << 1); // Unit: 1 / 1024 second
 	tmr2con |= T2ON;

	wdtcon &= ~WDTEN;

	pcon |= COREEN;    // IDLE

	tmr2con = 0; // Stop timer
	wdtcon |= WDTPND | WDTEN | 7; // Rub Watchdog

	ie = reg;
}

static
void mdelay(BYTE x)
{
	int i;
	while (x--) {
		RUB_WATCHDOG();
		i = 0x200;
		do {
			_asm
				nop
			_endasm;
		} while (--i);
	}
}

static const BYTE rot_index[4] = {
	ROTCODE_UP, ROTCODE_LEFT, ROTCODE_DOWN, ROTCODE_RIGHT
};

BYTE g_rgborder = PIXELORDER(LCD_ORIENTATION_RGB);

void lcd_orientation(BYTE which) __banked
{
	BYTE c;
	c = rot_index[which];
#if defined(LCD_CMDSET_OT)
	lcd_ot_setorientation(which);
#elif defined(LCD_CMDSET_ILI)
	lcd_ili_setorientation(which);
#else
	lcd_st_setrotation(c); // this wants the "rotcode"
#endif
	g_rgborder = RGBORDER(c);
}


static
void flash_writecmd(BYTE cmd)
{
	spi_select();
	spibuf = cmd;
	while (spi_busy());
	spi_deselect();
}

static void flash_wakeup(void)
{
	spi_select();
	spibuf = SPM_RES; while (spi_busy());
	spibuf = 0xff; while (spi_busy());
	spibuf = 0xff; while (spi_busy());
	spibuf = 0xff; while (spi_busy());
	sleep(10);
	spi_deselect();
}

void config_ports(BYTE enable)
{
	if (enable) {
		p1 = 0xff;
		p2 = ~P26;
		_LCD_PWR = 0;
		// Configure Inputs:
		p0dir = ~(P_XXX | LCD_PWR | P00); // Outputs
		p1dir = FLA_MISO | P13; // Inputs
		p2dir = ~(P26 | LCD_LED | LCD_CS | FLA_CS); // Those are outputs
	} else {
		p3dir = ALL_OUTPUTS;
		p3 = 0;
		p0up |= (BUT_MENU | P05 | P02 | LCD_PWR | P00); // Pullup enable
		p0dir = ALL_INPUTS & ~P00;
		p1 = P13; // VddIo switch?
		p1up = 0x00;
		p1dir = FLA_MISO; // only input
		p2dir = ~(P27 | P26 | LCD_LED | LCD_CS | FLA_CS ); // Outputs
#ifdef DPFMODEL_pink
		p2up  = ~(      P26 | LCD_LED | LCD_CS | FLA_CS ); // PU disable
		p2   &= ~(      P26           | LCD_CS | FLA_CS ); // ????
		// Pulling FLA_CS active does not make sense
#else
		p2up  = ~(      P26 | LCD_LED | LCD_CS | FLA_CS ); // PU disable
#endif
		p4dir = ALL_INPUTS;
	}
}

__pdata BYTE g_databuf[0x40];
unsigned BYTE g_datacount = 0;

void logger(void)
{
	BYTE n;
	unsigned char v;

	// Turn on oscillator for ADC:
	pcon &= ~OSCCEN;
	_asm nop nop nop _endasm;

	pcon &= ~SELRTC;  // use oscillator as system clock
	_asm nop nop nop _endasm; // Important: Wait to settle..

	config_ports(1);
	adc_config(1);
	adccon |= ADCGO; // kick on conversion

	wdtcon |= WDTPND | WDTEN | 7; // Turn on WDOG
	while ((adccon & ADCGO)); // Wait til ADC value ready
	
	v = (adcbufl >> 6) | (adcbufh << 2);

	g_databuf[g_datacount++] = v;
	g_count[LOG]++; // total count
	if (g_datacount == sizeof(g_databuf) / 2 /* || v < 0 */) {
		n = flash_write(g_databuf, g_datacount);
		g_datacount = 0;
	}

	wdtcon &= ~WDTEN; // Turn off watchdog
	pcon |= SELRTC;  // use RTC as system clock
	_asm nop nop nop _endasm; // Important: Wait to settle..
	pcon |= OSCCEN;
	config_ports(0);
	adc_config(0);
}

void shutdown(BYTE mode) __banked
{
	if (g_log) {
		flash_erasesector(LOG_SECTOR);
		g_datacount = 0;
		g_databuf[0] = 0xbe;
		g_databuf[1] = 0xef;
		flash_write(g_databuf, 2);
	}

	switch (mode) {
		case PWR_DOWN:
			tmr3con &= ~T3ON; // Disable RTC timer
		case PWR_DEEPSLEEP:
			ckcon = IROMCEN;   // Turn off ROM clock
#ifdef DANGEROUS_SLEEP
			// This is dangerous, because some flashes don't wake up
			// from power down...
			if (!g_log) 
				flash_writecmd(SPM_DP); // Power down flash
#endif
			config_ports(0);
			adc_config(0);
		case PWR_SLEEP:
			tmr2con &= ~T2ON; // Disable timer
			tmr1con = 0;
			tmr0con &= ~T0ON; // Disable timer
			_LCD_LED = nOFF; // turn off backlight
	}

	wdtcon &= ~WDTEN; // Turn off watchdog

	wkedg = WEP06; // falling edge menu button
	wkpnd = 0; // Clear pending wakeup events
	wken = ~(WEP06 | WEP07); // Allow only MENU button wakeup

	ie = EA | WAUIE;  // allow only port interrupts

	// _B_T3IE = 0; This resets the chip when pending

	pcon |= SELRTC;  // use RTC as system clock
	_asm nop nop nop _endasm; // Important: Wait to settle..

	// ..before we disable the main clock

	pcon |= OSCCEN;
	pcon |= STOPC;    // Stop clock
	_asm nop nop nop _endasm;

	// Now we wait for an interrupt:
	if (mode != PWR_DOWN) {
		_B_EA = 0; g_wkup = 0; g_alarm = 0; _B_EA = 1;
		ie |= RAWIE; // Enable RTC IRQ
		// We loop while we have only RTC interrupts:
		while (!g_wkup && !g_alarm) {
			// Log every x seconds:
			if (g_log && (g_count[SECONDS] & 0x1f) == 0) logger();
			pcon |= STOPC;    // Stop clock again
			_asm nop nop nop _endasm;
		}
	}

	pcon &= ~OSCCEN;
	_asm nop nop nop _endasm;

	wdtcon |= WDTPND | WDTEN | 7; // Turn on WDOG

	switch (mode) {
		case PWR_DOWN:
			tmr3con |= T3ON; // Enable RTC timer
		case PWR_DEEPSLEEP:
			pcon &= ~SELRTC;  // use 24 mhz sysclk
			ckcon = 0;   // Turn on ROM clock
			config_ports(1);
#ifdef DANGEROUS_SLEEP
			flash_wakeup();
#endif
			adc_config(1);
			lcd_init(); // Initialize LCD
			adccon |= ADCGO; // kick on conversion
		case PWR_SLEEP:
			timer1_config(g_lcd.brightness);
	}

	sleep(100);

	timer0_config();

	ie |= WAUIE | T0IE | RAWIE;
}

void init_ports(void) __banked
{
	config_ports(1);
}

void turn_off(void) __banked
{
	sleep(500);
	// If this is enabled, log ADC values to flash
	shutdown(PWR_DEEPSLEEP);
	sleep(200);
	g_button.evt = 0; // clear events so we don't right repeat this
	clrscreen(RGB565(0, 0, 70));
	disp_home(); print_splash();
	// g_refresh = 1; // force menu refresh
}


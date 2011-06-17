/** Unused code. Experimental */

#include "global.h"
#include "dpf.h"
#pragma codeseg HOME0

__data BYTE g_adcount = 0;

// Button queue:
__data BYTE g_butq[2];

volatile __bit g_alarm;

void timer0_isr(void) __interrupt(0) __using(1)
{
	BYTE b;
	static __idata BYTE debounce = 0;

	p3dir = ALL_INPUTS;
	g_count[TIMER]++;

	// _B_P04 = 0;
	// p0dir &= ~P04; // Output

	// p0dir |= P04; // Input

	if (!(adccon & ADCGO)) {
		g_adc = adcbufl | (adcbufh << 8);
	}

	if (!(--g_adcount)) {
		adccon |= ADCGO; // kick on conversion
		g_adcount = 8;
	}

	// Get port values:
	// if ((g_count[0] & 0x0f) == 0) {
		b = (p3 & 0x3f) | (p0 & 0xc0);
		// Debounce:
		debounce &= (g_butq[0] | g_butq[1] | b);
		debounce |= (g_butq[0] & g_butq[1] & b);

		g_butq[1] = g_butq[0];
		g_butq[0] = b;

		// Store in global vars:
		g_button.evt = debounce ^ g_button.level;  // Change
		g_button.level = debounce;   // Keypress
	// }
	p3dir = ALL_OUTPUTS;

	_B_T0P = 0; // Clear timer 0 pending
}

/** RTC and watchdog interrupt handler. Normally interrupts every second
 */

void rtc_isr(void) __interrupt(6) __using(1)
{
	g_count[SECONDS]++;
	if (g_count[SECONDS] == g_register[SECONDS] && g_count[SECONDS])
		g_alarm = 1;
	// Clear pending bit:
	_B_T3CP = 0;
}

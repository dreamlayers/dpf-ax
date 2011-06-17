#pragma codeseg DEBUG
#pragma constseg DEBUG

#include "dpf.h"
#include "global.h"
#include "print.h"

void print_dump(void) __banked
{
	BYTE ox;
	ox = g_term.x;

	puts("sp:     "); out_short(sp);
	g_term.x = ox; g_term.y++;
	puts("ie:     "); out_short(ie);
	g_term.x = ox; g_term.y++;
	puts("eif0:   "); out_short(eif0);
	g_term.x = ox; g_term.y++;
	puts("pcon:   "); out_short(pcon);
	g_term.x = ox; g_term.y++;
	puts("dpcon:  "); out_short(dpcon);
	g_term.x = ox; g_term.y++;
	puts("wtdcon: "); out_short(wdtcon);
}


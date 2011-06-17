; 
; Bootstrap module for our own environment:
;
	.module bootstrap
	.include 'dpf.inc'
	.globl _umain
	.globl __sdcc_banked_call

	; relocation order:
	.area HOME (CODE)
	.area SSEG (DATA)
__start__stack:
	.ds	1
	.area GSINIT (CODE)
	.area GSINIT0 (CODE)
	.area GSFINAL (CODE)

bootstrap::
	sjmp	skip
	.ascii "--bootstrap--"
skip:
	mov	sp,#__start__stack - 1

	; Fake return address:
	mov	a, #(return_base)
	push	acc
	mov	a, #(return_base >> 8)
	push	acc
	mov	G_bank, #(return_base >> 16)

	anl	dpcon, #~DPAID	; Disable autoincrement by default
	; orl	dpcon, #IA	; Enable IRQ vectors base at 0x1000

	; Load code of module 0 (IRQ handler)
	mov dpl,	#(irq0_timer0 >> 16)
	lcall	_bank_load

	; Debugging: Turn off backlight LED
	; mov tmr1con, #0
	; mov LCD_LED, #1

	mov	r0, #_umain
	mov	r1, #(_umain >> 8)
	mov	r2, #(_umain >> 16)

	ljmp	__sdcc_banked_call

bootstrap_end::

	; This bank must be a bank number which is not directly called
	; from this module.
	.area INIT (CODE)

return_base::
	ljmp	0x0000	; reset



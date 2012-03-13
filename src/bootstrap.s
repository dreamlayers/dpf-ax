; 
; Bootstrap module for our own environment:
;
	.module bootstrap
	.include 'dpf.inc'
	.globl _umain
	.globl __sdcc_banked_call

	; relocation order:
	.area SSEG (DATA)
__start__stack:
	.ds	1
	.area GSINIT0 (CODE)
bootstrap::
	orl	p0, #0x40	; p0.6 enable
	orl	p0up, #0x40	; p0.6 pullup enable
	nop
	nop
	nop
	nop
	nop
	mov	a, p0
	anl	a, #0x40
	jnz	s_GSINIT
	anl	dpcon, #~IA; Use ROM IRQ handlers
	ret

	.area GSINIT (CODE)
	.area GSFINAL (CODE)

boot:

	mov	sp,#__start__stack - 1 ; Set stack pointer
	anl	dpcon, #~DPAID	; Disable autoincrement by default

	; Load code of module 0 (IRQ handler)
	mov dpl,	#(irq0_timer0 >> 16)
	lcall	_bank_load

	orl	dpcon, #IA	; Enable IRQ vectors base at 0x1000

	; Debugging: Turn off backlight LED
	; mov tmr1con, #0
	; mov LCD_LED, #1

	acall	ramclr; Clear Stack

	mov	r0, #_umain
	mov	r1, #(_umain >> 8)
	mov	r2, #(_umain >> 16)

	ljmp	__sdcc_banked_call

ramclr:
	mov	r0, #__start__stack-1
clr_loop:
	mov	@r0, #0x00
	dec	r0
	mov	a, r0
	xrl	a, #0x01
	jnz	clr_loop
	ret

bootstrap_end::

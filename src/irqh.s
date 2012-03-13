;
;  IRQ handler code
;  2011/02/18 15:49
;
	.module irqh
;
	.area DSEG (ABS)

	.area BSEG(BIT)
_g_wkup::
	.ds 1
_g_tick::
	.ds 1

	.area IRQHANDLER (CODE, ABS)
	.org	0x1003
	.include 'dpf.inc'
;
irq0_timer0::
	; push	psw
	; mov	psw, #0x8	; Switch to register bank 1
	ljmp	_timer0_isr

	.org	0x100b
irq1_timer1::
	clr	_T1P
	reti
	.org	0x1013
irq2_timer2::
	setb	_g_tick
	clr	_T2P
	reti
; The USB IRQ handler
	.org	0x101b

irq3_usb:
	reti
; Don't use USB irq handler for now
	; ljmp	_usb_isr
;
	.org	0x102b
;
irq5_port:
	ljmp	port_handler
;
; Watchdog and RTC
	.org	0x1033
;
irq6_rtc:
	ljmp	_rtc_isr
;
; Port wakeup handler:

port_handler::
	mov	wken, #(WEDP | WEP07 | WEP06 | WEP05) ; Mask all bits
	anl	wkpnd, #0	; Clear pending bits
	; Set "woken up" flag:
	setb	_g_wkup
	reti

	.area HOME (CODE)

	.include 'dpf.inc'

	.area BLIT (CODE)
;
; Call to ROM lcd write routine
; Replace, if custom lcd write is required
;
lcd_write::
	ljmp	otp_lcd_write


ar0 = 0x00

_custom_landscape_blit::
	mov	a,r0
	push	acc
	clr	a
	mov	r0,#0x2b
	lcall	X12be
	mov	a,_g_blit+y0+1
	mov	r0,_g_blit+y0
	lcall	X12b5
	mov	a,_g_blit+y1+1
	mov	r0,_g_blit+y1
	lcall	X12b5
	clr	a
	mov	r0,#0x2a
	acall	X12be
	mov	a,_g_blit+x0+1
	mov	r0,_g_blit+x0
	lcall	X12b5
	mov	a,_g_blit+x1+1
	mov	r0,_g_blit+x1
	lcall	X12b5
	clr	a
	mov	r0,#0x2c
	lcall	X12be
	setb	LCD_A0
	pop	acc
	mov	r0,a
	ret	
;
X12b5:	setb	LCD_A0
X12b7:	lcall	otp_lcd_write
	mov	a,r0
	ljmp	otp_lcd_write
;
X12be:	clr	LCD_A0
	sjmp	X12b7
;
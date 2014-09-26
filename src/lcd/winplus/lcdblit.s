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
	push	ar0
	clr	a
	mov	r0,#0x2a
	lcall	X12bb
	mov	a,_g_blit+x0
	mov	r0,a
	mov	a,_g_blit+x0+1
	lcall	X12b2
	mov	a,_g_blit+x1
	mov	r0,a
	mov	a,_g_blit+x1+1
	lcall	X12b2
	clr	a
	mov	r0,#0x2b
	lcall	X12bb
	mov	a,_g_blit+y0
	mov	r0,a
	mov	a,_g_blit+y0+1
	lcall	X12b2
	mov	a,_g_blit+y1
	mov	r0,a
	mov	a,_g_blit+y1+1
	acall	X12b2
	clr	a
	mov	r0,#0x2c
	lcall	X12bb
	setb	LCD_A0
	pop	ar0
	ret	
;
X12b2:	setb	LCD_A0
X12b4:	lcall	otp_lcd_write
	mov	a,r0
	ljmp	otp_lcd_write
;
X12bb:	clr	LCD_A0
	sjmp	X12b4

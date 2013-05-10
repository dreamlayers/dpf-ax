.include 'dpf.inc'

	.area BLIT (CODE)
;
; Call to ROM lcd write routine
; Replace, if custom lcd write is required
;
lcd_write::
	ljmp	otp_lcd_write


ar0 = 0x00

_custom_portrait_blit::
	push	ar0
	clr	a
	mov	r0,#0x50
	lcall	X12d7
	mov	a,_g_blit+x0+1
	mov	r0,_g_blit+x0
	lcall	X12ce
	clr	a
	mov	r0,#0x51
	acall	X12d7
	mov	a,_g_blit+x1+1
	mov	r0,_g_blit+x1
	acall	X12ce
	clr	a
	mov	r0,#0x52
	acall	X12d7
	mov	a,_g_blit+y0+1
	mov	r0,_g_blit+y0
	acall	X12ce
	clr	a
	mov	r0,#0x53
	acall	X12d7
	mov	a,_g_blit+y1+1
	mov	r0,_g_blit+y1
	acall	X12ce
	clr	a
	mov	r0,#0x20
	acall	X12d7
	mov	a,_g_blit+x0+1
	mov	r0,_g_blit+x0
	acall	X12ce
	clr	a
	mov	r0,#0x21
	acall	X12d7
	mov	a,_g_blit+y0+1
	mov	r0,_g_blit+y0
	acall	X12ce
	clr	a
	mov	r0,#0x22
	acall	X12d7
	setb	LCD_A0
	pop	ar0
	ret	
;
X12ce:	setb	LCD_A0
X12d0:	lcall	otp_lcd_write
	mov	a,r0
	ljmp	otp_lcd_write
;
X12d7:	clr	LCD_A0
	sjmp	X12d0

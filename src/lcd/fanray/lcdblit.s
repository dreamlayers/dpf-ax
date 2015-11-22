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
	mov	a,r0
	push	acc
	clr	a
	mov	r0,#0x50
	lcall	X12d9
	mov	a,_g_blit+x0+1
	mov	r0,_g_blit+x0
	lcall	X12d0
	clr	a
	mov	r0,#0x51
	lcall	X12d9
	mov	a,_g_blit+x1+1
	mov	r0,_g_blit+x1
	acall	X12d0
	clr	a
	mov	r0,#0x52
	acall	X12d9
	mov	a,_g_blit+y0+1
	mov	r0,_g_blit+y0
	acall	X12d0
	clr	a
	mov	r0,#0x53
	acall	X12d9
	mov	a,_g_blit+y1+1
	mov	r0,_g_blit+y1
	acall	X12d0
	clr	a
	mov	r0,#0x20
	acall	X12d9
	mov	a,_g_blit+y0+1
	mov	r0,_g_blit+y0
	acall	X12d0
	clr	a
	mov	r0,#0x21
	acall	X12d9
	mov	a,_g_blit+x0+1
	mov	r0,_g_blit+x0
	acall	X12d0
	clr	a
	mov	r0,#0x22
	acall	X12d9
	setb	LCD_A0
	pop	acc
	mov	r0,a
	ret	
;
X12d0:	setb	LCD_A0
X12d2:	lcall	otp_lcd_write
	mov	a,r0
	ljmp	otp_lcd_write
;
X12d9:	clr	LCD_A0
	sjmp	X12d2

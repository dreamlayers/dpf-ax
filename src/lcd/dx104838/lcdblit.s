	.include 'dpf.inc'

	.area BLIT (CODE)

ar0 = 0x00

_custom_landscape_blit::
	mov	a,r0
	push	acc
	clr	a
	mov	r0,#0x2b
	lcall	X12b9
	mov	a,_g_blit+y0+1
	mov	r0,_g_blit+y0
	lcall	X12b0
	mov	a,_g_blit+y1+1
	mov	r0,_g_blit+y1
	acall	X12b0
	clr	a
	mov	r0,#0x2a
	acall	X12b9
	mov	a,_g_blit+x0+1
	mov	r0,_g_blit+x0
	acall	X12b0
	mov	a,_g_blit+x1+1
	mov	r0,_g_blit+x1
	acall	X12b0
	clr	a
	mov	r0,#0x2c
	acall	X12b9
	setb	LCD_A0
	pop	acc
	mov	r0,a
	ret	
;
X12b0:	setb	LCD_A0
X12b2:	lcall	otp_lcd_write
	mov	a,r0
	ljmp	otp_lcd_write
;
X12b9:	clr	LCD_A0
	sjmp	X12b2

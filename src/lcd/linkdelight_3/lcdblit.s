	.include 'dpf.inc'

	.area BLIT (CODE)

ar0 = 0x00

_custom_landscape_blit::
lcd_setcontext:
	push	ar0
	clr	a
	mov	r0,#0x50
	lcall	X12f0
	mov	a,_g_blit+y0+1
	mov	r0,_g_blit+y0
	lcall	X12e7
	clr	a
	mov	r0,#0x51
	acall	X12f0
	mov	a,_g_blit+y1+1
	mov	r0,_g_blit+y1
	acall	X12e7
	clr	a
	mov	r0,#0x52
	acall	X12f0
	mov	a,#0x3f
	clr	c
	subb	a,_g_blit+x1
	mov	r0,a
	mov	a,#0x1
	subb	a,_g_blit+x1+1
	acall	X12e7
	clr	a
	mov	r0,#0x53
	lcall	X12f0
	mov	a,#0x3f
	clr	c
	subb	a,_g_blit+x0
	mov	r0,a
	mov	a,#0x1
	subb	a,_g_blit+x0+1
	acall	X12e7
	clr	a
	mov	r0,#0x20
	acall	X12f0
	mov	a,#0xef
	clr	c
	subb	a,_g_blit+y0
	mov	r0,a
	mov	a,#0x0
	subb	a,_g_blit+y0+1
	acall	X12e7
	clr	a
	mov	r0,#0x21
	acall	X12f0
	mov	a,#0x3f
	clr	c
	subb	a,_g_blit+x0
	mov	r0,a
	mov	a,#0x1
	subb	a,_g_blit+x0+1
	acall	X12e7
	clr	a
	mov	r0,#0x22
	acall	X12f0
	setb	LCD_A0
	pop	ar0
	ret	
;
X12e7:	setb	LCD_A0
X12e9:	lcall	otp_lcd_write
	mov	a,r0
	ljmp	otp_lcd_write
;
X12f0:	clr	LCD_A0
	sjmp	X12e9

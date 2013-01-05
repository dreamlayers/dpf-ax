	.include 'dpf.inc'

	.area BLIT (CODE)

ar0 = 0x00

_custom_landscape_blit::
	mov	a,r0
	push	acc
	clr	a
	mov	r0,#0x50
	lcall	X12f8
	mov	a,#0xef
	clr	c
	subb	a,_g_blit+y1
	mov	r0,a
	mov	a,#0x0
	subb	a,_g_blit+y1+1
	lcall	X12ef
	clr	a
	mov	r0,#0x51
	acall	X12f8
	mov	a,#0xef
	clr	c
	subb	a,_g_blit+y0
	mov	r0,a
	mov	a,#0x0
	subb	a,_g_blit+y0+1
	acall	X12ef
	clr	a
	mov	r0,#0x52
	acall	X12f8
	mov	a,_g_blit+x0+1
	mov	r0,_g_blit+x0
	lcall	X12ef
	clr	a
	mov	r0,#0x53
	lcall	X12f8
	mov	a,_g_blit+x1+1
	mov	r0,_g_blit+x1
	lcall	X12ef
	clr	a
	mov	r0,#0x20
	lcall	X12f8
	mov	a,#0xef
	clr	c
	subb	a,_g_blit+y0
	mov	r0,a
	mov	a,#0x0
	subb	a,_g_blit+y0+1
	acall	X12ef
	clr	a
	mov	r0,#0x21
	lcall	X12f8
	mov	a,#0x3f
	clr	c
	subb	a,_g_blit+x0
	mov	r0,a
	mov	a,#0x1
	subb	a,_g_blit+x0+1
	acall	X12ef
	clr	a
	mov	r0,#0x22
	lcall	X12f8
	setb	LCD_A0
	pop	acc
	mov	r0,acc
	ret	
;
X12ef:	setb	LCD_A0
X12f1:	lcall	otp_lcd_write
	mov	a,r0
	ljmp	otp_lcd_write
;
X12f8:	clr	LCD_A0
	sjmp	X12f1
;

.include 'dpf.inc'

	.area BLIT (CODE)

ar0 = 0x00

_custom_portrait_blit::
	push	ar0
	clr	a
	mov	r0,#0x2a
	lcall	X12ae
	mov	a,_g_blit+x0+1
	mov	r0,_g_blit+x0
	lcall	X12b2
	mov	a,_g_blit+x1+1
	mov	r0,_g_blit+x1
	lcall	X12b2
	clr	a
	mov	r0,#0x2b
	acall	X12ae
	mov	a,_g_blit+y0+1
	mov	r0,_g_blit+y0
	acall	X12b2
	mov	a,_g_blit+y1+1
	mov	r0,_g_blit+y1
	acall	X12b2
	clr	a
	mov	r0,#0x2c
	acall	X12ae
	setb	LCD_A0
	pop	ar0
	ret	
;
X12ae:	clr	LCD_A0
	sjmp	X12b4
;
X12b2:	setb	LCD_A0
X12b4:	acall	X131a
	lcall	otp_lcd_write
	mov	a,r0
	acall	X131a
	ljmp	otp_lcd_write
;
X131a:	rrc	a
	mov	p0.0,c
	rrc	a
	mov	p0.2,c
	setb	c
	rlc	a
	mov	c,SPI_CS
	rlc	a
	mov	p2,a
	ret	
;

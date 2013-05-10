	.include 'dpf.inc'

	.area BLIT (CODE)
;
; Call to ROM lcd write routine
; Replace, if custom lcd write is required
;
lcd_write::
X131a:	rrc	a
	mov	p0.0,c
	rrc	a
	mov	p0.2,c
	setb	c
	rlc	a
	mov	c,SPI_CS
	rlc	a
	mov	p2,a
	ljmp	otp_lcd_write


ar0 = 0x00

_custom_portrait_blit::

	push	ar0
	clr	a
	mov	r0,#0x2
	acall	X12dc
	clr	a
	mov	r0,_g_blit+x0+1
	acall	X12e0
	clr	a
	mov	r0,#0x3
	acall	X12dc
	clr	a
	mov	r0,_g_blit+x0
	acall	X12e0
	clr	a
	mov	r0,#0x4
	acall	X12dc
	clr	a
	mov	r0,_g_blit+x1+1
	acall	X12e0
	clr	a
	mov	r0,#0x5
	acall	X12dc
	clr	a
	mov	r0,_g_blit+x1
	acall	X12e0
	clr	a
	mov	r0,#0x6
	acall	X12dc
	clr	a
	mov	r0,_g_blit+y0+1
	acall	X12e0
	clr	a
	mov	r0,#0x7
	acall	X12dc
	clr	a
	mov	r0,_g_blit+y0
	acall	X12e0
	clr	a
	mov	r0,#0x8
	acall	X12dc
	clr	a
	mov	r0,_g_blit+y1+1
	acall	X12e0
	clr	a
	mov	r0,#0x9
	acall	X12dc
	clr	a
	mov	r0,_g_blit+y1
	acall	X12e0
	clr	a
	mov	r0,#0x22
	acall	X12dc
	setb	LCD_A0
	pop	ar0
	ret	
;
X12dc:	clr	LCD_A0
	sjmp	X12e2
;
X12e0:	setb	LCD_A0
X12e2:	lcall	lcd_write
	mov	a,r0
	ljmp	lcd_write
;

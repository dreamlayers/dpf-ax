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
	mov	r0,#0x50
	lcall	X12e6
	mov	a,#0xef
	clr	c
	subb	a,_g_blit+y1
	mov	r0,a
	mov	a,#0x0
	subb	a,_g_blit+y1+1
	lcall	X12ea
	clr	a
	mov	r0,#0x51
	lcall	X12e6
	mov	a,#0xef
	clr	c
	subb	a,_g_blit+y0
	mov	r0,a
	mov	a,#0x0
	subb	a,_g_blit+y0+1
	lcall	X12ea
	clr	a
	mov	r0,#0x52
	lcall	X12e6
	mov	a,_g_blit+x0+1
	mov	r0,_g_blit+x0
	lcall	X12ea
	clr	a
	mov	r0,#0x53
	lcall	X12e6
	mov	a,_g_blit+x1+1
	mov	r0,_g_blit+x1
	lcall	X12ea
	clr	a
	mov	r0,#0x20
	lcall	X12e6
	mov	a,#0xef
	clr	c
	subb	a,_g_blit+y0
	mov	r0,a
	mov	a,#0x0
	subb	a,_g_blit+y0+1
	lcall	X12ea
	clr	a
	mov	r0,#0x21
	lcall	X12e6
	mov	a,#0x3f
	clr	c
	subb	a,_g_blit+x0
	mov	r0,a
	mov	a,#0x1
	subb	a,_g_blit+x0+1
	lcall	X12ea
	clr	a
	mov	r0,#0x22
	lcall	X12e6
	setb	LCD_A0
	pop	ar0
	ret	
;
X12e6:	clr	LCD_A0
	sjmp	X12ec
;
X12ea:	setb	LCD_A0
X12ec:	lcall	otp_lcd_write
	mov	a,r0
	ljmp	otp_lcd_write

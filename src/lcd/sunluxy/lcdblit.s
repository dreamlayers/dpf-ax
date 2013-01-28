	.include 'dpf.inc'

	.area BLIT (CODE)

ar0 = 0x00

_custom_blit::
	clr	LCD_A0
	mov	a,#0xf4
	lcall	otp_lcd_write
	mov	a,_g_blit+x0
	lcall	otp_lcd_write
	mov	a,#0xf6
	lcall	otp_lcd_write
	mov	a,_g_blit+x1
	lcall	otp_lcd_write
	mov	a,#0xf5
	lcall	otp_lcd_write
	mov	a,_g_blit+y0
	lcall	otp_lcd_write
	mov	a,#0xf7
	lcall	otp_lcd_write
	mov	a,_g_blit+y1
	lcall	otp_lcd_write
	mov	a,_g_blit+x0
	anl	a,#0xf
	lcall	otp_lcd_write
	mov	a,_g_blit+x0
	swap	a
	anl	a,#0x7
	orl	a,#0x10
	lcall	otp_lcd_write
	mov	a,_g_blit+y0
	anl	a,#0xf
	orl	a,#0x60
	lcall	otp_lcd_write
	mov	a,_g_blit+y0
	swap	a
	anl	a,#0x7
	orl	a,#0x70
	lcall	otp_lcd_write
	setb	LCD_A0
	ret	

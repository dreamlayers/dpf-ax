.include 'dpf.inc'

	.area BLIT (CODE)

ar0 = 0x00

_custom_blit::
	clr	LCD_A0
	mov	a,#0x2a
	lcall	otp_lcd_write
	setb	LCD_A0
	mov	a,_g_blit+x0+1
	lcall	otp_lcd_write
	mov	a,_g_blit+x0
	lcall	otp_lcd_write
	mov	a,_g_blit+x1+1
	lcall	otp_lcd_write
	mov	a,_g_blit+x1
	lcall	otp_lcd_write
	clr	LCD_A0
	mov	a,#0x2b
	lcall	otp_lcd_write
	setb	LCD_A0
	mov	a,_g_blit+y0+1
	lcall	otp_lcd_write
	mov	a,_g_blit+y0
	lcall	otp_lcd_write
	mov	a,_g_blit+y1+1
	lcall	otp_lcd_write
	mov	a,_g_blit+y1
	lcall	otp_lcd_write
	clr	LCD_A0
	mov	a,#0x2c
	lcall	otp_lcd_write
	setb	LCD_A0
	ret	

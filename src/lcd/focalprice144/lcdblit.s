	.include 'dpf.inc'

	.area BLIT (CODE)

ar0 = 0x00

_custom_portrait_blit::
	mov	a,#0x0d	
	clr	LCD_A0	
	lcall	otp_lcd_write
	mov	a,_g_blit+x0
	add	a,#2	
	clr	LCD_A0	
	lcall	otp_lcd_write
	mov	a,#0x0f	
	clr	LCD_A0	
	lcall	otp_lcd_write
	mov	a,_g_blit+x1
	add	a,#2	
	clr	LCD_A0	
	lcall	otp_lcd_write
	mov	a,#0x0e	
	clr	LCD_A0	
	lcall	otp_lcd_write
	mov	a,_g_blit+y0
	add	a,#0x10	
	clr	LCD_A0	
	lcall	otp_lcd_write
	mov	a,#0x10	
	clr	LCD_A0	
	lcall	otp_lcd_write
	mov	a,_g_blit+y1
	add	a,#0x10	
	clr	LCD_A0	
	lcall	otp_lcd_write
	setb	LCD_A0	
	ret		
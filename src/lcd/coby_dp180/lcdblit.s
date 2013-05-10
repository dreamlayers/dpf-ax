	.include 'dpf.inc'

	.area BLIT (CODE)

ar0 = 0x00

_custom_landscape_blit::
;
	mov	a,#0x2a
	clr	LCD_A0
	lcall	otp_lcd_write
	mov	a,_g_blit+x0+1
	setb	LCD_A0
	lcall	otp_lcd_write
	mov	a,_g_blit+x0
	setb	LCD_A0
	lcall	otp_lcd_write
	mov	a,_g_blit+x1+1
	setb	LCD_A0
	lcall	otp_lcd_write
	mov	a,_g_blit+x1
	setb	LCD_A0
	lcall	otp_lcd_write
	mov	a,#0x2b
	clr	LCD_A0
	lcall	otp_lcd_write
	mov	a,_g_blit+y0+1
	setb	LCD_A0
	lcall	otp_lcd_write
	mov	a,_g_blit+y0
	setb	LCD_A0
	lcall	otp_lcd_write
	mov	a,_g_blit+y1+1
	setb	LCD_A0
	lcall	otp_lcd_write
	mov	a,_g_blit+y1
	setb	LCD_A0
	lcall	otp_lcd_write
	mov	a,#0x2c
	clr	LCD_A0
	lcall	otp_lcd_write
	setb	LCD_A0
	ret	
;
;
; Call to ROM lcd write routine
; Replace, if custom lcd write is required
;
lcd_write::
	ljmp	otp_lcd_write

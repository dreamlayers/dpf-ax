	.include 'dpf.inc'

	.area BLIT (CODE)

ar0 = 0x00

_custom_blit::
	clr	LCD_A0
	mov	a,#0x2a
	lcall	otp_lcd_write
	setb	LCD_A0
	mov	a,_g_blit+x0+1
	add	a,#0x2
	lcall	otp_lcd_write
	mov	a,_g_blit+x0
	add	a,#0x2
	lcall	otp_lcd_write
	mov	a,_g_blit+x1+1
	add	a,#0x2
	lcall	otp_lcd_write
	mov	a,_g_blit+x1
	add	a,#0x2
	lcall	otp_lcd_write
	clr	LCD_A0
	mov	a,#0x2b
	lcall	otp_lcd_write
	setb	LCD_A0
	mov	a,_g_blit+y0+1
	add	a,#0x3
	lcall	otp_lcd_write
	mov	a,_g_blit+y0
	add	a,#0x3
	lcall	otp_lcd_write
	mov	a,_g_blit+y1+1
	add	a,#0x3
	lcall	otp_lcd_write
	mov	a,_g_blit+y1
	add	a,#0x3
	lcall	otp_lcd_write
	clr	LCD_A0
	mov	a,#0x2c
	lcall	otp_lcd_write
	setb	LCD_A0
	ret	
;
; Call to ROM lcd write routine
; Replace, if custom lcd write is required
;
lcd_write::
	ljmp	otp_lcd_write

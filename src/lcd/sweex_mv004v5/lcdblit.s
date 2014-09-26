	.include 'dpf.inc'

	.area BLIT (CODE)
;
; Call to ROM lcd write routine
; Replace, if custom lcd write is required
;
lcd_write::
	ljmp	otp_lcd_write


ar0 = 0x00

_custom_blit::
	mov	a,#0x15
	clr	LCD_A0
	lcall	otp_lcd_write
	mov	a,_g_blit+x0
	add	a,#0x3
	setb	LCD_A0
	lcall	otp_lcd_write
	mov	a,_g_blit+x1
	add	a,#0x3
	setb	LCD_A0
	lcall	otp_lcd_write
	mov	a,#0x75
	clr	LCD_A0
	lcall	otp_lcd_write
	mov	a,_g_blit+y0
	add	a,#0x4
	setb	LCD_A0
	lcall	otp_lcd_write
	mov	a,_g_blit+y1
	add	a,#0x4
	setb	LCD_A0
	lcall	otp_lcd_write
	mov	a,#0x5c
	clr	LCD_A0
	lcall	otp_lcd_write
	setb	LCD_A0
	ret	

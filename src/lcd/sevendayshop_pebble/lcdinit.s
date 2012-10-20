	.include 'dpf.inc'

	.area LCDAUX (CODE)

_lcd_custom_init::
	mov	p3dir,#0x0
	anl	p1dir,#0xe8
	anl	p2dir,#0xfd
	clr	LCD_CS
	setb	LCD_RST
	lcall	X1750
	setb	LCD_RST
	clr	LCD_RST
	mov	a,#0xa
	lcall	X1750
	setb	LCD_RST
	mov	a,#0x1
	lcall	X1766
	mov	a,#0x78
	lcall	X1750
	mov	a,#0x11
	lcall	X1766
	mov	a,#0x78
	lcall	X1750
	mov	a,#0x3a
	lcall	X1766
	mov	a,#0x5
	lcall	X1770
	mov	a,#0xc0
	lcall	X1766
	mov	a,#0x1
	lcall	X1770
	mov	a,#0x1
	lcall	X1770
	mov	a,#0xc3
	lcall	X1766
	mov	a,#0x7
	lcall	X1770
	mov	a,#0xc5
	lcall	X1766
	mov	a,#0x9a
	lcall	X1770
	mov	a,#0xc6
	lcall	X1766
	mov	a,#0x12
	lcall	X1770
	mov	a,#0xf0
	lcall	X1766
	mov	a,#0x5a
	lcall	X1770
	mov	a,#0xf2
	lcall	X1766
	mov	a,#0x0
	lcall	X1770
	mov	a,#0xf3
	lcall	X1766
	mov	a,#0x31
	lcall	X1770
	mov	a,#0xb4
	lcall	X1766
	mov	a,#0x4
	lcall	X1770
	mov	a,#0xe0
	lcall	X1766
	mov	a,#0x0
	lcall	X1770
	mov	a,#0x2
	lcall	X1770
	mov	a,#0x3
	lcall	X1770
	mov	a,#0x22
	lcall	X1770
	mov	a,#0x1f
	lcall	X1770
	mov	a,#0x0
	lcall	X1770
	mov	a,#0xf
	lcall	X1770
	mov	a,#0x4
	lcall	X1770
	mov	a,#0x2
	lcall	X1770
	mov	a,#0x0
	lcall	X1770
	mov	a,#0xe
	lcall	X1770
	mov	a,#0xf
	lcall	X1770
	mov	a,#0x0
	lcall	X1770
	mov	a,#0x4
	lcall	X1770
	mov	a,#0x3
	lcall	X1770
	mov	a,#0x5
	lcall	X1770
	mov	a,#0xe1
	lcall	X1766
	mov	a,#0x0
	lcall	X1770
	mov	a,#0x1f
	lcall	X1770
	mov	a,#0x21
	lcall	X1770
	mov	a,#0x3
	lcall	X1770
	mov	a,#0x2
	lcall	X1770
	mov	a,#0x0
	lcall	X1770
	mov	a,#0x4
	lcall	X1770
	mov	a,#0xf
	lcall	X1770
	mov	a,#0x5
	lcall	X1770
	mov	a,#0x3
	lcall	X1770
	mov	a,#0x4
	lcall	X1770
	mov	a,#0x0
	lcall	X1770
	mov	a,#0xf
	lcall	X1770
	mov	a,#0xe
	lcall	X1770
	mov	a,#0x0
	lcall	X1770
	mov	a,#0x2
	lcall	X1770
	mov	a,#0x3
	lcall	X1770
	mov	a,#0x2a
	lcall	X1766
	mov	a,#0x0
	lcall	X1766
	mov	a,#0x0
	lcall	X1770
	mov	a,#0x0
	lcall	X1770
	mov	a,#0x7f
	lcall	X1770
	mov	a,#0x2b
	lcall	X1766
	mov	a,#0x0
	lcall	X1766
	mov	a,#0x0
	lcall	X1770
	mov	a,#0x0
	lcall	X1770
	mov	a,#0x7f
	lcall	X1770
	mov	a,#0x2c
	lcall	X1766
	mov	a,#0x36
	lcall	X1766
	mov	a,#0xc8
	lcall	X1770
	mov	a,#0x29
	lcall	X1766
	ret	
;
X1750:	mov	r5,a
X1751:	mov	r6,#0x18
X1753:	mov	r7,#0xfa
X1755:	djnz	r7,X1755
	djnz	r6,X1753
	djnz	r5,X1751
	ret	
;
X1766:	clr	LCD_A0
	ljmp	otp_lcd_write
;
X1770:	setb	LCD_A0
	ljmp	otp_lcd_write
;
;
; No detectable Lcd_Contrast routine - do not use contrast setting!
;
;_lcd_custom_setcontrast::
;	ret

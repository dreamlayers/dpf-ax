	.include 'dpf.inc'

	.area LCDAUX (CODE)

_lcd_custom_init::
mov	p3dir,#0x0
	anl	p1dir,#0xe8
	anl	p2dir,#0xfd
	clr	LCD_CS
	clr	LCD_RST
	mov	a,#0x96
	lcall	X1670
	setb	LCD_RST
	mov	a,#0x96
	lcall	X1670
	mov	a,#0x1
	lcall	X1686
	mov	a,#0x32
	lcall	X1670
	mov	a,#0x11
	lcall	X1686
	mov	a,#0xc8
	lcall	X1670
	mov	a,#0xb1
	lcall	X1686
	mov	a,#0x5
	lcall	X1690
	mov	a,#0x1
	lcall	X1690
	mov	a,#0xc0
	lcall	X1686
	mov	a,#0x4
	lcall	X1690
	mov	a,#0x10
	lcall	X1690
	mov	a,#0xc1
	lcall	X1686
	mov	a,#0x3
	lcall	X1690
	mov	a,#0xc2
	lcall	X1686
	mov	a,#0x5
	lcall	X1690
	mov	a,#0xc5
	lcall	X1686
	mov	a,#0x4f
	lcall	X1690
	mov	a,#0x40
	lcall	X1690
	mov	a,#0xec
	lcall	X1686
	mov	a,#0x2
	lcall	X1690
	mov	a,#0xf2
	lcall	X1686
	mov	a,#0x1
	lcall	X1690
	mov	a,#0xe0
	lcall	X1686
	mov	a,#0x3b
	lcall	X1690
	mov	a,#0x2b
	lcall	X1690
	mov	a,#0x2a
	lcall	X1690
	mov	a,#0x2d
	lcall	X1690
	mov	a,#0x28
	lcall	X1690
	mov	a,#0xc
	lcall	X1690
	mov	a,#0x57
	lcall	X1690
	mov	a,#0xf0
	lcall	X1690
	mov	a,#0x41
	lcall	X1690
	mov	a,#0x12
	lcall	X1690
	mov	a,#0x15
	lcall	X1690
	mov	a,#0x12
	lcall	X1690
	mov	a,#0x11
	lcall	X1690
	mov	a,#0x10
	lcall	X1690
	mov	a,#0x4
	lcall	X1690
	mov	a,#0xe1
	lcall	X1686
	mov	a,#0x4
	lcall	X1690
	mov	a,#0x14
	lcall	X1690
	mov	a,#0x15
	lcall	X1690
	mov	a,#0x12
	lcall	X1690
	mov	a,#0x17
	lcall	X1690
	mov	a,#0x13
	lcall	X1690
	mov	a,#0x28
	lcall	X1690
	mov	a,#0x60
	lcall	X1690
	mov	a,#0x3e
	lcall	X1690
	mov	a,#0xd
	lcall	X1690
	mov	a,#0x2a
	lcall	X1690
	mov	a,#0x2d
	lcall	X1690
	mov	a,#0x2e
	lcall	X1690
	mov	a,#0x2f
	lcall	X1690
	mov	a,#0x3b
	lcall	X1690
	mov	a,#0x2a
	lcall	X1686
	mov	a,#0x0
	lcall	X1690
	mov	a,#0x0
	lcall	X1690
	mov	a,#0x0
	lcall	X1690
	mov	a,#0x7f
	lcall	X1690
	mov	a,#0x2b
	lcall	X1686
	mov	a,#0x4
	lcall	X1690
	mov	a,#0x14
	lcall	X1690
	mov	a,#0x15
	lcall	X1690
	mov	a,#0x12
	lcall	X1690
	mov	a,#0x36
	lcall	X1686
	mov	a,#0xc0
	lcall	X1690
	mov	a,#0x3a
	lcall	X1686
	mov	a,#0x65
	lcall	X1690
	mov	a,#0x29
	lcall	X1686
	mov	a,#0x2c
	lcall	X1686
	ret
;
X1670:	mov	r5,a
X1671:	mov	r6,#0x18
X1673:	mov	r7,#0xfa
X1675:	djnz	r7,X1675
	djnz	r6,X1673
	djnz	r5,X1671
	ret	
;
X1686:	clr	LCD_A0
	ljmp	otp_lcd_write
;
X1690:	setb	LCD_A0
	ljmp	otp_lcd_write
;
; No detectable Lcd_Contrast routine - do not use contrast setting!
;
;_lcd_custom_setcontrast::
;	ret

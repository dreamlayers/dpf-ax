	.include 'dpf.inc'

	.area LCDAUX (CODE)

_lcd_custom_init::
	mov	p3dir,#0x0
	anl	p1dir,#0xe8
	anl	p2dir,#0xfd
	clr	LCD_CS
	clr	LCD_RST
	mov	a,#0x96
	lcall	X15dd
	setb	LCD_RST
	mov	a,#0xc8
	lcall	X15dd
	mov	a,#0xe2
	lcall	X15f3
	mov	a,#0xc8
	lcall	X15dd
	mov	a,#0xeb
	lcall	X15f3
	mov	a,#0x81
	lcall	X15f3
	mov	a,#0x70
	lcall	X15f3
	mov	a,#0xd1
	lcall	X15f3
	mov	a,#0xd6
	lcall	X15f3
	mov	a,#0xa2
	lcall	X15f3
	mov	a,#0xc8
	lcall	X15f3
	mov	a,#0x50
	lcall	X15f3
	mov	a,#0x60
	lcall	X15f3
	mov	a,#0x70
	lcall	X15f3
	mov	a,#0x0
	lcall	X15f3
	mov	a,#0x10
	lcall	X15f3
	mov	a,#0x2a
	lcall	X15f3
	mov	a,#0xc0
	lcall	X15f3
	mov	a,#0x89
	lcall	X15f3
	mov	a,#0x24
	lcall	X15f3
	mov	a,#0x2f
	lcall	X15f3
	mov	a,#0xde
	lcall	X15f3
	mov	a,#0x30
	lcall	X15f3
	mov	a,#0x5f
	lcall	X15f3
	mov	a,#0x32
	lcall	X15f3
	mov	a,#0x5c
	lcall	X15f3
	mov	a,#0xf4
	lcall	X15f3
	mov	a,#0x0
	lcall	X15f3
	mov	a,#0xf5
	lcall	X15f3
	mov	a,#0xf6
	lcall	X15f3
	mov	a,#0x7f
	lcall	X15f3
	mov	a,#0xf7
	lcall	X15f3
	mov	a,#0x7f
	lcall	X15f3
	mov	a,#0xf8
	lcall	X15f3
	mov	a,#0xaf
	lcall	X15f3
	ret	
;
X15dd:	mov	r5,a
X15de:	mov	r6,#0x18
X15e0:	mov	r7,#0xfa
X15e2:	djnz	r7,X15e2
	djnz	r6,X15e0
	djnz	r5,X15de
	ret	
;
X15f3:	clr	LCD_A0
	ljmp	otp_lcd_write
;
X1602:	setb	LCD_A0
	ljmp	otp_lcd_write

;
_lcd_custom_setcontrast::
	mov	a,#0x81
	clr	LCD_A0
	lcall	otp_lcd_write
	mov	a,dpl
	dec	a
	mov	dptr,#contrast_table
	movc	a,@a+dptr
	setb	LCD_A0
	ljmp	otp_lcd_write
	
	.area LCDAUX (CODE)

contrast_table:
	.db	0x9a, 0x9f, 0xa5, 0xaa, 0xb0, 0xb5
;
_custom_contrasttbl_len::  .db  . - contrast_table

	.include 'dpf.inc'

	.area LCDAUX (CODE)

_lcd_custom_init::
	mov	p3dir,#0x0
	anl	p1dir,#0xe8
	anl	p2dir,#0xfd
	clr	LCD_CS
	clr	LCD_RST
	mov	a,#0x96
	lcall	X1658
	setb	LCD_RST
	mov	a,#0xc8
	lcall	X1658
	mov	a,#0xe2
	lcall	X166e
	mov	a,#0xc8
	lcall	X1658
	mov	a,#0xc8
	lcall	X1658
	mov	a,#0xc8
	lcall	X1658
	mov	a,#0xea
	lcall	X166e
	mov	a,#0x81
	lcall	X166e
	mov	a,#0x84
	lcall	X166e
	mov	a,#0x2f
	lcall	X166e
	mov	a,#0xd1
	lcall	X166e
	mov	a,#0xd6
	lcall	X166e
	mov	a,#0xc0
	lcall	X166e
	mov	a,#0x89
	lcall	X166e
	mov	a,#0xa2
	lcall	X166e
	mov	a,#0xc8
	lcall	X166e
	mov	a,#0x18
	lcall	X166e
	mov	a,#0xde
	lcall	X166e
	mov	a,#0xaf
	lcall	X166e
	ret	
;
X1658:	mov	r5,a
X1659:	mov	r6,#0x18
X165b:	mov	r7,#0xfa
X165d:	djnz	r7,X165d
	djnz	r6,X165b
	djnz	r5,X1659
	ret	
;
X166e:	clr	LCD_A0
	ljmp	otp_lcd_write

;
_lcd_custom_setcontrast::
	mov	a,#0x81
	clr	LCD_A0
	lcall	otp_lcd_write	
	mov	dptr,#contrast_table
	mov	a,r3
	dec	a
	movc	a,@a+dptr
	setb	LCD_A0
	ljmp	otp_lcd_write

	.area LCDAUX (CODE)

contrast_table:
  .db  0x65, 0x6a, 0x73, 0x79, 0x82, 0x89, 0x8f

_custom_contrasttbl_len::  .db  . - contrast_table

	.include 'dpf.inc'

	.area LCDAUX (CODE)

_lcd_custom_init::
mov	p3dir,#0x0
	anl	p1dir,#0xe8
	anl	p2dir,#0xfd
	clr	LCD_CS
	clr	LCD_RST
	mov	a,#0x96
	lcall	X167e
	setb	LCD_RST
	mov	a,#0xc8
	lcall	X167e
	mov	a,#0xb8
	lcall	X1694
	mov	a,#0x0
	lcall	X1694
	mov	a,#0xe2
	lcall	X1694
	mov	a,#0xc8
	lcall	X167e
	mov	a,#0x24
	lcall	X1694
	mov	a,#0x2a
	lcall	X1694
	mov	a,#0x2f
	lcall	X1694
	mov	a,#0xc0
	lcall	X1694
	mov	a,#0x89
	lcall	X1694
	mov	a,#0xa2
	lcall	X1694
	mov	a,#0xd1
	lcall	X1694
	mov	a,#0xd6
	lcall	X1694
	mov	a,#0xe9
	lcall	X1694
	mov	a,#0x81
	lcall	X1694
	mov	a,#0xb0
	lcall	X1694
	mov	a,#0xc8
	lcall	X1694
	mov	a,#0x56
	lcall	X1694
	mov	a,#0xdc
	lcall	X1694
	mov	a,#0xaf
	lcall	X1694
	mov	a,#0xc8
	lcall	X167e
	ret	
;
X167e:	mov	r5,a
X167f:	mov	r6,#0x18
X1681:	mov	r7,#0xfa
X1683:	djnz	r7,X1683
	djnz	r6,X1681
	djnz	r5,X167f
	ret	
;
X1694:	clr	LCD_A0
	ljmp	otp_lcd_write
;
X16a3:	setb	LCD_A0
	ljmp	otp_lcd_write

_lcd_custom_setbacklight::
	mov	a,r3
	jnz	doset
	ret
doset:
mov	dptr,#backlight_table
	mov	a,r3
	dec	a
	movc	a,@a+dptr
	mov	tmr1perl,#0xff
	mov	tmr1perh,#0x0
	mov	tmr1pwml,a
	mov	tmr1pwmh,#0x0
	ret
;	
_lcd_custom_setcontrast::
	mov	a,#0x81
	lcall	X1694
	mov	dptr,#contrast_table
	mov	a,r3
	dec	a
	movc	a,@a+dptr
	lcall	X16a3
	ret

	.area LCDAUX (CODE)


backlight_table::
	.db	0xe1, 0xda, 0xd2, 0xc5, 0xbe, 0xb6, 0xaf, 0xa8, 0xa0, 0x99, 0x8f
	.db	0x89, 0x82, 0x7b, 0x5c, 0x54, 0x4c, 0x44, 0x32, 0x23, 0x14
_custom_backlighttbl_len::  .db  . - backlight_table


contrast_table::
	.db	0xa5, 0xa7, 0xa9, 0xab, 0xad, 0xaf, 0xb2, 0xb4
	.db	0xb7, 0xba, 0xbd, 0xc0, 0xc3, 0xc5, 0xc7, 0xc9
_custom_contrasttbl_len::  .db  . - contrast_table


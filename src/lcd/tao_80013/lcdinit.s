	.include 'dpf.inc'

	.area LCDAUX (CODE)

_lcd_custom_init::
	mov	p3dir,#0x0
	anl	p1dir,#0xe8
	anl	p2dir,#0xfd
	clr	LCD_CS
	clr	LCD_RST
	mov	a,#0x96
	lcall	X16e8
	setb	LCD_RST
	mov	a,#0x96
	lcall	X16e8
	mov	a,#0x1
	lcall	X16fe
	mov	a,#0x32
	lcall	X16e8
	mov	a,#0x11
	lcall	X16fe
	mov	a,#0xc8
	lcall	X16e8
	mov	a,#0xb1
	lcall	X16fe
	mov	a,#0x1
	lcall	X1708
	mov	a,#0x8
	lcall	X1708
	mov	a,#0x5
	lcall	X1708
	mov	a,#0xb2
	lcall	X16fe
	mov	a,#0x2
	lcall	X1708
	mov	a,#0x23
	lcall	X1708
	mov	a,#0x22
	lcall	X1708
	mov	a,#0xb3
	lcall	X16fe
	mov	a,#0x2
	lcall	X1708
	mov	a,#0x23
	lcall	X1708
	mov	a,#0x22
	lcall	X1708
	mov	a,#0x2
	lcall	X1708
	mov	a,#0x23
	lcall	X1708
	mov	a,#0x22
	lcall	X1708
	mov	a,#0xb4
	lcall	X16fe
	mov	a,#0x3
	lcall	X1708
	mov	a,#0xc0
	lcall	X16fe
	mov	a,#0x82
	lcall	X1708
	mov	a,#0x14
	lcall	X1708
	mov	a,#0x84
	lcall	X1708
	mov	a,#0xc2
	lcall	X16fe
	mov	a,#0xa
	lcall	X1708
	mov	a,#0x0
	lcall	X1708
	mov	a,#0xc3
	lcall	X16fe
	mov	a,#0x8a
	lcall	X1708
	mov	a,#0x2e
	lcall	X1708
	mov	a,#0xc4
	lcall	X16fe
	mov	a,#0x8a
	lcall	X1708
	mov	a,#0xaa
	lcall	X1708
	mov	a,#0xc5
	lcall	X16fe
	mov	a,#0xb
	lcall	X1708
	mov	a,#0xe0
	lcall	X16fe
	mov	a,#0x3
	lcall	X1708
	mov	a,#0x15
	lcall	X1708
	mov	a,#0xc
	lcall	X1708
	mov	a,#0x12
	lcall	X1708
	mov	a,#0x3a
	lcall	X1708
	mov	a,#0x32
	lcall	X1708
	mov	a,#0x2b
	lcall	X1708
	mov	a,#0x2d
	lcall	X1708
	mov	a,#0x2b
	lcall	X1708
	mov	a,#0x28
	lcall	X1708
	mov	a,#0x30
	lcall	X1708
	mov	a,#0x3c
	lcall	X1708
	mov	a,#0x0
	lcall	X1708
	mov	a,#0x2
	lcall	X1708
	mov	a,#0x3
	lcall	X1708
	mov	a,#0x10
	lcall	X1708
	mov	a,#0xe1
	lcall	X16fe
	mov	a,#0x3
	lcall	X1708
	mov	a,#0x15
	lcall	X1708
	mov	a,#0xc
	lcall	X1708
	mov	a,#0x12
	lcall	X1708
	mov	a,#0x27
	lcall	X1708
	mov	a,#0x25
	lcall	X1708
	mov	a,#0x21
	lcall	X1708
	mov	a,#0x28
	lcall	X1708
	mov	a,#0x28
	lcall	X1708
	mov	a,#0x27
	lcall	X1708
	mov	a,#0x30
	lcall	X1708
	mov	a,#0x3c
	lcall	X1708
	mov	a,#0x0
	lcall	X1708
	mov	a,#0x1
	lcall	X1708
	mov	a,#0x3
	lcall	X1708
	mov	a,#0x10
	lcall	X1708
	mov	a,#0x2a
	lcall	X16fe
	mov	a,#0x0
	lcall	X1708
	mov	a,#0x0
	lcall	X1708
	mov	a,#0x0
	lcall	X1708
	mov	a,#0x7f
	lcall	X1708
	mov	a,#0x36
	lcall	X16fe
	mov	a,#0xc0
	lcall	X1708
	mov	a,#0x3a
	lcall	X16fe
	mov	a,#0x5
	lcall	X1708
	mov	a,#0x29
	lcall	X16fe
	mov	a,#0x2c
	lcall	X16fe
	ret	
;
X16fe:	clr	LCD_A0
	ljmp	otp_lcd_write
;
X1708:	setb	LCD_A0
	ljmp	otp_lcd_write
;
X16e8:	mov	r5,a
X16e9:	mov	r6,#0x18
X16eb:	mov	r7,#0xfa
X16ed:	djnz	r7,X16ed
	djnz	r6,X16eb
	djnz	r5,X16e9
	ret	
;
_lcd_custom_setbacklight::
	mov	a,r3
	jnz	doset
	ret
doset:	clr	c
	mov	a,#0x15
	subb	a,r3
	mov	tmr1perl,#0xff
	mov	tmr1perh,#0x40
	mov	tmr1pwml,#0xff
	mov	tmr1pwmh,a
	ret
;
;
_lcd_custom_setcontrast::
	mov	a,#0xc5
	lcall	X16fe
	mov	dptr,#contrast_table
	mov	a,r3
	dec	a
	movc	a,@a+dptr
	lcall	X1708
	mov	a,#0xf
	lcall	X1708
	ret
;
	.area LCDAUX (CODE)


;backlight_table::
;	.db	If needed, put data for custom backlight handling here
;_custom_backlighttbl_len::  .db  . - backlight_table


contrast_table::
	.db	0x06, 0x08, 0x0a, 0x0c, 0x0e, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1a, 0x1c
	.db	0x1e, 0x20, 0x22, 0x24, 0x26, 0x28, 0x2a, 0x2c, 0x2e, 0x30, 0x32, 0x34
	.db	0x36, 0x38, 0x3a, 0x3c, 0x3e, 0x40, 0x42, 0x44
;
_custom_contrasttbl_len::  .db  . - contrast_table


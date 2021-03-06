	.include 'dpf.inc'

	.area LCDAUX (CODE)

_lcd_custom_init::
	mov	p3dir,#0x0
	anl	p1dir,#0xe8
	anl	p2dir,#0xfd
	clr	LCD_CS
	clr	LCD_RST
	mov	a,#0x96
	lcall	X16ec
	setb	p1.0
	mov	a,#0x96
	lcall	X16ec
	mov	a,#0x1
	lcall	X1702
	mov	a,#0x96
	lcall	X16ec
	mov	a,#0xd7
	lcall	X1702
	mov	a,#0x9f
	lcall	X170c
	mov	a,#0xe0
	lcall	X1702
	mov	a,#0x0
	lcall	X170c
	mov	a,#0xa
	lcall	X16ec
	mov	a,#0xe3
	lcall	X1702
	mov	a,#0x14
	lcall	X16ec
	mov	a,#0xe1
	lcall	X1702
	mov	a,#0x28
	lcall	X1702
	mov	a,#0x11
	lcall	X170c
	mov	a,#0xa
	lcall	X16ec
	mov	a,#0xc0
	lcall	X1702
	mov	a,#0xff
	lcall	X170c
	mov	a,#0x0
	lcall	X170c
	mov	a,#0x0
	lcall	X1702
	mov	a,#0x0
	lcall	X170c
	mov	a,#0xc3
	lcall	X1702
	mov	a,#0x0
	lcall	X170c
	mov	a,#0xc4
	lcall	X1702
	mov	a,#0x6
	lcall	X170c
	mov	a,#0xc5
	lcall	X1702
	mov	a,#0x1
	lcall	X170c
	mov	a,#0xcb
	lcall	X1702
	mov	a,#0x1
	lcall	X170c
	mov	a,#0xd0
	lcall	X1702
	mov	a,#0x1d
	lcall	X170c
	mov	a,#0xb5
	lcall	X1702
	mov	a,#0x80
	lcall	X170c
	mov	a,#0xbd
	lcall	X1702
	mov	a,#0x2
	lcall	X170c
	mov	a,#0xe8
	lcall	X1702
	mov	a,#0x8d
	lcall	X170c
	mov	a,#0xd
	lcall	X170c
	mov	a,#0x1c
	lcall	X170c
	mov	a,#0xf0
	lcall	X1702
	mov	a,#0x6
	lcall	X170c
	mov	a,#0xb
	lcall	X170c
	mov	a,#0xd
	lcall	X170c
	mov	a,#0x15
	lcall	X170c
	mov	a,#0x11
	lcall	X1702
	mov	a,#0x20
	lcall	X1702
	mov	a,#0x38
	lcall	X1702
	mov	a,#0x36
	lcall	X1702
	mov	a,#0xe8
	lcall	X170c
	mov	a,#0x3a
	lcall	X1702
	mov	a,#0x5
	lcall	X170c
	mov	a,#0xb0
	lcall	X1702
	mov	a,#0x7f
	lcall	X170c
	mov	a,#0x2a
	lcall	X1702
	mov	a,#0x
	lcall	X170c
	mov	a,#0x7f
	lcall	X170c
	mov	a,#0x2b
	lcall	X1702
	mov	a,#0x4
	lcall	X170c
	mov	a,#0x83
	lcall	X170c
	mov	a,#0xf9
	lcall	X1702
	mov	a,#0x
	lcall	X170c
	mov	a,#0x3
	lcall	X170c
	mov	a,#0x5
	lcall	X170c
	mov	a,#0x7
	lcall	X170c
	mov	a,#0x9
	lcall	X170c
	mov	a,#0xb
	lcall	X170c
	mov	a,#0xd
	lcall	X170c
	mov	a,#0xf
	lcall	X170c
	mov	a,#0x11
	lcall	X170c
	mov	a,#0x13
	lcall	X170c
	mov	a,#0x15
	lcall	X170c
	mov	a,#0x17
	lcall	X170c
	mov	a,#0x19
	lcall	X170c
	mov	a,#0x1b
	lcall	X170c
	mov	a,#0x1d
	lcall	X170c
	mov	a,#0x1f
	lcall	X170c
	mov	a,#0x29
	lcall	X1702
	mov	a,#0x2c
	lcall	X1702
	ret

X16ec:	mov	r5,a
;
X1702:	clr	LCD_A0
	ljmp	otp_lcd_write
;
X170c:	setb	LCD_A0
	ljmp	otp_lcd_write
;
_lcd_custom_setcontrast::
	mov	a,#0x25
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
	.db	0x25, 0x27, 0x29, 0x2b, 0x2d, 0x30, 0x35, 0x3a, 0x40
	.db	0x45, 0x4a, 0x50, 0x55, 0x5a, 0x60, 0x65
;
_custom_contrasttbl_len::  .db  . - contrast_table

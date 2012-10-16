	.include 'dpf.inc'

	.area LCDAUX (CODE)

_lcd_custom_init::
X1330:	mov	p3dir,#0x0
	anl	p1dir,#0xe8
	anl	p2dir,#0xfd
	clr	LCD_CS
	clr	LCD_RST
	mov	a,#0x96
	lcall	X178c
	setb	LCD_RST
X1344:	mov	a,#0x96
	lcall	X178c
	mov	a,#0x1
	lcall	X17a2
	mov	a,#0x96
	lcall	X178c
	mov	a,#0xd7
	lcall	X17a2
	mov	a,#0xbf
	lcall	X17ac
	mov	a,#0xe0
	lcall	X17a2
	mov	a,#0x0
	lcall	X17ac
	mov	a,#0xa
	lcall	X178c
	mov	a,#0xe3
	lcall	X17a2
	mov	a,#0x14
	lcall	X178c
	mov	a,#0xe1
	lcall	X17a2
	mov	a,#0x28
	lcall	X17a2
	mov	a,#0x11
	lcall	X17a2
	mov	a,#0xc0
X1387:	lcall	X17a2
	mov	a,#0x17
	lcall	X17ac
	mov	a,#0x1
	lcall	X17ac
	mov	a,#0xc3
	lcall	X17a2
	mov	a,#0x0
	lcall	X17ac
	mov	a,#0xc4
	lcall	X17a2
	mov	a,#0x6
	lcall	X17ac
X13a8:	mov	a,#0xc5
	lcall	X17a2
	mov	a,#0x1
	lcall	X17ac
	mov	a,#0xcb
	lcall	X17a2
	mov	a,#0x1
	lcall	X17ac
	mov	a,#0xb7
	lcall	X17a2
	mov	a,#0xc8
	lcall	X17ac
	mov	a,#0x25
	lcall	X17a2
	mov	a,#0x30
	lcall	X17ac
	mov	a,#0xd0
	lcall	X17a2
	mov	a,#0x1d
	lcall	X17ac
	mov	a,#0xb5
	lcall	X17a2
	mov	a,#0x1c
	lcall	X17ac
	mov	a,#0xf0
	lcall	X17a2
	mov	a,#0x6
	lcall	X17ac
	mov	a,#0xb
	lcall	X17ac
	mov	a,#0xd
	lcall	X17ac
	mov	a,#0x12
	lcall	X17ac
	mov	a,#0xb4
	lcall	X17a2
	mov	a,#0x18
	lcall	X17ac
	mov	a,#0xd7
	lcall	X17a2
	mov	a,#0xbf
	lcall	X17ac
	mov	a,#0xb5
	lcall	X17a2
	mov	a,#0x1c
	lcall	X17ac
	mov	a,#0xcd
	lcall	X17a2
	mov	a,#0x80
	lcall	X17ac
	mov	a,#0xd0
	lcall	X17a2
	mov	a,#0x1d
	lcall	X17ac
	mov	a,#0xb4
	lcall	X17a2
	mov	a,#0x18
	lcall	X17ac
	mov	a,#0x20
	lcall	X17a2
	mov	a,#0x2a
	lcall	X17a2
X1443:	mov	a,#0x4
	lcall	X17ac
	mov	a,#0x83
X144a:	lcall	X17ac
	mov	a,#0x2b
	lcall	X17a2
	mov	a,#0x0
X1454:	lcall	X17ac
	mov	a,#0x83
	lcall	X17ac
	mov	a,#0x3a
	lcall	X17a2
	mov	a,#0x5
	lcall	X17ac
	mov	a,#0x36
	lcall	X17a2
	mov	a,#0x0
	lcall	X17ac
	mov	a,#0xb0
	lcall	X17a2
	mov	a,#0x83
	lcall	X17ac
	mov	a,#0xf9
	lcall	X17a2
	mov	a,#0x0
X1481:	lcall	X17ac
	mov	a,#0x3
	lcall	X17ac
	mov	a,#0x5
	lcall	X17ac
	mov	a,#0x7
	lcall	X17ac
	mov	a,#0x9
	lcall	X17ac
	mov	a,#0xb
	lcall	X17ac
	mov	a,#0xd
	lcall	X17ac
	mov	a,#0xf
	lcall	X17ac
	mov	a,#0x11
	lcall	X17ac
	mov	a,#0x13
	lcall	X17ac
	mov	a,#0x15
	lcall	X17ac
	mov	a,#0x17
	lcall	X17ac
X14bb:	mov	a,#0x19
	lcall	X17ac
	mov	a,#0x1b
	lcall	X17ac
X14c5:	mov	a,#0x1d
	lcall	X17ac
	mov	a,#0x1f
	lcall	X17ac
X14cf:	mov	a,#0x29
	lcall	X17a2
;
	ret	
;
X178c:	mov	r5,a
X178d:	mov	r6,#0x18
X178f:	mov	r7,#0xfa
X1791:	djnz	r7,X1791
	djnz	r6,X178f
	djnz	r5,X178d
	ret	
;
X17a2:	clr	LCD_A0
	ljmp	otp_lcd_write
;
X17ac:	setb	LCD_A0
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
	.db	0x36, 0x3e, 0x42, 0x46, 0x50, 0x52, 0x58, 0x5c, 0x60, 0x64, 0x68
;
_custom_contrasttbl_len::  .db  . - contrast_table

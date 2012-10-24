	.include 'dpf.inc'

	.area LCDAUX (CODE)

_lcd_custom_init::
	mov	p3dir,#0x0
	anl	p1dir,#0xe8
	anl	p2dir,#0xfd
	clr	LCD_CS
	clr	LCD_RST
	mov	a,#0xc8
	lcall	X171f
	setb	LCD_RST
	mov	a,#0xc8
	lcall	X171f
	mov	a,#0x2c
	lcall	X1735
	mov	a,#0x14
	lcall	X171f
	mov	a,#0x2
	lcall	X1735
	mov	a,#0x1
	lcall	X1735
	mov	a,#0xa
	lcall	X171f
	mov	a,#0x20
	lcall	X1735
	mov	a,#0x5
	lcall	X1735
	mov	a,#0x26
	lcall	X1735
	mov	a,#0x1
	lcall	X1735
	mov	a,#0xa
	lcall	X171f
	mov	a,#0x26
	lcall	X1735
	mov	a,#0x9
	lcall	X1735
	mov	a,#0xa
	lcall	X171f
	mov	a,#0x26
	lcall	X1735
	mov	a,#0xb
	lcall	X1735
	mov	a,#0xa
	lcall	X171f
	mov	a,#0x26
	lcall	X1735
	mov	a,#0xf
	lcall	X1735
	mov	a,#0xa
	lcall	X171f
	mov	a,#0x28
	lcall	X1735
	mov	a,#0x0
	lcall	X1735
	mov	a,#0x45
	lcall	X1735
	mov	a,#0x0
	lcall	X1735
	mov	a,#0x53
	lcall	X1735
	mov	a,#0x0
	lcall	X1735
	mov	a,#0x10
	lcall	X1735
	mov	a,#0x3
	lcall	X1735
	mov	a,#0x24
	lcall	X1735
	mov	a,#0x22
	lcall	X1735
	mov	a,#0x30
	lcall	X1735
	mov	a,#0xd
	lcall	X1735
	mov	a,#0x32
	lcall	X171f
	mov	a,#0x32
	lcall	X1735
	mov	a,#0xc
	lcall	X1735
	mov	a,#0xa
	lcall	X171f
	mov	a,#0x40
	lcall	X1735
	mov	a,#0x80
	lcall	X1735
	mov	a,#0x42
	lcall	X1735
	mov	a,#0x2
	lcall	X1735
	mov	a,#0x83
	lcall	X1735
	mov	a,#0x43
	lcall	X1735
	mov	a,#0x2
	lcall	X1735
	mov	a,#0x7f
	lcall	X1735
	mov	a,#0x34
	lcall	X1735
	mov	a,#0x91
	lcall	X1735
	mov	a,#0x2a
	lcall	X1735
	mov	a,#0x98
	lcall	X1735
	mov	a,#0x2b
	lcall	X1735
	mov	a,#0x88
	lcall	X1735
	mov	a,#0x22
	lcall	X1735
	mov	a,#0x22
	lcall	X1735
	mov	a,#0x36
	lcall	X1735
	mov	a,#0x24
	lcall	X1735
	mov	a,#0x55
	lcall	X1735
	mov	a,#0x0
	lcall	X1735
	mov	a,#0x64
	lcall	X171f
	mov	a,#0x51
	lcall	X1735
	ret	
;
X171f:	mov	r5,a
X1720:	mov	r6,#0x18
X1722:	mov	r7,#0xfa
X1724:	djnz	r7,X1724
	djnz	r6,X1722
	djnz	r5,X1720
	ret	
;
X1735:	clr	LCD_A0
	ljmp	otp_lcd_write
;
_lcd_custom_setcontrast::
	mov	a,#0x2a
	clr	LCD_A0
	lcall	otp_lcd_write
	mov	a,dpl
	dec	a
	mov	dptr,#contrast_table
	movc	a,@a+dptr
	clr	LCD_A0
	ljmp	otp_lcd_write
	
	.area LCDAUX (CODE)

contrast_table:
	.db	0x90, 0x92, 0x94, 0x96, 0x98, 0x9a, 0x9c, 0x9e, 0xa0, 0xa2
;
_custom_contrasttbl_len::  .db  . - contrast_table

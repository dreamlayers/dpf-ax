	.include 'dpf.inc'

	.area LCDAUX (CODE)

_lcd_custom_init::
	mov	p3dir,#0x0
	anl	p1dir,#0xe8
	anl	p2dir,#0xfd
	clr	LCD_CS
	clr	LCD_RST
	mov	a,#0x96
	lcall	X1679
	setb	LCD_RST
	mov	a,#0x96
	lcall	X1679
	mov	a,#0x11
	lcall	X168f
	mov	a,#0x64
	lcall	X1679
	mov	a,#0xd7
	lcall	X168f
	mov	a,#0x18
	lcall	X1699
	mov	a,#0xb7
	lcall	X168f
	mov	a,#0xa
	lcall	X1699
	mov	a,#0xc0
	lcall	X168f
	mov	a,#0x30
	lcall	X1699
	mov	a,#0x1
	lcall	X1699
	mov	a,#0xc3
	lcall	X168f
	mov	a,#0x4
	lcall	X1699
	mov	a,#0xb0
	lcall	X168f
	mov	a,#0x82
	lcall	X1699
	mov	a,#0xb5
	lcall	X168f
	mov	a,#0x0
	lcall	X1699
	mov	a,#0x29
	lcall	X168f
	mov	a,#0xf0
	lcall	X168f
	mov	a,#0xc
	lcall	X1699
	mov	a,#0xc
	lcall	X1699
	mov	a,#0xc
	lcall	X1699
	mov	a,#0xc
	lcall	X1699
	mov	a,#0x2a
	lcall	X168f
	mov	a,#0x0
	lcall	X1699
	mov	a,#0x7f
	lcall	X1699
	mov	a,#0x2b
	lcall	X168f
	mov	a,#0x4
	lcall	X1699
	mov	a,#0x83
	lcall	X1699
	mov	a,#0x2c
	lcall	X168f
	ret	
;
X1679:	mov	r5,a
X167a:	mov	r6,#0x18
X167c:	mov	r7,#0xfa
X167e:	djnz	r7,X167e
	djnz	r6,X167c
	djnz	r5,X167a
	ret	
;
X168f:	clr	LCD_A0
	ljmp	otp_lcd_write
;
X1699:	setb	LCD_A0
	ljmp	otp_lcd_write
;

_lcd_custom_setbacklight::
	mov	a,r3
	jnz	doset
	ret
doset:
	clr	c
	mov	a,#0x40
	subb	a,r3
	subb	a,r3
	mov	tmr1perl,#0xff
	mov	tmr1perh,#0x40
	mov	tmr1pwml,#0xff
	mov	tmr1pwmh,a
	ret
;
_lcd_custom_setcontrast::
	mov	a,#0x25
	lcall	X168f
	mov	dptr,#contrast_table
	mov	a,r3
	dec	a
	movc	a,@a+dptr
	lcall	X1699
	ret

	.area LCDAUX (CODE)


;backlight_table::
;	.db	If needed, put data for custom backlight handling here
;_custom_backlighttbl_len::  .db  . - backlight_table


contrast_table::
	.db	0x38, 0x3a, 0x3c, 0x3e, 0x40, 0x42, 0x44
_custom_contrasttbl_len::  .db  . - contrast_table


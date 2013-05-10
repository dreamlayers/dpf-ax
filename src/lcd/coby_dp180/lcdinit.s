	.include 'dpf.inc'

	.area LCDAUX (CODE)

_lcd_custom_init::
;
;	mov	p3dir,#0x0
;	anl	p1dir,#0xe8
;	anl	p2dir,#0xfd
;	clr	LCD_CS
;	clr	LCD_RST
;	mov	a,#0x96
;	lcall	X1648
;	setb	LCD_RST
;	mov	a,#0x96
;	lcall	X1648
	mov	a,#0x11
	lcall	X165e
	mov	a,#0x96
	lcall	X1648
	mov	a,#0x26
	lcall	X165e
	mov	a,#0x4
	lcall	X1668
	mov	a,#0xf2
	lcall	X165e
	mov	a,#0x0
	lcall	X1668
	mov	a,#0xb1
	lcall	X165e
	mov	a,#0xa
	lcall	X1668
	mov	a,#0x14
	lcall	X1668
	mov	a,#0xc0
	lcall	X165e
	mov	a,#0xa
	lcall	X1668
	mov	a,#0x5
X147d:	lcall	X1668
	mov	a,#0xc1
	lcall	X165e
	mov	a,#0x2
	lcall	X1668
	mov	a,#0xc5
	lcall	X165e
	mov	a,#0x2f
	lcall	X1668
	mov	a,#0x4e
	lcall	X1668
	mov	a,#0xc7
	lcall	X165e
	mov	a,#0x40
	lcall	X1668
	mov	a,#0x2a
	lcall	X165e
	mov	a,#0x0
	lcall	X1668
	mov	a,#0x0
	lcall	X1668
	mov	a,#0x0
	lcall	X1668
	mov	a,#0x7f
	lcall	X1668
	mov	a,#0x2a
	lcall	X165e
	mov	a,#0x0
	lcall	X1668
	mov	a,#0x0
	lcall	X1668
	mov	a,#0x0
	lcall	X1668
	mov	a,#0x9f
	lcall	X1668
	mov	a,#0x36
	lcall	X165e
	mov	a,#0xa8
	lcall	X1668
	mov	a,#0x3a
	lcall	X165e
	mov	a,#0x5
	lcall	X1668
	mov	a,#0x2c
	lcall	X165e
	mov	a,#0x29
	lcall	X165e
	ret	
;
X1648:	mov	r5,a
X1649:	mov	r6,#0x18
X164b:	mov	r7,#0xfa
X164d:	djnz	r7,X164d
	djnz	r6,X164b
	djnz	r5,X1649
	ret	
;
X165e:	clr	LCD_A0
	ljmp	otp_lcd_write
;
X1668:	setb	LCD_A0
	ljmp	otp_lcd_write

_lcd_custom_setbacklight::
;
	mov	dptr,#backlight_table
	mov	a,r3
;	dec	a		; allow value 0!
	movc	a,@a+dptr
	mov	tmr1perl,#0xff
	mov	tmr1perh,#0x0
	mov	tmr1pwml,a
	mov	tmr1pwmh,#0x0
	ret
;
_lcd_custom_setcontrast::
;
	mov	a,#0xc5
	lcall	X165e
	mov	dptr,#contrast_table
	mov	a,r3
	dec	a
	mov	b,#0x2
	mul	ab
	mov	b,a
	movc	a,@a+dptr
X158b:	lcall	X1668
	mov	a,b
	add	a,#0x1
	movc	a,@a+dptr
	lcall	X1668
	ret
;

	.area LCDAUX (CODE)


backlight_table::
	.db	0xff, 0xc8, 0xc3, 0xb9, 0xa5, 0x9b, 0x91, 0x7d, 0x64, 0x5f
	.db	0x55, 0x4b, 0x41, 0x37, 0x2d, 0x23, 0x01
_custom_backlighttbl_len::  .db  . - backlight_table


contrast_table::
	.db	0x52, 0x1b,  0x4d, 0x20,  0x48, 0x25,  0x43, 0x2a,  0x3e, 0x2f
	.db	0x39, 0x34,  0x34, 0x39,  0x2f, 0x3e
_custom_contrasttbl_len::  .db  (. - contrast_table) / 2


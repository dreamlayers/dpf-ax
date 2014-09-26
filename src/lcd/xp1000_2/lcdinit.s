	.include 'dpf.inc'

	.area LCDAUX (CODE)

_lcd_custom_init::
	mov	p3dir,#0x0
	anl	p1dir,#0xe8
	anl	p2dir,#0xfd
	clr	LCD_CS
	clr	LCD_RST
	mov	a,#0x96
	lcall	X1648
	setb	LCD_RST
	mov	a,#0xc8
	lcall	X1648
	mov	a,#0xe2
	lcall	X165e
	mov	a,#0xc8
	lcall	X1648
	mov	a,#0xc8
	lcall	X1648
	mov	a,#0xc8
	lcall	X1648
	mov	a,#0xea
	lcall	X165e
	mov	a,#0x81
	lcall	X165e
	mov	a,#0x84
	lcall	X165e
	mov	a,#0x2f
	lcall	X165e
	mov	a,#0xd1
	lcall	X165e
	mov	a,#0xd6
	lcall	X165e
	mov	a,#0xc0
	lcall	X165e
	mov	a,#0x89
	lcall	X165e
	mov	a,#0xa2
	lcall	X165e
	mov	a,#0xc8
	lcall	X165e
	mov	a,#0x18
	lcall	X165e
	mov	a,#0xde
	lcall	X165e
	mov	a,#0xaf
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
X166d:	setb	LCD_A0
	ljmp	otp_lcd_write
;
_lcd_custom_setbacklight::
	mov	a,r3
	jnz	doset
	ret
doset:
	mov	a,r3
	mov	b,#0xc
	mul	ab
	mov	b,a
	mov	a,#0xff
	clr	c
	subb	a,b
	mov	tmr1perl,#0xff
	mov	tmr1perh,#0x0
	mov	tmr1pwml,a
	mov	tmr1pwmh,#0x0
	ret
;
_lcd_custom_setcontrast::
	mov	a,#0x81
	lcall	X165e
	mov	dptr,#contrast_table
	mov	a,r3
	dec	a
	movc	a,@a+dptr
	lcall	X166d
	ret
;
	.area LCDAUX (CODE)


;backlight_table::
;	.db	If needed, put data for custom backlight handling here
;_custom_backlighttbl_len::  .db  . - backlight_table


contrast_table::
	.db	0x82, 0x89, 0x8f, 0x97, 0xa4, 0xab
_custom_contrasttbl_len::  .db  . - contrast_table


	.include 'dpf.inc'

	.area LCDAUX (CODE)

_lcd_custom_init::
	mov	p3dir,#0x0
	anl	p1dir,#0xe8
	anl	p2dir,#0xcd
	clr	LCD_CS
	clr	LCD_RST
	mov	a,#0x96
	lcall	X1899
	setb	LCD_RST
	mov	a,#0x96
	lcall	X1899
	mov	a,#0x1
	lcall	X18af
	mov	a,#0x64
	lcall	X1899
	mov	a,#0x11
	lcall	X18af
	mov	a,#0xc8
	lcall	X1899
	mov	a,#0xff
	lcall	X18af
	mov	a,#0x40
	lcall	X18b9
	mov	a,#0x1
	lcall	X18b9
	mov	a,#0x1a
	lcall	X18b9
	mov	a,#0xb1
	lcall	X18af
	mov	a,#0x1
	lcall	X18b9
	mov	a,#0x2c
	lcall	X18b9
	mov	a,#0x2d
	lcall	X18b9
	mov	a,#0xb2
	lcall	X18af
	mov	a,#0x1
	lcall	X18b9
	mov	a,#0x2c
	lcall	X18b9
	mov	a,#0x2d
	lcall	X18b9
	mov	a,#0xb3
	lcall	X18af
	mov	a,#0x1
	lcall	X18b9
	mov	a,#0x2c
	lcall	X18b9
	mov	a,#0x2d
	lcall	X18b9
	mov	a,#0x1
	lcall	X18b9
	mov	a,#0x2c
	lcall	X18b9
	mov	a,#0x2d
	lcall	X18b9
	mov	a,#0xb4
	lcall	X18af
	mov	a,#0x7
	lcall	X18b9
	mov	a,#0xb6
	lcall	X18af
	mov	a,#0x15
	lcall	X18b9
	mov	a,#0x2
	lcall	X18b9
	mov	a,#0xc0
	lcall	X18af
	mov	a,#0x2a
	lcall	X18b9
	mov	a,#0x2
	lcall	X18b9
	mov	a,#0x84
	lcall	X18b9
	mov	a,#0xc1
	lcall	X18af
	mov	a,#0xc5
	lcall	X18b9
	mov	a,#0xc2
	lcall	X18af
	mov	a,#0xa
	lcall	X18b9
	mov	a,#0x0
	lcall	X18b9
	mov	a,#0xc3
	lcall	X18af
	mov	a,#0x8a
	lcall	X18b9
	mov	a,#0x2a
	lcall	X18b9
	mov	a,#0xc4
	lcall	X18af
	mov	a,#0x8a
	lcall	X18b9
	mov	a,#0xee
	lcall	X18b9
	mov	a,#0xc5
	lcall	X18af
	mov	a,#0x1a
	lcall	X18b9
	mov	a,#0xc7
	lcall	X18af
	mov	a,#0xb0
	lcall	X18b9
	mov	a,#0x36
	lcall	X18af
	mov	a,#0xc0
	lcall	X18b9
	mov	a,#0x3a
	lcall	X18af
	mov	a,#0x5
	lcall	X18b9
	mov	a,#0xe0
	lcall	X18af
	mov	a,#0xf
	lcall	X18b9
	mov	a,#0x1a
	lcall	X18b9
	mov	a,#0xf
	lcall	X18b9
	mov	a,#0x18
	lcall	X18b9
	mov	a,#0x2f
	lcall	X18b9
	mov	a,#0x28
	lcall	X18b9
	mov	a,#0x20
	lcall	X18b9
	mov	a,#0x22
	lcall	X18b9
	mov	a,#0x1f
	lcall	X18b9
	mov	a,#0x1b
	lcall	X18b9
	mov	a,#0x23
	lcall	X18b9
	mov	a,#0x37
	lcall	X18b9
	mov	a,#0x0
	lcall	X18b9
	mov	a,#0x7
	lcall	X18b9
	mov	a,#0x2
	lcall	X18b9
	mov	a,#0x10
	lcall	X18b9
	mov	a,#0xe1
	lcall	X18af
	mov	a,#0xf
	lcall	X18b9
	mov	a,#0x1b
	lcall	X18b9
	mov	a,#0xf
	lcall	X18b9
	mov	a,#0x17
	lcall	X18b9
	mov	a,#0x33
	lcall	X18b9
	mov	a,#0x2c
	lcall	X18b9
	mov	a,#0x29
	lcall	X18b9
	mov	a,#0x2e
	lcall	X18b9
	mov	a,#0x30
	lcall	X18b9
	mov	a,#0x30
	lcall	X18b9
	mov	a,#0x39
	lcall	X18b9
	mov	a,#0x3f
	lcall	X18b9
	mov	a,#0x0
	lcall	X18b9
	mov	a,#0x7
	lcall	X18b9
	mov	a,#0x3
	lcall	X18b9
	mov	a,#0x10
	lcall	X18b9
	mov	a,#0x3a
	lcall	X18af
	mov	a,#0x5
	lcall	X18b9
	mov	a,#0x29
	lcall	X18af
	mov	a,#0x64
	lcall	X1899
	mov	a,#0x2c
	lcall	X18af
	ret	
;
X1899:	mov	r5,a
X189a:	mov	r6,#0x18
X189c:	mov	r7,#0xfa
X189e:	djnz	r7,X189e
	djnz	r6,X189c
	djnz	r5,X189a
	ret	
;
X18af:	clr	LCD_A0
	ljmp	otp_lcd_write
;
X18b9:	setb	LCD_A0
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
	mov	a,#0x25
	lcall	X18af
	mov	dptr,#contrast_table
	mov	a,r3
	dec	a
	movc	a,@a+dptr
	lcall	X18b9
	ret
;
	.area LCDAUX (CODE)


;backlight_table::
;	.db	If needed, put data for custom backlight handling here
;_custom_backlighttbl_len::  .db  . - backlight_table


contrast_table::
	.db	0x2d, 0x2e, 0x31, 0x33, 0x35, 0x37, 0x39, 0x41, 0x43
	.db	0x44, 0x45, 0x46, 0x47, 0x49, 0x4b, 0x4d

_custom_contrasttbl_len::  .db  . - contrast_table


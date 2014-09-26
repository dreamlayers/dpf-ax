	.include 'dpf.inc'

	.area LCDAUX (CODE)

_lcd_custom_init::
;
X1330:	mov	p3dir,#0x0
	anl	p1dir,#0xe8
	anl	p2dir,#0xfd
	clr	LCD_CS
	clr	LCD_RST
	mov	a,#0x96
	lcall	X16b3
	setb	LCD_RST
	mov	a,#0x96
	lcall	X16b3
	mov	a,#0x1
	lcall	X16c9
	mov	a,#0x96
	lcall	X16b3
	mov	a,#0xd7
	lcall	X16c9
	mov	a,#0x9f
	lcall	X16d3
	mov	a,#0xe0
	lcall	X16c9
	mov	a,#0x0
	lcall	X16d3
	mov	a,#0xa
	lcall	X16b3
	mov	a,#0xe3
	lcall	X16c9
	mov	a,#0x14
	lcall	X16b3
	mov	a,#0xe1
	lcall	X16c9
	mov	a,#0x28
	lcall	X16c9
	mov	a,#0x11
	lcall	X16c9
	mov	a,#0x32
	lcall	X16b3
	mov	a,#0xc3
	lcall	X16c9
	mov	a,#0x3
	lcall	X16d3
	mov	a,#0xc4
	lcall	X16c9
	mov	a,#0x6
	lcall	X16d3
	mov	a,#0xc5
	lcall	X16c9
	mov	a,#0x1
	lcall	X16d3
	mov	a,#0xcb
	lcall	X16c9
	mov	a,#0x1
	lcall	X16d3
	mov	a,#0xb7
	lcall	X16c9
	mov	a,#0x0
	lcall	X16d3
	mov	a,#0xd0
	lcall	X16c9
	mov	a,#0x1d
	lcall	X16d3
	mov	a,#0xb5
	lcall	X16c9
	mov	a,#0x8b
	lcall	X16d3
	mov	a,#0xec
	lcall	X16c9
	mov	a,#0x4
	lcall	X16d3
	mov	a,#0xbd
	lcall	X16c9
	mov	a,#0x2
	lcall	X16d3
	mov	a,#0xf0
	lcall	X16c9
	mov	a,#0x7
	lcall	X16d3
	mov	a,#0xc
	lcall	X16d3
	mov	a,#0xc
	lcall	X16d3
	mov	a,#0x12
	lcall	X16d3
	mov	a,#0x20
	lcall	X16c9
	mov	a,#0x2a
	lcall	X16c9
	mov	a,#0x0
	lcall	X16d3
	mov	a,#0x7f
	lcall	X16d3
	mov	a,#0x2b
	lcall	X16c9
	mov	a,#0x0
	lcall	X16d3
	mov	a,#0x7f
	lcall	X16d3
	mov	a,#0x3a
	lcall	X16c9
	mov	a,#0x5
	lcall	X16d3
	mov	a,#0x36
	lcall	X16c9
	mov	a,#0xa8
	lcall	X16d3
	mov	a,#0xb0
	lcall	X16c9
	mov	a,#0x83
	lcall	X16d3
	mov	a,#0x29
	lcall	X16c9
	mov	a,#0xf9
	lcall	X16c9
	mov	a,#0x0
	lcall	X16d3
	mov	a,#0x2
	lcall	X16d3
	mov	a,#0x4
	lcall	X16d3
	mov	a,#0x6
	lcall	X16d3
	mov	a,#0x8
	lcall	X16d3
	mov	a,#0xa
	lcall	X16d3
	mov	a,#0xc
	lcall	X16d3
	mov	a,#0xe
	lcall	X16d3
	mov	a,#0x10
	lcall	X16d3
	mov	a,#0x12
	lcall	X16d3
	mov	a,#0x14
	lcall	X16d3
	mov	a,#0x16
	lcall	X16d3
	mov	a,#0x18
	lcall	X16d3
	mov	a,#0x1a
	lcall	X16d3
	mov	a,#0x1c
	lcall	X16d3
	mov	a,#0x1e
	lcall	X16d3
	mov	a,#0x2c
	lcall	X16c9
	ret	
;
X16b3:	mov	r5,a
X16b4:	mov	r6,#0x18
X16b6:	mov	r7,#0xfa
X16b8:	djnz	r7,X16b8
	djnz	r6,X16b6
	djnz	r5,X16b4
	ret	
;
X16c9:	clr	LCD_A0
	ljmp	otp_lcd_write
;
X16d3:	setb	LCD_A0
	ljmp	otp_lcd_write
;
_lcd_custom_setbacklight::
	mov	a,r3
	jnz	doset
	ret
doset:
	mov	a,r3
	mov	b,#0x1f
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
	mov	a,#0xc0
	lcall	X16c9
	mov	dptr,#contrast_table
	mov	a,r3
	dec	a
	movc	a,@a+dptr
	lcall	X16d3
	mov	a,#0x0
	lcall	X16d3
	ret
;
	.area LCDAUX (CODE)


;backlight_table::
;	.db	If needed, put data for custom backlight handling here
;_custom_backlighttbl_len::  .db  . - backlight_table


contrast_table::
	.db	0xdd, 0xe0, 0xe3, 0xe6, 0xe9, 0xed, 0xf0, 0xf3
	.db	0xf6, 0xf8, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff

_custom_contrasttbl_len::  .db  . - contrast_table


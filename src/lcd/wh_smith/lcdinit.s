	.include 'dpf.inc'

	.area LCDAUX (CODE)

rd  = 0xb7

_lcd_custom_init::
	setb	LCD_RST
	anl	p2dir,#0xfd
	anl	p1dir,#0xfe
	clr	LCD_CS
	clr	LCD_RST
	mov	a,#0x64
	lcall	X16b8
	setb	LCD_RST
	mov	a,#0x64
	lcall	X16b8
	setb	LCD_CS
	mov	a,#0x1
	lcall	X16e8
	mov	a,#0x96
	lcall	X16b8
	mov	a,#0xd7
	lcall	X16e8
	mov	a,#0x9f
	lcall	X16e4
	mov	a,#0xe0
	lcall	X16e8
	mov	a,#0x0
	lcall	X16e4
	mov	a,#0x96
	lcall	X16b8
	mov	a,#0xe3
	lcall	X16e8
	mov	a,#0x96
	lcall	X16b8
	mov	a,#0xe1
	lcall	X16e8
	mov	a,#0x28
	lcall	X16e8
	mov	a,#0x11
	lcall	X16e8
	mov	a,#0x78
	lcall	X16b8
	mov	a,#0xc0
	lcall	X16e8
	mov	a,#0xff
	lcall	X16e4
	mov	a,#0x0
	lcall	X16e4
	mov	a,#0x25
	lcall	X16e8
	mov	a,#0x3f
	lcall	X16e4
	mov	a,#0xc3
	lcall	X16e8
	mov	a,#0x0
	lcall	X16e4
	mov	a,#0xc4
	lcall	X16e8
	mov	a,#0x6
	lcall	X16e4
	mov	a,#0xc5
	lcall	X16e8
	mov	a,#0x1
	lcall	X16e4
	mov	a,#0xcb
	lcall	X16e8
	mov	a,#0x1
	lcall	X16e4
	mov	a,#0xd0
	lcall	X16e8
	mov	a,#0x1d
	lcall	X16e4
	mov	a,#0xb5
	lcall	X16e8
	mov	a,#0x89
	lcall	X16e4
	mov	a,#0xbd
	lcall	X16e8
	mov	a,#0x2
	lcall	X16e4
	mov	a,#0xf0
	lcall	X16e8
	mov	a,#0x6
	lcall	X16e4
	mov	a,#0xb
	lcall	X16e4
	mov	a,#0xd
	lcall	X16e4
	mov	a,#0x15
	lcall	X16e4
	mov	a,#0x36
	lcall	X16e8
	mov	a,#0x88
	lcall	X16e4
	mov	a,#0x3a
	lcall	X16e8
	mov	a,#0x5
	lcall	X16e4
	mov	a,#0xb0
	lcall	X16e8
	mov	a,#0x7f
	lcall	X16e4
	mov	a,#0x2a
	lcall	X16e8
	mov	a,#0x0
	lcall	X16e4
	mov	a,#0x7f
	lcall	X16e4
	mov	a,#0x2b
	lcall	X16e8
	mov	a,#0x0
	lcall	X16e4
	mov	a,#0x7f
	lcall	X16e4
	mov	a,#0xf9
	lcall	X16e8
	mov	a,#0x0
	lcall	X16e4
	mov	a,#0x2
	lcall	X16e4
	mov	a,#0x4
	lcall	X16e4
	mov	a,#0x6
	lcall	X16e4
	mov	a,#0x8
	lcall	X16e4
	mov	a,#0xa
	lcall	X16e4
	mov	a,#0xc
	lcall	X16e4
	mov	a,#0xe
	lcall	X16e4
	mov	a,#0x10
	lcall	X16e4
	mov	a,#0x12
	lcall	X16e4
	mov	a,#0x14
	lcall	X16e4
	mov	a,#0x16
	lcall	X16e4
	mov	a,#0x18
	lcall	X16e4
	mov	a,#0x1a
	lcall	X16e4
	mov	a,#0x1c
	lcall	X16e4
	mov	a,#0x1e
	lcall	X16e4
	mov	a,#0x29
	lcall	X16e8
	mov	a,#0x2c
	lcall	X16e8
	ret	
;
X16b8:	mov	r5,a
X16b9:	mov	r6,#0x18
X16bb:	mov	r7,#0xfa
X16bd:	djnz	r7,X16bd
	djnz	r6,X16bb
	djnz	r5,X16b9
	ret	
;
X1630:	lcall	X16c4
	clr	LCD_CS
	mov	LCD_A0,c
	setb	c
	lcall	X16ec
X163b:	clr	LCD_RD
	rlc	a
	jz	X164a
	mov	rd,c
	lcall	X16ec
	setb	LCD_RD
	clr	c
	sjmp	X163b
;
X164a:	setb	LCD_CS
	ret	
;
X16c4:	anl	p1dir,#0xfb
	anl	p3dir,#0x7f
	anl	p1dir,#0xef
	clr	LCD_RD
	ret	

;
X16e4:	setb	c
	ljmp	X1630
;
X16e8:	clr	c
	ljmp	X1630
;
X16ec:	ret	
;
_lcd_custom_setbacklight::
	mov	a,r3
	jnz	doset
	ret
doset:	mov	a,r3
	mov	b,#0x3
	mul	ab
	mov	b,a
	mov	a,#0x3f
	clr	c
	subb	a,b
	mov	b,a
	mov	tmr1perl,#0xff
	mov	tmr1perh,#0x40
	mov	tmr1pwml,a
	mov	tmr1pwmh,b
	ret
;
_lcd_custom_setcontrast::
	mov	a,#0x25
	lcall	X16e8
	mov	dptr,#contrast_table
	mov	a,r3
	dec	a
	movc	a,@a+dptr
	lcall	X16e4
	ret
;
	.area LCDAUX (CODE)


;backlight_table::
;	.db	If needed, put data for custom backlight handling here
;_custom_backlighttbl_len::  .db  . - backlight_table


contrast_table::
	.db	0x2a, 0x2e, 0x33, 0x37, 0x3b, 0x3f, 0x44
_custom_contrasttbl_len::  .db  . - contrast_table


	.include 'dpf.inc'

	.area LCDAUX (CODE)

_lcd_custom_init::
	mov	p3dir,#0x0
	anl	p1dir,#0xe8
	anl	p2dir,#0xfd
	clr	LCD_CS
	clr	LCD_RST
	mov	a,#0x96
	lcall	X17d0
	setb	LCD_RST
	mov	a,#0x96
	lcall	X17d0
	mov	a,#0x1
	lcall	X17e6
	mov	a,#0x96
	lcall	X17d0
	mov	a,#0xd7
	lcall	X17e6
	mov	a,#0xbf
	lcall	X17f0
	mov	a,#0xe0
	lcall	X17e6
	mov	a,#0x0
	lcall	X17f0
	mov	a,#0xa
	lcall	X17d0
	mov	a,#0xe3
	lcall	X17e6
	mov	a,#0x14
	lcall	X17d0
	mov	a,#0xe1
	lcall	X17e6
	mov	a,#0x28
	lcall	X17e6
	mov	a,#0x11
	lcall	X17e6
	mov	a,#0x32
	lcall	X17d0
	mov	a,#0xc0
	lcall	X17e6
	mov	a,#0x1a
	lcall	X17f0
	mov	a,#0x1
	lcall	X17f0
	mov	a,#0xc3
	lcall	X17e6
	mov	a,#0x1
	lcall	X17f0
	mov	a,#0xc4
	lcall	X17e6
	mov	a,#0x5
	lcall	X17f0
	mov	a,#0xc5
	lcall	X17e6
	mov	a,#0x1
	lcall	X17f0
	mov	a,#0xc6
	lcall	X17e6
	mov	a,#0x0
	lcall	X17f0
	mov	a,#0xcb
	lcall	X17e6
	mov	a,#0x1
	lcall	X17f0
	mov	a,#0xcc
	lcall	X17e6
	mov	a,#0x45
	lcall	X17f0
	mov	a,#0xce
	lcall	X17e6
	mov	a,#0x0
	lcall	X17f0
	mov	a,#0xb7
	lcall	X17e6
	mov	a,#0xc8
	lcall	X17f0
	mov	a,#0xd0
	lcall	X17e6
	mov	a,#0x1d
	lcall	X17f0
	mov	a,#0xd7
	lcall	X17e6
	mov	a,#0xbf
	lcall	X17f0
	mov	a,#0xd0
	lcall	X17e6
	mov	a,#0x1d
	lcall	X17f0
	mov	a,#0xb5
	lcall	X17e6
	mov	a,#0x9c
	lcall	X17f0
	mov	a,#0xcd
	lcall	X17e6
	mov	a,#0xd2
	lcall	X17f0
	mov	a,#0xd0
	lcall	X17e6
	mov	a,#0x1d
	lcall	X17f0
	mov	a,#0xb4
	lcall	X17e6
	mov	a,#0x18
	lcall	X17f0
	mov	a,#0x20
	lcall	X17e6
	mov	a,#0xf7
	lcall	X17e6
	mov	a,#0x6
	lcall	X17f0
	mov	a,#0x13
	lcall	X17e6
	mov	a,#0x3a
	lcall	X17e6
	mov	a,#0x5
	lcall	X17f0
	mov	a,#0x36
	lcall	X17e6
	mov	a,#0x60
	lcall	X17f0
	mov	a,#0xf9
	lcall	X17e6
	mov	a,#0x0
	lcall	X17f0
	mov	a,#0x2
	lcall	X17f0
	mov	a,#0x4
	lcall	X17f0
	mov	a,#0x6
	lcall	X17f0
	mov	a,#0x8
	lcall	X17f0
	mov	a,#0xa
	lcall	X17f0
	mov	a,#0xc
	lcall	X17f0
	mov	a,#0xe
	lcall	X17f0
	mov	a,#0x10
	lcall	X17f0
	mov	a,#0x12
	lcall	X17f0
	mov	a,#0x14
	lcall	X17f0
	mov	a,#0x16
	lcall	X17f0
	mov	a,#0x18
	lcall	X17f0
	mov	a,#0x1a
	lcall	X17f0
	mov	a,#0x1c
	lcall	X17f0
	mov	a,#0x1e
	lcall	X17f0
	mov	a,#0x2a
	lcall	X17e6
	mov	a,#0x4
	lcall	X17f0
	mov	a,#0x83
	lcall	X17f0
	mov	a,#0x2b
	lcall	X17e6
	mov	a,#0x4
X14c2:	lcall	X17f0
	mov	a,#0x83
	lcall	X17f0
	mov	a,#0x29
	lcall	X17e6
	mov	a,#0x38
	lcall	X17e6
	mov	a,#0x25
	lcall	X17e6
	mov	a,#0x3f
	lcall	X17f0
	mov	a,#0xb0
	lcall	X17e6
	mov	a,#0x83
	lcall	X17f0
	mov	a,#0xb1
	lcall	X17e6
	mov	a,#0x0
	lcall	X17f0
	mov	a,#0xb3
	lcall	X17e6
	mov	a,#0x0
	lcall	X17f0
	mov	a,#0xf0
	lcall	X17e6
	mov	a,#0x6
	lcall	X17f0
	mov	a,#0xb
	lcall	X17f0
	mov	a,#0x14
	lcall	X17f0
	mov	a,#0x1b
	lcall	X17f0
	mov	a,#0xf3
	lcall	X17e6
	mov	a,#0x4
	lcall	X17f0
	mov	a,#0xf2
	lcall	X17e6
	mov	a,#0x1e
	lcall	X17f0
	mov	a,#0x28
	lcall	X17f0
	mov	a,#0x32
	lcall	X17f0
	mov	a,#0xf4
	lcall	X17e6
	mov	a,#0x0
	lcall	X17f0
	mov	a,#0x55
	lcall	X17f0
	mov	a,#0x60
	lcall	X17f0
	mov	a,#0xf0
	lcall	X17f0
	mov	a,#0xa3
	lcall	X17f0
	mov	a,#0xf9
	lcall	X17f0
	mov	a,#0x6f
	lcall	X17f0
	mov	a,#0xff
	lcall	X17f0
	mov	a,#0xf9
	lcall	X17e6
	mov	a,#0x0
	lcall	X17f0
	mov	a,#0x3
	lcall	X17f0
	mov	a,#0x5
	lcall	X17f0
	mov	a,#0x7
	lcall	X17f0
	mov	a,#0x9
	lcall	X17f0
	mov	a,#0xb
	lcall	X17f0
	mov	a,#0xd
	lcall	X17f0
	mov	a,#0xf
	lcall	X17f0
	mov	a,#0x11
	lcall	X17f0
	mov	a,#0x13
	lcall	X17f0
	mov	a,#0x15
	lcall	X17f0
	mov	a,#0x17
	lcall	X17f0
	mov	a,#0x19
	lcall	X17f0
	mov	a,#0x1b
	lcall	X17f0
	mov	a,#0x1d
	lcall	X17f0
	mov	a,#0x1f
	lcall	X17f0
	mov	a,#0x2c
	lcall	X17e6
	ret	
;
X17d0:	mov	r5,a
X17d1:	mov	r6,#0x18
X17d3:	mov	r7,#0xfa
X17d5:	djnz	r7,X17d5
	djnz	r6,X17d3
	djnz	r5,X17d1
	ret	
;
X17e6:	clr	LCD_A0
	ljmp	otp_lcd_write
;
X17f0:	setb	LCD_A0
	ljmp	otp_lcd_write
;

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

_lcd_custom_setcontrast::
;
	mov	a,#0xc0
	lcall	X17e6
	mov	dptr,#contrast_table
	mov	a,r3
	dec	a
	movc	a,@a+dptr
	lcall	X17f0
	mov	a,#0x1
	lcall	X17f0
	ret
;

	.area LCDAUX (CODE)

contrast_table::
	.db	0x00, 0x02, 0x04, 0x06, 0x08, 0x0a, 0x0d, 0x10
	.db	0x14, 0x18, 0x1d, 0x21, 0x24, 0x28, 0x2c, 0x2f
_custom_contrasttbl_len::  .db  . - contrast_table


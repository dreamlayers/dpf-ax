	.include 'dpf.inc'

	.area LCDAUX (CODE)

_lcd_custom_init::
	mov	p3dir,#0x0
	anl	p1dir,#0xe8
	anl	p2dir,#0xfd
	clr	LCD_CS
	clr	LCD_RST
	mov	a,#0x96
	lcall	X17fb
	setb	LCD_RST
	mov	a,#0x96
	lcall	X17fb
	mov	a,#0x1
	lcall	X1811
	mov	a,#0x96
	lcall	X17fb
	mov	a,#0xd7
	lcall	X1811
	mov	a,#0x9f
	lcall	X181b
	mov	a,#0xe0
	lcall	X1811
	mov	a,#0x0
	lcall	X181b
	mov	a,#0xa
	lcall	X17fb
	mov	a,#0xe3
	lcall	X1811
	mov	a,#0x14
	lcall	X17fb
	mov	a,#0xe1
	lcall	X1811
	mov	a,#0xca
	lcall	X1811
	mov	a,#0x6
	lcall	X181b
	mov	a,#0xc8
	lcall	X1811
	mov	a,#0x10
	lcall	X181b
	mov	a,#0xc9
	lcall	X1811
	mov	a,#0x20
	lcall	X181b
	mov	a,#0xf5
	lcall	X1811
	mov	a,#0x88
	lcall	X181b
	mov	a,#0xc5
	lcall	X1811
	mov	a,#0x21
	lcall	X181b
	mov	a,#0xd0
	lcall	X1811
	mov	a,#0x1d
	lcall	X181b
	mov	a,#0xc3
	lcall	X1811
	mov	a,#0x0
	lcall	X181b
	mov	a,#0xc4
	lcall	X1811
	mov	a,#0x6
	lcall	X181b
	mov	a,#0xc5
	lcall	X1811
	mov	a,#0x21
	lcall	X181b
	mov	a,#0xcb
	lcall	X1811
	mov	a,#0x1
	lcall	X181b
	mov	a,#0xcc
	lcall	X1811
	mov	a,#0x45
	lcall	X181b
	mov	a,#0xce
	lcall	X1811
	mov	a,#0x0
	lcall	X181b
	mov	a,#0xb7
	lcall	X1811
	mov	a,#0xc8
	lcall	X181b
	mov	a,#0xd0
	lcall	X1811
	mov	a,#0x1d
	lcall	X181b
	mov	a,#0xd7
	lcall	X1811
	mov	a,#0xbf
	lcall	X181b
	mov	a,#0xb5
	lcall	X1811
	mov	a,#0x1
	lcall	X181b
	mov	a,#0xcd
	lcall	X1811
	mov	a,#0x80
	lcall	X181b
	mov	a,#0xd0
	lcall	X1811
	mov	a,#0x1d
	lcall	X181b
	mov	a,#0xb4
	lcall	X1811
	mov	a,#0x18
	lcall	X181b
	mov	a,#0x11
	lcall	X1811
	mov	a,#0x20
	lcall	X1811
	mov	a,#0x38
	lcall	X1811
	mov	a,#0x25
	lcall	X1811
	mov	a,#0x4c
	lcall	X181b
	mov	a,#0x13
	lcall	X1811
	mov	a,#0x34
	lcall	X1811
	mov	a,#0x33
	lcall	X1811
	mov	a,#0x0
	lcall	X181b
	mov	a,#0x80
	lcall	X181b
	mov	a,#0x0
	lcall	X181b
	mov	a,#0x37
	lcall	X1811
	mov	a,#0x0
	lcall	X181b
	mov	a,#0x3a
	lcall	X1811
	mov	a,#0x5
	lcall	X181b
	mov	a,#0x36
	lcall	X1811
	mov	a,#0x0
	lcall	X181b
	mov	a,#0x29
	lcall	X1811
	mov	a,#0xb0
	lcall	X1811
	mov	a,#0x7f
	lcall	X181b
	mov	a,#0xb1
	lcall	X1811
	mov	a,#0x0
	lcall	X181b
	mov	a,#0xb3
	lcall	X1811
	mov	a,#0x0
	lcall	X181b
	mov	a,#0xc0
	lcall	X1811
	mov	a,#0xff
	lcall	X181b
	mov	a,#0x0
	lcall	X181b
	mov	a,#0xc6
	lcall	X1811
	mov	a,#0x0
	lcall	X181b
	mov	a,#0xb7
	lcall	X1811
	mov	a,#0xc8
	lcall	X181b
	mov	a,#0xf9
	lcall	X1811
	mov	a,#0x0
	lcall	X181b
	mov	a,#0x2
	lcall	X181b
	mov	a,#0x4
	lcall	X181b
	mov	a,#0x6
	lcall	X181b
	mov	a,#0x8
	lcall	X181b
	mov	a,#0xa
	lcall	X181b
	mov	a,#0xc
	lcall	X181b
	mov	a,#0xe
	lcall	X181b
	mov	a,#0x10
	lcall	X181b
	mov	a,#0x12
	lcall	X181b
	mov	a,#0x14
	lcall	X181b
	mov	a,#0x16
	lcall	X181b
	mov	a,#0x18
	lcall	X181b
	mov	a,#0x1a
	lcall	X181b
	mov	a,#0x1c
	lcall	X181b
	mov	a,#0x1e
	lcall	X181b
	mov	a,#0x2a
	lcall	X1811
	mov	a,#0x0
	lcall	X181b
	mov	a,#0x7f
	lcall	X181b
	mov	a,#0x2b
	lcall	X1811
	mov	a,#0x4
	lcall	X181b
	mov	a,#0x83
	lcall	X181b
	mov	a,#0x2c
	lcall	X1811
	ret
;
X1811:	clr	LCD_A0
	ljmp	otp_lcd_write
;
X181b:	setb	LCD_A0
	ljmp	otp_lcd_write
;
X17fb:	mov	r5,a
X17fc:	mov	r6,#0x18
X17fe:	mov	r7,#0xfa
X1800:	djnz	r7,X1800
	djnz	r6,X17fe
	djnz	r5,X17fc
	ret	
;
_lcd_custom_setbacklight::
	mov	a,r3
	jnz	doset
	ret
doset:	clr	c
	mov	a,#0x15
	subb	a,r3
	mov	tmr1perl,#0xff
	mov	tmr1perh,#0x40
	mov	tmr1pwml,#0xff
	mov	tmr1pwmh,a
	ret
;
; If custom contrast handling is needed, uncomment the following label
; and set LCD_CONTRAST_CUSTOM in dpfmodel.h
; r3 contains contrast value (1 .. LCD_MAX_CONTRAST_VALUE)
;
;_lcd_custom_setcontrast::

	.area LCDAUX (CODE)


;backlight_table::
;	.db	If needed, put data for custom backlight handling here
;_custom_backlighttbl_len::  .db  . - backlight_table


;contrast_table::
;	.db	If needed, put data for custom contrast handling here
;_custom_contrasttbl_len::  .db  . - contrast_table


	.include 'dpf.inc'

	.area LCDAUX (CODE)

_lcd_custom_init::
	mov	p3dir,#0x0
	anl	p1dir,#0xe8
	anl	p2dir,#0xfd
	clr	LCD_CS
	clr	LCD_RST
	mov	a,#0x96
	lcall	X1695
	setb	LCD_RST
	mov	a,#0x96
	lcall	X1695
	mov	a,#0xee
	lcall	X16ab
	mov	a,#0xb0
	lcall	X16ab
	mov	a,#0x3
	lcall	X16b5
	mov	a,#0x6
	lcall	X16b5
	mov	a,#0xc7
	lcall	X16ab
	mov	a,#0x4
	lcall	X16b5
	mov	a,#0x2a
	lcall	X16ab
	mov	a,#0x4
	lcall	X16b5
	mov	a,#0x83
	lcall	X16b5
	mov	a,#0x2b
	lcall	X16ab
	mov	a,#0x4
	lcall	X16b5
	mov	a,#0x83
	lcall	X16b5
	mov	a,#0xc5
	lcall	X16ab
	mov	a,#0xa
	lcall	X16b5
	mov	a,#0xa
	lcall	X16b5
	mov	a,#0xa
	lcall	X16b5
	mov	a,#0xa
	lcall	X16b5
	mov	a,#0xc3
	lcall	X16ab
	mov	a,#0x35
	lcall	X16b5
	mov	a,#0xc4
	lcall	X16ab
	mov	a,#0x74
	lcall	X16b5
	mov	a,#0xb9
	lcall	X16ab
	mov	a,#0x21
	lcall	X16ab
	mov	a,#0x36
	lcall	X16ab
	mov	a,#0x0
	lcall	X16b5
	mov	a,#0x3a
	lcall	X16ab
	mov	a,#0x5
	lcall	X16b5
	mov	a,#0xc9
	lcall	X16ab
	mov	a,#0xd
	lcall	X16b5
	mov	a,#0xc2
	lcall	X16ab
	mov	a,#0x0
	lcall	X16b5
	mov	a,#0xb4
	lcall	X16ab
	mov	a,#0xb6
	lcall	X16ab
	mov	a,#0x11
	lcall	X16ab
	ret	
;
X1695:	mov	r5,a
X1696:	mov	r6,#0x18
X1698:	mov	r7,#0xfa
X169a:	djnz	r7,X169a
	djnz	r6,X1698
	djnz	r5,X1696
	ret	
;
X16ab:	clr	LCD_A0
	ljmp	otp_lcd_write
;
X16b5:	setb	LCD_A0
	ljmp	otp_lcd_write
;
_lcd_custom_setbacklight::
	mov	a,r3
	jnz	doset
	ret
doset:	clr	c
	mov	a,#0x15
	subb	a,r3
	mov	b,#0xb4
	mul	ab
	subb	a,r3
	mov	tmr1perl,#0xff
	mov	tmr1perh,#0x40
	mov	tmr1pwml,a
	mov	tmr1pwmh,b
	ret
;
_lcd_custom_setcontrast::
	mov	a,#0x25
	lcall	X16ab
	mov	dptr,#contrast_table
	mov	a,r3
	dec	a
	movc	a,@a+dptr
	lcall	X16b5
	ret

	.area LCDAUX (CODE)


;backlight_table::
;	.db	If needed, put data for custom backlight handling here
;_custom_backlighttbl_len::  .db  . - backlight_table


contrast_table::
	.db	0x2c, 0x2e, 0x30, 0x32, 0x34, 0x36, 0x38
_custom_contrasttbl_len::  .db  . - contrast_table


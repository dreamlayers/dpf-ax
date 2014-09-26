	.include 'dpf.inc'

	.area LCDAUX (CODE)

_lcd_custom_init::
	mov	p3dir,#0x0
	anl	p1dir,#0xe8
	anl	p2dir,#0xfd
	clr	LCD_CS
	clr	LCD_RST
	mov	a,#0x96
	lcall	X16ae
	setb	LCD_RST
	mov	a,#0x96
	lcall	X16ae
	mov	a,#0x94
	lcall	X16c4
	mov	a,#0xd1
	lcall	X16c4
	mov	a,#0x1e
	lcall	X16ae
	mov	a,#0x20
	lcall	X16c4
	mov	a,#0xf
	lcall	X16ce
	mov	a,#0xfb
	lcall	X16c4
	mov	a,#0x3
	lcall	X16ce
	mov	a,#0xf7
	lcall	X16c4
	mov	a,#0x28
	lcall	X16ce
	mov	a,#0x2e
	lcall	X16ce
	mov	a,#0x5
	lcall	X16ce
	mov	a,#0x82
	lcall	X16c4
	mov	a,#0x0
	lcall	X16ce
	mov	a,#0xbb
	lcall	X16c4
	mov	a,#0x1
	lcall	X16ce
	mov	a,#0xbc
	lcall	X16c4
	mov	a,#0x1
	lcall	X16ce
	mov	a,#0x1
	lcall	X16ce
	mov	a,#0x0
	lcall	X16ce
	mov	a,#0xf2
	lcall	X16c4
	mov	a,#0x3
	lcall	X16ce
	mov	a,#0x0
	lcall	X16ce
	mov	a,#0x81
	lcall	X16c4
	mov	a,#0x3a
	lcall	X16ce
	mov	a,#0x6
	lcall	X16ce
	mov	a,#0xca
	lcall	X16c4
	mov	a,#0x0
	lcall	X16ce
	mov	a,#0x20
	lcall	X16ce
	mov	a,#0x0
	lcall	X16ce
	mov	a,#0x15
	lcall	X16c4
	mov	a,#0x0
	lcall	X16ce
	mov	a,#0x7f
	lcall	X16ce
	mov	a,#0x5c
	lcall	X16c4
	mov	a,#0x75
	lcall	X16c4
	mov	a,#0x0
	lcall	X16ce
	mov	a,#0x7f
	lcall	X16ce
	mov	a,#0xab
	lcall	X16c4
	mov	a,#0x0
	lcall	X16ce
	mov	a,#0xa7
	lcall	X16c4
	mov	a,#0xaf
	lcall	X16c4
	ret	
;
X16ae:	mov	r5,a
X16af:	mov	r6,#0x18
X16b1:	mov	r7,#0xfa
X16b3:	djnz	r7,X16b3
	djnz	r6,X16b1
	djnz	r5,X16af
	ret	
;
X16c4:	clr	LCD_A0
	ljmp	otp_lcd_write
;
X16ce:	setb	LCD_A0
	ljmp	otp_lcd_write
;
;
_lcd_custom_setbacklight::
	mov	a,r3
	jnz	doset
	ret
doset:
	clr	c
	mov	a,#0x15
	subb	a,r3
	mov	tmr1perl,#0xff
	mov	tmr1perh,#0x40
	mov	tmr1pwml,#0xff
	mov	tmr1pwmh,a
	ret

_lcd_custom_setcontrast::
	mov	a,#0x81
	lcall	X16c4
	mov	dptr,#contrast_table
	mov	a,r3
	dec	a
	movc	a,@a+dptr
	lcall	X16ce
	ret

	.area LCDAUX (CODE)


;backlight_table::
;	.db	If needed, put data for custom backlight handling here
;_custom_backlighttbl_len::  .db  . - backlight_table


contrast_table::
	.db	0x24, 0x26, 0x28, 0x2a, 0x2c, 0x2e, 0x30
_custom_contrasttbl_len::  .db  . - contrast_table


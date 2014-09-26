	.include 'dpf.inc'

	.area LCDAUX (CODE)

_lcd_custom_init::
	mov	p3dir,#0x0
	anl	p1dir,#0xe8
	anl	p2dir,#0xfd
	clr	LCD_CS
	clr	LCD_RST
	mov	a,#0x96
	lcall	X16cf
	setb	LCD_RST
	mov	a,#0x96
	lcall	X16cf
	mov	a,#0x94
	lcall	X16e5
	mov	a,#0xd1
	lcall	X16e5
	mov	a,#0x1e
	lcall	X16cf
	mov	a,#0x20
	lcall	X16e5
	mov	a,#0xf
	lcall	X16ef
	mov	a,#0xfb
	lcall	X16e5
	mov	a,#0x2
	lcall	X16ef
	mov	a,#0xf7
	lcall	X16e5
	mov	a,#0x28
	lcall	X16ef
	mov	a,#0x2e
	lcall	X16ef
	mov	a,#0x5
	lcall	X16ef
	mov	a,#0x82
	lcall	X16e5
	mov	a,#0x0
	lcall	X16ef
	mov	a,#0xbb
	lcall	X16e5
	mov	a,#0x1
	lcall	X16ef
	mov	a,#0xbc
	lcall	X16e5
	mov	a,#0x1
	lcall	X16ef
	mov	a,#0x1
	lcall	X16ef
	mov	a,#0x0
	lcall	X16ef
	mov	a,#0xf2
	lcall	X16e5
	mov	a,#0x3
	lcall	X16ef
	mov	a,#0x0
	lcall	X16ef
	mov	a,#0x81
	lcall	X16e5
	mov	a,#0x20
	lcall	X16ef
	mov	a,#0x6
	lcall	X16ef
	mov	a,#0xca
	lcall	X16e5
	mov	a,#0x0
	lcall	X16ef
	mov	a,#0x20
	lcall	X16ef
	mov	a,#0x0
	lcall	X16ef
	mov	a,#0x15
	lcall	X16e5
	mov	a,#0x0
	lcall	X16ef
	mov	a,#0x83
	lcall	X16ef
	mov	a,#0x5c
	lcall	X16e5
	mov	a,#0x75
	lcall	X16e5
	mov	a,#0x0
	lcall	X16ef
	mov	a,#0x83
	lcall	X16ef
	mov	a,#0xab
	lcall	X16e5
	mov	a,#0x0
	lcall	X16ef
	mov	a,#0xa7
	lcall	X16e5
	mov	a,#0xaf
	lcall	X16e5
	ret	
;
X16cf:	mov	r5,a
X16d0:	mov	r6,#0x18
X16d2:	mov	r7,#0xfa
X16d4:	djnz	r7,X16d4
	djnz	r6,X16d2
	djnz	r5,X16d0
	ret	
;
X16e5:	clr	LCD_A0
	ljmp	otp_lcd_write
;
X16ef:	setb	LCD_A0
	ljmp	otp_lcd_write
;
_lcd_custom_setbacklight::
	mov	a,r3
	jnz	doset
	ret
doset:
	mov	dptr,#backlight_table
	mov	a,r3
	dec	a
	movc	a,@a+dptr
	mov	tmr1perl,#0xff
	mov	tmr1perh,#0x0
	mov	tmr1pwml,a
	mov	tmr1pwmh,#0x0
	ret

_lcd_custom_setcontrast::
	mov	a,#0x81
	lcall	X16e5
	mov	dptr,#contrast_table
	mov	a,r3
	dec	a
	movc	a,@a+dptr
	lcall	X16ef
	ret
;

	.area LCDAUX (CODE)


backlight_table::
	.db	0xe1, 0xda, 0xd2, 0xc5, 0xbe, 0xb6, 0xaf, 0xa8
	.db	0xa0, 0x99, 0x8f, 0x89, 0x6e, 0x5a, 0x46, 0x32
	.db	0x23, 0x19, 0x0f, 0x07, 0x02
_custom_backlighttbl_len::  .db  . - backlight_table


contrast_table::
	.db	0x08, 0x0a, 0x0c, 0x0e, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1a
_custom_contrasttbl_len::  .db  . - contrast_table


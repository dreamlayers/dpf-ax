	.include 'dpf.inc'

	.area LCDAUX (CODE)

_lcd_custom_init::
	mov	p3dir,#0x0
	anl	p1dir,#0xe8
	anl	p2dir,#0xfd
	clr	LCD_CS
	clr	LCD_RST
	mov	a,#0x96
	lcall	X164d
	setb	LCD_RST
	mov	a,#0xc8
	lcall	X164d
	mov	a,#0xb8
	lcall	X1663
	mov	a,#0x0
	lcall	X1663
	mov	a,#0xe2
	lcall	X1663
	mov	a,#0xc8
	lcall	X164d
	mov	a,#0x24
	lcall	X1663
	mov	a,#0x2a
	lcall	X1663
	mov	a,#0x2f
	lcall	X1663
	mov	a,#0xc0
	lcall	X1663
	mov	a,#0x89
	lcall	X1663
	mov	a,#0xa2
	lcall	X1663
	mov	a,#0xd1
	lcall	X1663
	mov	a,#0xd6
	lcall	X1663
	mov	a,#0xe9
	lcall	X1663
	mov	a,#0x81
	lcall	X1663
	mov	a,#0xb0
	lcall	X1663
	mov	a,#0xc8
	lcall	X1663
	mov	a,#0x56
	lcall	X1663
	mov	a,#0xdc
	lcall	X1663
	mov	a,#0xaf
	lcall	X1663
	mov	a,#0xc8
	lcall	X164d
	ret	
;
X164d:	mov	r5,a
X164e:	mov	r6,#0x18
X1650:	mov	r7,#0xfa
X1652:	djnz	r7,X1652
	djnz	r6,X1650
	djnz	r5,X164e
	ret	
;
X1663:	clr	LCD_A0
	ljmp	otp_lcd_write
;
X1672:	setb	LCD_A0
	ljmp	otp_lcd_write
;
_lcd_custom_setbacklight::
	mov	a,r3
	jnz	doset
	ret
doset:
	clr	c
	mov	a,#0x40
	subb	a,r3
	subb	a,#0x6
	subb	a,r3
	mov	tmr1perl,#0xff
	mov	tmr1perh,#0x40
	mov	tmr1pwml,#0xff
	mov	tmr1pwmh,a
	ret
;
_lcd_custom_setcontrast::
	mov	a,#0x81
	lcall	X1663
	mov	dptr,#contrast_table
	mov	a,r3
	dec	a
	movc	a,@a+dptr
	lcall	X1672
	ret
;
	.area LCDAUX (CODE)


;backlight_table::
;	.db	If needed, put data for custom backlight handling here
;_custom_backlighttbl_len::  .db  . - backlight_table


contrast_table::
	.db	0x96, 0x98, 0x9a, 0x9c, 0x9e, 0xa0, 0xa2
_custom_contrasttbl_len::  .db  . - contrast_table


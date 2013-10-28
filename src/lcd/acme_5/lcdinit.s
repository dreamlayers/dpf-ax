	.include 'dpf.inc'

	.area LCDAUX (CODE)

_lcd_custom_init::
	mov	p3dir,#0x0
	anl	p1dir,#0xe8
	anl	p2dir,#0xfd
	clr	LCD_CS
	clr	LCD_RST
	mov	a,#0x96
	lcall	X1714
	setb	LCD_RST
	mov	a,#0x96
	lcall	X1714
	mov	a,#0x11
	lcall	X172a
	mov	a,#0x78
	lcall	X1714
	mov	a,#0xf0
	lcall	X172a
	mov	a,#0x1
	lcall	X1734
	mov	a,#0xf2
	lcall	X172a
	mov	a,#0x10
	lcall	X1734
	mov	a,#0xb1
	lcall	X172a
	mov	a,#0x1
	lcall	X1734
	mov	a,#0x7
	lcall	X1734
	mov	a,#0x7
	lcall	X1734
	mov	a,#0xb4
	lcall	X172a
	mov	a,#0x6
	lcall	X1734
	mov	a,#0xc1
	lcall	X172a
	mov	a,#0x2
	lcall	X1734
	mov	a,#0xc2
	lcall	X172a
	mov	a,#0x1
	lcall	X1734
	mov	a,#0xc5
	lcall	X172a
	mov	a,#0x16
	lcall	X1734
	mov	a,#0xf
	lcall	X1734
	mov	a,#0x3a
	lcall	X172a
	mov	a,#0x5
	lcall	X1734
	mov	a,#0x36
	lcall	X172a
	mov	a,#0xd8
	lcall	X1734
	mov	a,#0xe0
	lcall	X172a
	mov	a,#0x1
	lcall	X1734
	mov	a,#0xb
	lcall	X1734
	mov	a,#0x1
	lcall	X1734
	mov	a,#0x6
	lcall	X1734
	mov	a,#0x4
	lcall	X1734
	mov	a,#0xa
	lcall	X1734
	mov	a,#0xa
	lcall	X1734
	mov	a,#0xc
	lcall	X1734
	mov	a,#0xc
	lcall	X1734
	mov	a,#0xa
	lcall	X1734
	mov	a,#0x29
	lcall	X1734
	mov	a,#0x3c
	lcall	X1734
	mov	a,#0x2
	lcall	X1734
	mov	a,#0x1
	lcall	X1734
	mov	a,#0x4
	lcall	X1734
	mov	a,#0x1f
	lcall	X1734
	mov	a,#0xe1
	lcall	X172a
	mov	a,#0x0
	lcall	X1734
	mov	a,#0xc
	lcall	X1734
	mov	a,#0x0
	lcall	X1734
	mov	a,#0x10
	lcall	X1734
	mov	a,#0x15
	lcall	X1734
	mov	a,#0x13
	lcall	X1734
	mov	a,#0x13
	lcall	X1734
	mov	a,#0x2a
	lcall	X1734
	mov	a,#0x33
	lcall	X1734
	mov	a,#0x3a
	lcall	X1734
	mov	a,#0x37
	lcall	X1734
	mov	a,#0x3c
	lcall	X1734
	mov	a,#0x2
	lcall	X1734
	mov	a,#0x0
	lcall	X1734
	mov	a,#0x4
	lcall	X1734
	mov	a,#0x1f
	lcall	X1734
	mov	a,#0x29
	lcall	X172a
	mov	a,#0x78
	lcall	X1714
	ret	
;
X1714:	mov	r5,a
X1715:	mov	r6,#0x18
X1717:	mov	r7,#0xfa
X1719:	djnz	r7,X1719
	djnz	r6,X1717
	djnz	r5,X1715
	ret	
;
X172a:	clr	LCD_A0
	ljmp	otp_lcd_write
;
X1734:	setb	LCD_A0
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
	subb	a,r3
	mov	tmr1perl,#0xff
	mov	tmr1perh,#0x40
	mov	tmr1pwml,#0xff
	mov	tmr1pwmh,a
	ret
;
_lcd_custom_setcontrast::
	mov	a,#0x25
	lcall	X172a
	mov	dptr,#contrast_table
	mov	a,r3
	dec	a
	movc	a,@a+dptr
	lcall	X1734
	ret
;
	.area LCDAUX (CODE)


;backlight_table::
;	.db	If needed, put data for custom backlight handling here
;_custom_backlighttbl_len::  .db  . - backlight_table


contrast_table::
	.db	0x44, 0x46, 0x48, 0x4a, 0x4c, 0x4e, 0x50
_custom_contrasttbl_len::  .db  . - contrast_table


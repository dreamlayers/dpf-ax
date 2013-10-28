	.include 'dpf.inc'

	.area LCDAUX (CODE)

_lcd_custom_init::
	mov	p3dir,#0x0
	anl	p1dir,#0xe8
	anl	p2dir,#0xfd
	clr	LCD_CS
	clr	LCD_RST
	mov	a,#0x96
	lcall	X1726
	setb	LCD_RST
	mov	a,#0x96
	lcall	X1726
	mov	a,#0x11
	lcall	X173c
	mov	a,#0x78
	lcall	X1726
	mov	a,#0xf0
	lcall	X173c
	mov	a,#0x1
	lcall	X1746
	mov	a,#0xf2
	lcall	X173c
	mov	a,#0x10
	lcall	X1746
	mov	a,#0xb1
	lcall	X173c
	mov	a,#0x1
	lcall	X1746
	mov	a,#0x7
	lcall	X1746
	mov	a,#0x7
	lcall	X1746
	mov	a,#0xb4
	lcall	X173c
	mov	a,#0x6
	lcall	X1746
	mov	a,#0xc1
	lcall	X173c
	mov	a,#0x6
	lcall	X1746
	mov	a,#0xc2
	lcall	X173c
	mov	a,#0x1
	lcall	X1746
	mov	a,#0xc5
	lcall	X173c
	mov	a,#0x14
	lcall	X1746
	mov	a,#0xf
	lcall	X1746
	mov	a,#0x3a
	lcall	X173c
	mov	a,#0x5
	lcall	X1746
	mov	a,#0x36
	lcall	X173c
	mov	a,#0xd8
	lcall	X1746
	mov	a,#0xe0
	lcall	X173c
	mov	a,#0x1
	lcall	X1746
	mov	a,#0xb
	lcall	X1746
	mov	a,#0x1
	lcall	X1746
	mov	a,#0x6
	lcall	X1746
	mov	a,#0x4
	lcall	X1746
	mov	a,#0xa
	lcall	X1746
	mov	a,#0xa
	lcall	X1746
	mov	a,#0xc
	lcall	X1746
	mov	a,#0xc
	lcall	X1746
	mov	a,#0xa
	lcall	X1746
	mov	a,#0x29
	lcall	X1746
	mov	a,#0x3c
	lcall	X1746
	mov	a,#0x2
	lcall	X1746
	mov	a,#0x1
	lcall	X1746
	mov	a,#0x4
	lcall	X1746
	mov	a,#0x1f
	lcall	X1746
	mov	a,#0xe1
	lcall	X173c
	mov	a,#0x0
	lcall	X1746
	mov	a,#0xc
	lcall	X1746
	mov	a,#0x0
	lcall	X1746
	mov	a,#0x10
	lcall	X1746
	mov	a,#0x15
	lcall	X1746
	mov	a,#0x13
	lcall	X1746
	mov	a,#0x13
	lcall	X1746
	mov	a,#0x2a
	lcall	X1746
	mov	a,#0x33
	lcall	X1746
	mov	a,#0x3a
	lcall	X1746
	mov	a,#0x37
	lcall	X1746
	mov	a,#0x3c
	lcall	X1746
	mov	a,#0x2
	lcall	X1746
	mov	a,#0x0
	lcall	X1746
	mov	a,#0x4
	lcall	X1746
	mov	a,#0x1f
	lcall	X1746
	mov	a,#0x29
	lcall	X173c
	mov	a,#0x78
	lcall	X1726
	ret	

;
X1726:	mov	r5,a
X1727:	mov	r6,#0x18
X1729:	mov	r7,#0xfa
X172b:	djnz	r7,X172b
	djnz	r6,X1729
	djnz	r5,X1727
	ret	
;
X173c:	clr	LCD_A0
	ljmp	otp_lcd_write
;
X1746:	setb	LCD_A0
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
	mov	b,#0xb4
	mul	ab
	subb	a,r3
	mov	tmr1perl,#0xff
	mov	tmr1perh,#0x40
	mov	tmr1pwml,a
	mov	tmr1pwmh,b
	ret
;
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


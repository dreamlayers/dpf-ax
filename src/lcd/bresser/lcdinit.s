	.include 'dpf.inc'

	.area LCDAUX (CODE)

_lcd_custom_init::
	mov	p3dir,#0x0
	anl	p1dir,#0xe8
	anl	p2dir,#0xfd
	clr	LCD_CS
	clr	LCD_RST
	mov	a,#0x96
	lcall	X15d8
	setb	LCD_RST
	mov	a,#0x96
	lcall	X15d8
	mov	a,#0x1
	lcall	X15ee
	mov	a,#0x3
	lcall	X15ee
	mov	a,#0x11
	lcall	X15ee
	mov	a,#0x13
	lcall	X15ee
	mov	a,#0x20
	lcall	X15ee
	mov	a,#0xb0
	lcall	X15ee
	mov	a,#0x25
	lcall	X15ee
	mov	a,#0x48
	lcall	X15f8
	mov	a,#0x28
	lcall	X15ee
	mov	a,#0x2a
	lcall	X15ee
	mov	a,#0x0
	lcall	X15f8
	mov	a,#0x7f
	lcall	X15f8
	mov	a,#0x2b
	lcall	X15ee
	mov	a,#0x0
	lcall	X15f8
	mov	a,#0x7f
	lcall	X15f8
	mov	a,#0x33
	lcall	X15ee
	mov	a,#0x0
	lcall	X15f8
	mov	a,#0x7f
	lcall	X15f8
	mov	a,#0x0
	lcall	X15f8
	mov	a,#0x34
	lcall	X15ee
	mov	a,#0x36
	lcall	X15ee
	mov	a,#0x0
	lcall	X15f8
	mov	a,#0x37
	lcall	X15ee
	mov	a,#0x0
	lcall	X15f8
	mov	a,#0xb4
	lcall	X15ee
	mov	a,#0x8
	lcall	X15f8
	mov	a,#0xba
	lcall	X15ee
	mov	a,#0x7
	lcall	X15f8
	mov	a,#0xd
	lcall	X15f8
	mov	a,#0x38
	lcall	X15ee
	mov	a,#0x3a
	lcall	X15ee
	mov	a,#0x5
	lcall	X15f8
	mov	a,#0x2c
	lcall	X15ee
	mov	a,#0x29
	lcall	X15ee
	ret	
;
X15d8:	mov	r5,a
X15d9:	mov	r6,#0x18
X15db:	mov	r7,#0xfa
X15dd:	djnz	r7,X15dd
	djnz	r6,X15db
	djnz	r5,X15d9
	ret	
;
X15ee:	clr	LCD_A0
	ljmp	otp_lcd_write
;
X15f8:	setb	LCD_A0
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


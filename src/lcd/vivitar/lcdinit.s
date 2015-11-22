	.include 'dpf.inc'

	.area LCDAUX (CODE)

_lcd_custom_init::
	mov	p3dir,#0x0
	anl	p1dir,#0xe8
	anl	p2dir,#0xfd
	clr	LCD_CS
	clr	LCD_RST
	mov	a,#0x96
	lcall	X1797
	setb	LCD_RST
	mov	a,#0x96
	lcall	X1797
	mov	a,#0x11
	lcall	X17ad
	mov	a,#0xc8
	lcall	X1797
	mov	a,#0xb1
	lcall	X17ad
	mov	a,#0x1
	lcall	X17b7
	mov	a,#0x8
	lcall	X17b7
	mov	a,#0x5
	lcall	X17b7
	mov	a,#0xb2
	lcall	X17ad
	mov	a,#0x2
	lcall	X17b7
	mov	a,#0x23
	lcall	X17b7
	mov	a,#0x22
	lcall	X17b7
	mov	a,#0xb3
	lcall	X17ad
	mov	a,#0x2
	lcall	X17b7
	mov	a,#0x23
	lcall	X17b7
	mov	a,#0x22
	lcall	X17b7
	mov	a,#0x2
	lcall	X17b7
	mov	a,#0x23
	lcall	X17b7
	mov	a,#0x22
	lcall	X17b7
	mov	a,#0xb4
	lcall	X17ad
	mov	a,#0x3
	lcall	X17b7
	mov	a,#0xc0
	lcall	X17ad
	mov	a,#0x82
	lcall	X17b7
	mov	a,#0x14
	lcall	X17b7
	mov	a,#0x84
	lcall	X17b7
	mov	a,#0xc2
	lcall	X17ad
	mov	a,#0xa
	lcall	X17b7
	mov	a,#0x0
	lcall	X17b7
	mov	a,#0xc3
	lcall	X17ad
	mov	a,#0x8a
	lcall	X17b7
	mov	a,#0x2e
	lcall	X17b7
	mov	a,#0xc4
	lcall	X17ad
	mov	a,#0x8a
	lcall	X17b7
	mov	a,#0xaa
	lcall	X17b7
	mov	a,#0xc5
	lcall	X17ad
	mov	a,#0xb
	lcall	X17b7
	mov	a,#0x2a
	lcall	X17ad
	mov	a,#0x0
	lcall	X17b7
	mov	a,#0x2
	lcall	X17b7
	mov	a,#0x0
	lcall	X17b7
	mov	a,#0x81
	lcall	X17b7
	mov	a,#0x2b
	lcall	X17b7
	mov	a,#0x0
	lcall	X17b7
	mov	a,#0x3
	lcall	X17b7
	mov	a,#0x0
	lcall	X17b7
	mov	a,#0x82
	lcall	X17b7
	mov	a,#0xe0
	lcall	X17ad
	mov	a,#0x3
	lcall	X17b7
	mov	a,#0x15
	lcall	X17b7
	mov	a,#0xc
	lcall	X17b7
	mov	a,#0x12
	lcall	X17b7
	mov	a,#0x3a
	lcall	X17b7
	mov	a,#0x32
	lcall	X17b7
	mov	a,#0x2b
	lcall	X17b7
	mov	a,#0x2d
	lcall	X17b7
	mov	a,#0x2b
	lcall	X17b7
	mov	a,#0x28
	lcall	X17b7
	mov	a,#0x30
	lcall	X17b7
	mov	a,#0x3c
	lcall	X17b7
	mov	a,#0x0
	lcall	X17b7
	mov	a,#0x2
	lcall	X17b7
	mov	a,#0x3
	lcall	X17b7
	mov	a,#0x10
	lcall	X17b7
	mov	a,#0xe1
	lcall	X17ad
	mov	a,#0x3
	lcall	X17b7
	mov	a,#0x15
	lcall	X17b7
	mov	a,#0xc
	lcall	X17b7
	mov	a,#0x12
	lcall	X17b7
	mov	a,#0x27
	lcall	X17b7
	mov	a,#0x25
	lcall	X17b7
	mov	a,#0x21
	lcall	X17b7
	mov	a,#0x28
	lcall	X17b7
	mov	a,#0x28
	lcall	X17b7
	mov	a,#0x27
	lcall	X17b7
	mov	a,#0x30
	lcall	X17b7
	mov	a,#0x3c
	lcall	X17b7
	mov	a,#0x0
	lcall	X17b7
	mov	a,#0x1
	lcall	X17b7
	mov	a,#0x3
	lcall	X17b7
	mov	a,#0x10
	lcall	X17b7
	mov	a,#0x36
	lcall	X17ad
	mov	a,#0xc0
	lcall	X17b7
	mov	a,#0x3a
	lcall	X17ad
	mov	a,#0x5
	lcall	X17b7
	mov	a,#0x29
	lcall	X17ad
	mov	a,#0x2c
	lcall	X17ad
	ret
;
X1797:	mov	r5,a
X1798:	mov	r6,#0x18
X179a:	mov	r7,#0xfa
X179c:	djnz	r7,X179c
	djnz	r6,X179a
	djnz	r5,X1798
	ret	
;
X17ad:	clr	LCD_A0
	ljmp	otp_lcd_write
;
X17b7:	setb	LCD_A0
	ljmp	otp_lcd_write
	
;
_lcd_custom_setbacklight::
	mov	a,r3
	jnz	doset
	ret
doset:
;
	mov	dptr,#backlight_table
	mov	a,r3
	dec	a
	movc	a,@a+dptr
	mov	tmr1perl,#0xff
	mov	tmr1perh,#0x0
	mov	tmr1pwml,a
	mov	tmr1pwmh,#0x0
	ret
;
;
; If custom contrast handling is needed, uncomment the following label
; and set LCD_CONTRAST_CUSTOM in dpfmodel.h
; r3 contains contrast value (1 .. LCD_MAX_CONTRAST_VALUE)
;
;_lcd_custom_setcontrast::

	.area LCDAUX (CODE)


backlight_table::
	.db	0xe1, 0xda, 0xd2, 0xc5, 0xbe, 0xb6, 0xaf, 0xa8, 0xa0, 0x99, 0x8f
	.db	0x89, 0x82, 0x7b, 0x5c, 0x4b, 0x32, 0x1e, 0x0a, 0x01, 0x00  
_custom_backlighttbl_len::  .db  . - backlight_table


;contrast_table::
;	.db	If needed, put data for custom contrast handling here
;_custom_contrasttbl_len::  .db  . - contrast_table


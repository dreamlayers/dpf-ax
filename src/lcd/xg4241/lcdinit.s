	.include 'dpf.inc'

	.area LCDAUX (CODE)

rd  = 0xb7

_lcd_custom_init::
;
	setb	LCD_RST
	anl	p2dir,#0xfd
	anl	p1dir,#0xfe
	clr	LCD_CS
	clr	LCD_RST
	mov	a,#0x64
	lcall	X15cd
	setb	LCD_RST
	mov	a,#0x64
	lcall	X15cd
	setb	LCD_CS
	mov	a,#0x1
	lcall	X15fd
	mov	a,#0xc8
	lcall	X15cd
	mov	a,#0x11
	lcall	X15fd
	mov	a,#0xc8
	lcall	X15cd
	mov	a,#0x29
	lcall	X15fd
	mov	a,#0xc8
	lcall	X15cd
	mov	a,#0x20
	lcall	X15fd
	mov	a,#0x38
	lcall	X15fd
	mov	a,#0x13
	lcall	X15fd
	mov	a,#0x36
	lcall	X15fd
	mov	a,#0xd0
	lcall	X15f9
	mov	a,#0x3a
	lcall	X15fd
	mov	a,#0x5
	lcall	X15f9
	mov	a,#0x25
	lcall	X15fd
	mov	a,#0x34
	lcall	X15f9
	mov	a,#0x2a
	lcall	X15fd
	mov	a,#0x0
	lcall	X15f9
	mov	a,#0x7f
	lcall	X15f9
	mov	a,#0x2b
	lcall	X15fd
	mov	a,#0x0
	lcall	X15f9
	mov	a,#0x7f
	lcall	X15f9
	mov	a,#0x2c
	lcall	X15fd
	ret
;
X154a:	lcall	X15d9
	clr	LCD_CS
	mov	LCD_A0,c
	setb	c
	lcall	X1601
X1555:	clr	LCD_RD
	rlc	a
	jz	X1564
	mov	rd,c
	lcall	X1601
	setb	LCD_RD
	clr	c
	sjmp	X1555
;
X1564:	setb	LCD_CS
	ret	
;
X15cd:	mov	r5,a
X15ce:	mov	r6,#0x18
X15d0:	mov	r7,#0xfa
X15d2:	djnz	r7,X15d2
	djnz	r6,X15d0
	djnz	r5,X15ce
	ret	
;
X15f9:	setb	c
	ljmp	X154a
;
X15fd:	clr	c
	ljmp	X154a
;
X1601:	ret	
;
X15d9:	anl	p1dir,#0xfb
	anl	p3dir,#0x7f
	anl	p1dir,#0xef
	clr	LCD_RD
	ret	
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
_lcd_custom_setcontrast::
	mov	a,#0x25
	lcall	X15fd
	mov	dptr,#contrast_table
	mov	a,r3
	dec	a
	movc	a,@a+dptr
	lcall	X15f9
	ret
;
	.area LCDAUX (CODE)


;backlight_table::
;	.db	If needed, put data for custom backlight handling here
;_custom_backlighttbl_len::  .db  . - backlight_table


contrast_table::
	.db	0x36, 0x3a, 0x3e, 0x42, 0x46, 0x4a, 0x4e
_custom_contrasttbl_len::  .db  . - contrast_table


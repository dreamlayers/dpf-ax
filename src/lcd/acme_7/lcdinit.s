	.include 'dpf.inc'

	.area LCDAUX (CODE)

_lcd_custom_init::
	mov	p3dir,#0x0
	anl	p1dir,#0xe8
	anl	p2dir,#0xfd
	setb	LCD_CS
	clr	LCD_RST
	mov	a,#0x96
	lcall	X15e7
	setb	LCD_RST
	mov	a,#0x96
	lcall	X15e7
	clr	LCD_CS
	mov	a,#0xee
	lcall	X15fd
	mov	a,#0xb0
	lcall	X15fd
	mov	a,#0x3
	lcall	X1607
	mov	a,#0x6
	lcall	X1607
	mov	a,#0xc7
	lcall	X15fd
	mov	a,#0x4
	lcall	X1607
	mov	a,#0xc5
	lcall	X15fd
	mov	a,#0x17
	lcall	X1607
	mov	a,#0x17
	lcall	X1607
	mov	a,#0x17
	lcall	X1607
	mov	a,#0x17
	lcall	X1607
	mov	a,#0xc3
	lcall	X15fd
	mov	a,#0x35
	lcall	X1607
	mov	a,#0x25
	lcall	X15fd
	mov	a,#0x26
	lcall	X1607
	mov	a,#0xc0
	lcall	X15fd
	mov	a,#0x60
	lcall	X1607
	mov	a,#0x0
	lcall	X1607
	mov	a,#0xc4
	lcall	X15fd
	mov	a,#0x84
	lcall	X1607
	mov	a,#0xb9
	lcall	X15fd
	mov	a,#0x21
	lcall	X15fd
	mov	a,#0x36
	lcall	X15fd
	mov	a,#0x0
	lcall	X1607
	mov	a,#0x3a
	lcall	X15fd
	mov	a,#0x5
	lcall	X1607
	mov	a,#0xc9
	lcall	X15fd
	mov	a,#0xd
	lcall	X1607
	mov	a,#0xc2
	lcall	X15fd
	mov	a,#0x0
	lcall	X1607
	mov	a,#0xb4
	lcall	X15fd
	mov	a,#0xb6
	lcall	X15fd
	mov	a,#0x11
	lcall	X15fd
	ret	
;
X15e7:	mov	r5,a
X15e8:	mov	r6,#0x18
X15ea:	mov	r7,#0xfa
X15ec:	djnz	r7,X15ec
	djnz	r6,X15ea
	djnz	r5,X15e8
	ret
;
X15fd:	clr	LCD_A0
	ljmp	otp_lcd_write
;
X1607:	setb	LCD_A0
	ljmp	otp_lcd_write
;

_lcd_custom_setbacklight::
	mov	a,r3
	jnz	doset
	ret
doset:	clr	c
	mov	a,#0x40
	subb	a,r3
	mov	tmr1perl,#0x40
	mov	tmr1perh,#0x0
	mov	tmr1pwml,a
	mov	tmr1pwmh,#0x0
	ret

_lcd_custom_setcontrast::
	mov	a,#0x25
	lcall	X15fd
	mov	dptr,#contrast_table
	mov	a,r3
	dec	a
	movc	a,@a+dptr
	lcall	X1607
	ret
;
	.area LCDAUX (CODE)


;backlight_table::
;	.db	If needed, put data for custom backlight handling here
;_custom_backlighttbl_len::  .db  . - backlight_table


contrast_table::
	.db	0x20, 0x24, 0x26, 0x28, 0x2a, 0x2c, 0x2f
_custom_contrasttbl_len::  .db  . - contrast_table


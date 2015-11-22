	.include 'dpf.inc'

	.area LCDAUX (CODE)

_lcd_custom_init::
	mov	p3dir,#0x0
	anl	p1dir,#0xe8
	anl	p2dir,#0xfd
	setb	LCD_CS
	clr	LCD_RST
	mov	a,#0x96
	lcall	X167d
	setb	LCD_RST
	mov	a,#0x96
	lcall	X167d
	clr	LCD_CS
	mov	a,#0x40
	lcall	X1693
	mov	a,#0x80
	lcall	X1693
	mov	a,#0x3c
	lcall	X167d
	mov	a,#0x18
	lcall	X1693
	mov	a,#0x0
	lcall	X1693
	mov	a,#0x3c
	lcall	X167d
	mov	a,#0x2c
	lcall	X1693
	mov	a,#0x3c
	lcall	X167d
	mov	a,#0x2
	lcall	X1693
	mov	a,#0x0
	lcall	X1693
	mov	a,#0x3c
	lcall	X167d
	mov	a,#0x2a
	lcall	X1693
	mov	a,#0x40
	lcall	X1693
	mov	a,#0x3c
	lcall	X167d
	mov	a,#0x10
	lcall	X1693
	mov	a,#0xa
	lcall	X1693
	mov	a,#0x3c
	lcall	X167d
	mov	a,#0x28
	lcall	X1693
	mov	a,#0x0
	lcall	X1693
	mov	a,#0x3c
	lcall	X167d
	mov	a,#0x30
	lcall	X1693
	mov	a,#0xb
	lcall	X1693
	mov	a,#0x3c
	lcall	X167d
	mov	a,#0x32
	lcall	X1693
	mov	a,#0x0
	lcall	X1693
	mov	a,#0x3c
	lcall	X167d
	mov	a,#0x34
	lcall	X1693
	mov	a,#0x91
	lcall	X1693
	mov	a,#0x3c
	lcall	X167d
	mov	a,#0x36
	lcall	X1693
	mov	a,#0x0
	lcall	X1693
	mov	a,#0x3c
	lcall	X167d
	mov	a,#0x45
	lcall	X1693
	mov	a,#0x0
	lcall	X1693
	mov	a,#0x3c
	lcall	X167d
	mov	a,#0x53
	lcall	X1693
	mov	a,#0x0
	lcall	X1693
	mov	a,#0x3c
	lcall	X167d
	mov	a,#0x0
	lcall	X1693
	mov	a,#0x0
	lcall	X1693
	mov	a,#0x3c
	lcall	X167d
	mov	a,#0x55
	lcall	X1693
	mov	a,#0x0
	lcall	X1693
	mov	a,#0x56
	lcall	X1693
	mov	a,#0x0
	lcall	X1693
	mov	a,#0x3c
	lcall	X167d
	mov	a,#0x57
	lcall	X1693
	mov	a,#0x7f
	lcall	X1693
	mov	a,#0x3c
	lcall	X167d
	mov	a,#0x70
	lcall	X1693
	mov	a,#0x1c
	lcall	X1693
	mov	a,#0x3c
	lcall	X167d
	mov	a,#0x7f
	lcall	X1693
	mov	a,#0x50
	lcall	X1693
	mov	a,#0x3c
	lcall	X167d
	mov	a,#0x42
	lcall	X1693
	mov	a,#0x0
	lcall	X1693
	mov	a,#0x7f
	lcall	X1693
	mov	a,#0x43
	lcall	X1693
	mov	a,#0x0
	lcall	X1693
	mov	a,#0x7f
	lcall	X1693
	mov	a,#0x51
	lcall	X1693
	mov	a,#0x3c
	lcall	X167d
	ret
;
X167d:	mov	r5,a
X167e:	mov	r6,#0x18
X1680:	mov	r7,#0xfa
X1682:	djnz	r7,X1682
	djnz	r6,X1680
	djnz	r5,X167e
	ret	
;
X1693:	clr	LCD_A0
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
	mov	tmr1perl,#0xff
	mov	tmr1perh,#0x0
	mov	tmr1pwml,a
	mov	tmr1pwmh,#0x0
	ret

_lcd_custom_setcontrast::
	mov	a,#0x2a
	lcall	X1693
	mov	dptr,#contrast_table
	mov	a,r3
	dec	a
	movc	a,@a+dptr
	lcall	X1693
	ret

	.area LCDAUX (CODE)


;backlight_table::
;	.db	If needed, put data for custom backlight handling here
;_custom_backlighttbl_len::  .db  . - backlight_table


contrast_table::
	.db	0x2e, 0x32, 0x36, 0x3e, 0x42, 0x46, 0x4a
_custom_contrasttbl_len::  .db  . - contrast_table


	.include 'dpf.inc'

	.area LCDAUX (CODE)

_lcd_custom_init::
	mov	p3dir,#0x0
	anl	p1dir,#0xe8
	anl	p2dir,#0xfd
	clr	LCD_CS
	clr	LCD_RST
	mov	a,#0xc8
	lcall	X1779
	setb	LCD_RST
	mov	a,#0xc8
	lcall	X1779
	mov	a,#0x2c
	lcall	X178f
	mov	a,#0x14
	lcall	X1779
	mov	a,#0xeb
	lcall	X178f
	mov	a,#0x2
	lcall	X178f
	mov	a,#0x1
	lcall	X178f
	mov	a,#0xa
	lcall	X1779
	mov	a,#0x20
	lcall	X178f
	mov	a,#0x1
	lcall	X178f
	mov	a,#0x26
	lcall	X178f
	mov	a,#0x1
	lcall	X178f
	mov	a,#0xa
	lcall	X1779
	mov	a,#0x26
	lcall	X178f
	mov	a,#0x9
	lcall	X178f
	mov	a,#0xa
	lcall	X1779
	mov	a,#0x26
	lcall	X178f
	mov	a,#0xb
	lcall	X178f
	mov	a,#0xa
	lcall	X1779
	mov	a,#0x26
	lcall	X178f
	mov	a,#0xf
	lcall	X178f
	mov	a,#0xa
	lcall	X1779
	mov	a,#0x28
	lcall	X178f
	mov	a,#0x3
	lcall	X178f
	mov	a,#0x45
	lcall	X178f
	mov	a,#0x0
	lcall	X178f
	mov	a,#0x53
	lcall	X178f
	mov	a,#0x0
	lcall	X178f
	mov	a,#0x10
	lcall	X178f
	mov	a,#0x3
	lcall	X178f
	mov	a,#0x24
	lcall	X178f
	mov	a,#0xf
	lcall	X178f
	mov	a,#0x30
	lcall	X178f
	mov	a,#0x13
	lcall	X178f
	mov	a,#0x32
	lcall	X1779
	mov	a,#0x32
	lcall	X178f
	mov	a,#0xe
	lcall	X178f
	mov	a,#0xa
	lcall	X1779
	mov	a,#0x40
	lcall	X178f
	mov	a,#0x80
	lcall	X178f
	mov	a,#0x42
	lcall	X178f
	mov	a,#0x2
	lcall	X178f
	mov	a,#0x83
	lcall	X178f
	mov	a,#0x43
	lcall	X178f
	mov	a,#0x2
	lcall	X178f
	mov	a,#0x7f
	lcall	X178f
	mov	a,#0x34
	lcall	X178f
	mov	a,#0x1f
	lcall	X178f
	mov	a,#0x2a
	lcall	X178f
	mov	a,#0x4f
	lcall	X178f
	mov	a,#0x53
	lcall	X178f
	mov	a,#0x0
	lcall	X178f
	mov	a,#0x55
	lcall	X178f
	mov	a,#0x0
	lcall	X178f
	mov	a,#0x64
	lcall	X1779
	mov	a,#0x56
	lcall	X178f
	mov	a,#0x0
	lcall	X178f
	mov	a,#0x57
	lcall	X178f
	mov	a,#0x7f
	lcall	X178f
	mov	a,#0x36
	lcall	X178f
	mov	a,#0x0
	lcall	X178f
	mov	a,#0x51
	lcall	X178f
	ret

X1779:	mov	r5,a
X177a:	mov	r6,#0x18
X177c:	mov	r7,#0xfa
X177e:	djnz	r7,X177e
	djnz	r6,X177c
	djnz	r5,X177a
	ret	
;
X178f:	clr	LCD_A0
	ljmp	otp_lcd_write


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

_lcd_custom_setcontrast::
	mov	a,#0x2a
	lcall	X178f
	mov	dptr,#contrast_table
	mov	a,r3
	dec	a
	movc	a,@a+dptr
	lcall	X178f
	ret
;
	.area LCDAUX (CODE)


;backlight_table::
;	.db	If needed, put data for custom backlight handling here
;_custom_backlighttbl_len::  .db  . - backlight_table


contrast_table::
	.db	0x44, 0x46, 0x48, 0x4a, 0x4c, 0x4e, 0x50, 0x52, 0x54, 0x56, 0x58 
_custom_contrasttbl_len::  .db  . - contrast_table


	.include 'dpf.inc'

	.area LCDAUX (CODE)

_lcd_custom_init::
;
	mov	p3dir,#0x0
	anl	p1dir,#0xe8
	anl	p2dir,#0xfd
	clr	LCD_CS
	clr	LCD_RST
	mov	a,#0x96
	lcall	X1792
	setb	LCD_RST
	mov	a,#0x96
	lcall	X1792
	mov	a,#0x1
	lcall	X17a8
	mov	a,#0x96
	lcall	X1792
	mov	a,#0xd7
	lcall	X17a8
	mov	a,#0xbf
	lcall	X17b2
	mov	a,#0xe0
	lcall	X17a8
	mov	a,#0x0
	lcall	X17b2
	mov	a,#0xa
	lcall	X1792
	mov	a,#0xe3
	lcall	X17a8
	mov	a,#0x14
	lcall	X1792
	mov	a,#0xe1
	lcall	X17a8
	mov	a,#0x28
	lcall	X17a8
	mov	a,#0x11
	lcall	X17a8
	mov	a,#0xc0
	lcall	X17a8
	mov	a,#0x17
	lcall	X17b2
	mov	a,#0x1
	lcall	X17b2
	mov	a,#0xc3
	lcall	X17a8
	mov	a,#0x0
	lcall	X17b2
	mov	a,#0xc4
	lcall	X17a8
	mov	a,#0x6
	lcall	X17b2
	mov	a,#0xc5
	lcall	X17a8
	mov	a,#0x1
	lcall	X17b2
	mov	a,#0xcb
	lcall	X17a8
	mov	a,#0x1
	lcall	X17b2
	mov	a,#0xb7
	lcall	X17a8
	mov	a,#0xc8
	lcall	X17b2
	mov	a,#0x25
	lcall	X17a8
	mov	a,#0x20
	lcall	X17b2
	mov	a,#0xd0
	lcall	X17a8
	mov	a,#0x1d
	lcall	X17b2
	mov	a,#0xb5
	lcall	X17a8
	mov	a,#0x1c
	lcall	X17b2
	mov	a,#0xf0
	lcall	X17a8
	mov	a,#0x6
	lcall	X17b2
	mov	a,#0xb
	lcall	X17b2
	mov	a,#0xd
	lcall	X17b2
	mov	a,#0x12
	lcall	X17b2
	mov	a,#0xb4
	lcall	X17a8
	mov	a,#0x18
	lcall	X17b2
	mov	a,#0xd7
	lcall	X17a8
	mov	a,#0xbf
	lcall	X17b2
	mov	a,#0xb5
	lcall	X17a8
	mov	a,#0x1c
	lcall	X17b2
	mov	a,#0xcd
	lcall	X17a8
	mov	a,#0x80
	lcall	X17b2
	mov	a,#0xd0
	lcall	X17a8
	mov	a,#0x1d
	lcall	X17b2
	mov	a,#0xb4
	lcall	X17a8
	mov	a,#0x18
	lcall	X17b2
	mov	a,#0x20
	lcall	X17a8
	mov	a,#0x2a
	lcall	X17a8
	mov	a,#0x4
	lcall	X17b2
	mov	a,#0x83
	lcall	X17b2
	mov	a,#0x2b
	lcall	X17a8
	mov	a,#0x0
	lcall	X17b2
	mov	a,#0x83
	lcall	X17b2
	mov	a,#0x3a
	lcall	X17a8
	mov	a,#0x5
	lcall	X17b2
	mov	a,#0x36
	lcall	X17a8
	mov	a,#0x8
	lcall	X17b2
	mov	a,#0xb0
	lcall	X17a8
	mov	a,#0x83
	lcall	X17b2
	mov	a,#0xf9
	lcall	X17a8
	mov	a,#0x0
	lcall	X17b2
	mov	a,#0x3
	lcall	X17b2
	mov	a,#0x5
	lcall	X17b2
	mov	a,#0x7
	lcall	X17b2
	mov	a,#0x9
	lcall	X17b2
	mov	a,#0xb
	lcall	X17b2
	mov	a,#0xd
	lcall	X17b2
	mov	a,#0xf
	lcall	X17b2
	mov	a,#0x11
	lcall	X17b2
	mov	a,#0x13
	lcall	X17b2
	mov	a,#0x15
	lcall	X17b2
	mov	a,#0x17
	lcall	X17b2
	mov	a,#0x19
	lcall	X17b2
	mov	a,#0x1b
	lcall	X17b2
	mov	a,#0x1d
	lcall	X17b2
	mov	a,#0x1f
	lcall	X17b2
	mov	a,#0x29
	lcall	X17a8
	ret	
;
X1792:	mov	r5,a
X1793:	mov	r6,#0x18
X1795:	mov	r7,#0xfa
X1797:	djnz	r7,X1797
	djnz	r6,X1795
	djnz	r5,X1793
	ret	
;
X17a8:	clr	LCD_A0
	ljmp	otp_lcd_write
;
X17b2:	setb	LCD_A0
	ljmp	otp_lcd_write
;
_lcd_custom_setbacklight::
;
	mov	dptr,#backlight_table
	mov	a,r3
;	dec	a		; value 0 is okay
	movc	a,@a+dptr
	mov	tmr1perl,#0xff
	mov	tmr1perh,#0x0
	mov	tmr1pwml,a
	mov	tmr1pwmh,#0x0
	ret
;

_lcd_custom_setcontrast::
;
	mov	a,#0x25
	lcall	X17a8
	mov	dptr,#contrast_table
	mov	a,r3
	dec	a
	movc	a,@a+dptr
	lcall	X17b2
	ret
;

	.area LCDAUX (CODE)

backlight_table::
	.db	0xff, 0xfe, 0xf9, 0xf2, 0xeb, 0xe4, 0xd8, 0xcc, 0xc0, 0xb4
	.db	0xa8, 0x9c, 0x90, 0x84, 0x78, 0x6c, 0x60, 0x54, 0x48, 0x3c
	.db	0x30, 0x24 ;, 0x22  <-- one to much for standard LCD_MAX_BRIGHTNESS_VALUE
_custom_backlighttbl_len::  .db  . - backlight_table


contrast_table::
	.db	0x26, 0x30, 0x32, 0x34, 0x36, 0x38, 0x3a, 0x3c, 0x3e, 0x40
_custom_contrasttbl_len::  .db  . - contrast_table


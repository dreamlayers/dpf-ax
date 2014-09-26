	.include 'dpf.inc'

	.area LCDAUX (CODE)

_lcd_custom_init::
mov	p3dir,#0x0
	anl	p1dir,#0xe8
	anl	p2dir,#0xfd
	clr	LCD_CS
	mov	a,#0x96
	lcall	X1670
	clr	LCD_RST
	mov	a,#0x96
	lcall	X1670
	setb	LCD_RST
	mov	a,#0x96
	lcall	X1670
	mov	a,#0x11
	lcall	X1686
	mov	a,#0x64
	lcall	X1670
	mov	a,#0xd7
	lcall	X1686
	mov	a,#0x18
	lcall	X1690
	mov	a,#0xb7
	lcall	X1686
	mov	a,#0xa
	lcall	X1690
	mov	a,#0xc0
	lcall	X1686
	mov	a,#0x30
	lcall	X1690
	mov	a,#0x1
	lcall	X1690
	mov	a,#0xc3
	lcall	X1686
	mov	a,#0x4
	lcall	X1690
	mov	a,#0xb0
	lcall	X1686
	mov	a,#0x82
	lcall	X1690
	mov	a,#0xb5
	lcall	X1686
	mov	a,#0x0
	lcall	X1690
	mov	a,#0x29
	lcall	X1686
	mov	a,#0x2a
	lcall	X1686
	mov	a,#0x0
	lcall	X1690
	mov	a,#0x7f
	lcall	X1690
	mov	a,#0x2b
	lcall	X1686
	mov	a,#0x4
	lcall	X1690
	mov	a,#0x83
	lcall	X1690
	mov	a,#0x2c
	lcall	X1686
	ret	
;
X1670:	mov	r5,a
X1671:	mov	r6,#0x18
X1673:	mov	r7,#0xfa
X1675:	djnz	r7,X1675
	djnz	r6,X1673
	djnz	r5,X1671
	ret	
;
	mov	tmr1con,#0x43
	mov	tmr1cntl,#0x0
	mov	tmr1cnth,#0x0
	ret	
;
X1686:	clr	LCD_A0
	ljmp	otp_lcd_write
;
X1690:	setb	LCD_A0
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
	mov	tmr1perl,#0xff
	mov	tmr1perh,#0x40
	mov	tmr1pwml,#0xff
	mov	tmr1pwmh,a
	ret

_lcd_custom_setcontrast::
	mov	a,#0x25
	lcall	X1686
	mov	dptr,#contrast_table
	mov	a,r3
	dec	a
	movc	a,@a+dptr
	lcall	X1690
	ret
;
	.area LCDAUX (CODE)


;backlight_table::
;	.db	If needed, put data for custom backlight handling here
;_custom_backlighttbl_len::  .db  . - backlight_table


contrast_table::
	.db	0x44, 0x46, 0x48, 0x4a, 0x4c, 0x4e, 0x50, 0x52, 0x54, 0x56
_custom_contrasttbl_len::  .db  . - contrast_table


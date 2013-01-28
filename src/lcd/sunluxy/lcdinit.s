	.include 'dpf.inc'

	.area LCDAUX (CODE)

_lcd_custom_init::
	mov	p3dir,#0x0
	anl	p1dir,#0xe8
	anl	p2dir,#0xfd
	clr	LCD_CS
	clr	LCD_RST
	mov	a,#0x96
	lcall	X159c
	setb	LCD_RST
	mov	a,#0xc8
	lcall	X159c
	mov	a,#0xe2
	lcall	X15b2
	mov	a,#0xc8
	lcall	X159c
	mov	a,#0xc8
	lcall	X159c
	mov	a,#0xc8
	lcall	X159c
	mov	a,#0xea
	lcall	X15b2
	mov	a,#0x81
	lcall	X15b2
	mov	a,#0x84
	lcall	X15b2
	mov	a,#0x2f
	lcall	X15b2
	mov	a,#0xd1
	lcall	X15b2
	mov	a,#0xd6
	lcall	X15b2
	mov	a,#0xc0
	lcall	X15b2
	mov	a,#0x89
	lcall	X15b2
	mov	a,#0xa2
	lcall	X15b2
	mov	a,#0xc8
	lcall	X15b2
	mov	a,#0x18
	lcall	X15b2
	mov	a,#0xde
	lcall	X15b2
	mov	a,#0xaf
	lcall	X15b2
	ret	
;
X159c:	mov	r5,a
X159d:	mov	r6,#0x18
X159f:	mov	r7,#0xfa
X15a1:	djnz	r7,X15a1
	djnz	r6,X159f
	djnz	r5,X159d
	ret	
;
X15b2:	clr	LCD_A0
	ljmp	otp_lcd_write
;
_lcd_custom_setcontrast::
	mov	a,#0x81
    clr	LCD_A0
	lcall	otp_lcd_write
	mov	a,dpl
	dec	a
	mov	dptr,#contrast_table
	movc	a,@a+dptr
    setb    LCD_A0
	ljmp	otp_lcd_write

	.area LCDAUX (CODE)
contrast_table:
	.db	0x60, 0x65, 0x6a, 0x6f, 0x74, 0x79, 0x7b,0x7f, 0x81, 0x83
;
_custom_contrasttbl_len::  .db  . - contrast_table

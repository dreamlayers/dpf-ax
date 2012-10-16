	.include 'dpf.inc'

	.area LCDAUX (CODE)

_lcd_custom_init::
	mov	p3dir,#0x0
	anl	p1dir,#0xe8
	anl	p2dir,#0xfd
	clr	LCD_CS
	clr	LCD_RST
	mov	a,#0x96
	lcall	X1639
	setb	LCD_RST
	mov	a,#0xc8
	lcall	X1639
	mov	a,#0xe2
	lcall	X164f
	mov	a,#0xc8
	lcall	X1639
	mov	a,#0xc8
	lcall	X1639
	mov	a,#0xc8
	lcall	X1639
	mov	a,#0xea
	lcall	X164f
	mov	a,#0x81
	lcall	X164f
	mov	a,#0x84
	lcall	X164f
	mov	a,#0x2f
	lcall	X164f
	mov	a,#0xd1
	lcall	X164f
	mov	a,#0xd6
	lcall	X164f
	mov	a,#0xc0
	lcall	X164f
	mov	a,#0x89
	mov	a,#0xa2
	lcall	X164f
	mov	a,#0xc8
	lcall	X164f
	mov	a,#0x18
	lcall	X164f
	mov	a,#0xde
	lcall	X164f
	mov	a,#0xaf
	lcall	X164f
	ret	
;
X1639:	mov	r5,a
X163a:	mov	r6,#0x18
X163c:	mov	r7,#0xfa
X163e:	djnz	r7,X163e
	djnz	r6,X163c
	djnz	r5,X163a
	ret	
;
X164f:	clr	LCD_A0
	ljmp	otp_lcd_write
;
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
	.db	0x83, 0x86, 0x89, 0x8c, 0x8f, 0x92, 0x95
;
_custom_contrasttbl_len::  .db  . - contrast_table

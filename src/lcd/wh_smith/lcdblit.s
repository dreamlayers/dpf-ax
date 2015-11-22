	.include 'dpf.inc'

	.area BLIT (CODE)
;
; Call to ROM lcd write routine
; Replace, if custom lcd write is required
;
rd  = 0xb7
;
lcd_write::
	ljmp	X12d0
;	ljmp	otp_lcd_write


ar0 = 0x00

_custom_blit::
	mov	a,#0x2a
	lcall	X12cc
	mov	a,_g_blit+x0
	lcall	X12d0
	mov	a,_g_blit+x1
	lcall	X12d0
	mov	a,#0x2b
	lcall	X12cc
	mov	a,_g_blit+y0
	lcall	X12d0
	mov	a,_g_blit+y1
	lcall	X12d0
	mov	a,#0x2c
	ljmp	X12cc
;
X12a3:	lcall	X12c0
	clr	LCD_CS
	mov	LCD_A0,c
	setb	c
	lcall	X12cb
X12ae:	clr	LCD_RD
	rlc	a
	jz	X12bd
	mov	rd,c
	lcall	X12cb
	setb	LCD_RD
	clr	c
	sjmp	X12ae
;
X12bd:	setb	LCD_CS
	ret	
;
X12c0:	anl	p1dir,#0xfb
	anl	p3dir,#0x7f
	anl	p1dir,#0xef
	clr	LCD_RD
X12cb:	ret	
;
X12cc:	clr	c
	ljmp	X12a3
;
X12d0:	setb	c
	ljmp	X12a3

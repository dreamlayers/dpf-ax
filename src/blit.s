	.include 'dpf.inc'
	.module blit
	.area HOME (CODE)
	.area DSEG (DATA)
_g_chartbl_offs::
	.ds 3
	.area PRINTF (CODE)


_blit_char4x8::
	; prepare SPI access:
	clr	ea
	clr	SPI_CS

	clr	a
	mov	r0, a
	; Shift bits:
	;                 a         c                r0
	mov	a, dpl	; get index c
	rrc	a	; 0ABCDEFG #H
	xch	a, r0	; 00000000                   0ABCDEFG
	rrc	a	; H0000000 #0

	xch	a, r0	; 0ABCDEFG                   H0000000
	rrc	a	; 00ABCDEF #G
	xch	a, r0	; H0000000                   00ABCDEF
	rrc	a	; GH000000 #0

	; r0:a now contains (64 * c)
	add	a, _g_chartbl_offs
	xch	a, r0
	addc	a, _g_chartbl_offs+1
	mov	r1, a
	clr	a
	addc	a, _g_chartbl_offs+2

	mov	spibuf, #SPM_READ
w1:	jnb	_SPIPND, w1
	mov	spibuf, a ; Address H
w2:	jnb	_SPIPND, w2
	mov	spibuf, r1 ; Address M
w3:	jnb	_SPIPND, w3
	mov	spibuf, r0 ; Address L
w4:	jnb	_SPIPND, w4
	xch	a, spibuf   ; one dummy

	mov	r7, #32	; char size
repeat:
w5:	jnb	_SPIPND, w5
	xch	a, spibuf
	lcall	otp_lcd_write
w6:	jnb	_SPIPND, w6
	xch	a, spibuf
	lcall	otp_lcd_write

	djnz	r7, repeat
	setb	SPI_CS
	mov	a, spibuf

	setb	ea
	ret

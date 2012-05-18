	.include 'dpf.inc'
	.module blit
	.area HOME (CODE)
	.area DSEG (DATA)
_g_chartbl_offs::
	.ds 3

	.area ISEG (DATA)
_g_fontcolor::
        .ds 2
_g_fontbgcolor::
        .ds 2

    .area BSEG   (BIT)
_g_largefont::
	.ds	1

	.area PRINTF (CODE)


_blit_char::
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

        jb      _g_largefont, _blit_char9x16

_blit_char4x8::
	add	a, _g_chartbl_offs
	xch	a, r0
	addc	a, _g_chartbl_offs+1
	mov	r1, a
	clr	a
	addc	a, _g_chartbl_offs+2

	mov	spibuf, #SPM_READ
	jnb	_SPIPND, .
	mov	spibuf, a ; Address H
	jnb	_SPIPND, .
	mov	spibuf, r1 ; Address M
	jnb	_SPIPND, .
	mov	spibuf, r0 ; Address L
	jnb	_SPIPND, .
	xch	a, spibuf   ; one dummy

	mov	r7, #32	; char size
repeat:
	jnb	_SPIPND, .
	xch	a, spibuf
	lcall	otp_lcd_write
	jnb	_SPIPND, .
	xch	a, spibuf
	lcall	otp_lcd_write

	djnz	r7, repeat

return:
	setb	SPI_CS
	mov	a, spibuf

	setb	ea
	ret

_blit_char9x16::
        xch     a, r0
	rrc	a
	xch	a, r0
	rrc	a

	; r0:a now contains (32 * c)
    
	add	a, #(chartbl9x16_offs - 0x20 * 32)
	xch	a, r0
	addc	a, #((chartbl9x16_offs - 0x20 * 32) >> 8)
	mov	r1, a
	clr	a
	addc	a, #((chartbl9x16_offs - 0x20 * 32) >> 16)

	mov	spibuf, #SPM_READ
	jnb	_SPIPND, .
	mov	spibuf, a ; Address H
	jnb	_SPIPND, .
	mov	spibuf, r1 ; Address M
	jnb	_SPIPND, .
	mov	spibuf, r0 ; Address L
	jnb	_SPIPND, .
	xch	a, spibuf   ; one dummy

	mov	r7, #16	; rows / char
repeat16:
        mov     r6, #8
	jnb	_SPIPND, .
	xch	a, spibuf
        mov     r1, a           ;r1: bit 0-7 char
	jnb	_SPIPND, .
	xch	a, spibuf
        mov     r2, a           ;r2: bit 8 of char
repeat8:
        mov     a, r1
        rlc     a
        mov     r1, a
        acall   showpixel
        djnz    r6, repeat8
        mov     a, r2
        rlc     a
        acall   showpixel
	djnz	r7, repeat16
        sjmp    return

showpixel:
        mov     r0, #_g_fgcol + 1
        jc      b1
        mov     r0, #_g_bgcol + 1
b1:     mov	a, @r0
        lcall	otp_lcd_write
        dec     r0
        mov	a, @r0
        lcall	otp_lcd_write
        ret

;__blit_splash::
;	clr	ea
;	clr	SPI_CS
;	mov	spibuf, #SPM_READ
;bsw1:	jnb	_SPIPND, bsw1
;	mov	spibuf, #(splash_offs >> 16) ; Address H
;bsw2:	jnb	_SPIPND, bsw2
;	mov	spibuf, #(splash_offs >> 8)  ; Address M
;bsw3:	jnb	_SPIPND, bsw3
;	mov	spibuf, #splash_offs	     ; Address L
;bsw4:	jnb	_SPIPND, bsw4
;	xch	a, spibuf   ; one dummy
;
;bsw5:	jnb	_SPIPND, bsw5
;	xch	a, spibuf
;	mov	r2, a		; length / 4 H
;bsw6:	jnb	_SPIPND, bsw6
;	xch	a, spibuf
;	mov	r7, a		; length / 4 L
;bsl:	jnb	_SPIPND, bsl
;	xch	a, spibuf
;	lcall	otp_lcd_write
;bsw7:	jnb	_SPIPND, bsw7
;	xch	a, spibuf
;	lcall	otp_lcd_write
;bsw8:	jnb	_SPIPND, bsw8
;	xch	a, spibuf
;	lcall	otp_lcd_write
;bsw9:	jnb	_SPIPND, bsw9
;	xch	a, spibuf
;	lcall	otp_lcd_write
;	djnz	r7, bsl
;	djnz	r2, bsl
;
;	setb	SPI_CS
;	mov	a, spibuf
;	setb	ea
;	ret

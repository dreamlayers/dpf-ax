	.include 'dpf.inc'
	.module initconf

	.area INIT (CODE)

__load_config_from_flash::
	clr	ea
	clr	SPI_CS
	mov	spibuf, #SPM_READ
bsw1:	jnb	_SPIPND, bsw1
	mov	a, b
	mov	spibuf, a	; Address H
bsw2:	jnb	_SPIPND, bsw2
	mov	a, dph
	mov	spibuf, a	; Address M
bsw3:	jnb	_SPIPND, bsw3
	mov	a, dpl
	mov	spibuf, a	; Address L
bsw4:	jnb	_SPIPND, bsw4
	xch	a, spibuf   ; one dummy
;
bsw5:	jnb	_SPIPND, bsw5
	xch	a, spibuf
	mov	dpl, #0		; ret = not OK
	cjne	a, #0x47, _ret	; the magic word!
bsw6:	jnb	_SPIPND, bsw6
	xch	a, spibuf
	cjne	a, #0x11, _ret
bsw7:	jnb	_SPIPND, bsw7
	xch	a, spibuf
	mov	r7,a		; length
	mov	r0,#_g_config		
bsl:	jnb	_SPIPND, bsl
	xch	a, spibuf
	mov	@r0, a
	inc	r0
	djnz	r7, bsl
;
	mov	dpl, #0x01	;ret = OK
_ret:	setb	SPI_CS
	mov	a, spibuf
	setb	ea
	ret

	.include 'dpf.inc'
	.module blit
	.area HOME (CODE)
	.area DSEG (DATA)
__blit_splash_PARM_2::		;share space with local params
resx2:	.ds	1
resy2:	.ds	1
	.area ISEG (DATA)
	.area BSEG   (BIT)

	.area PRINTF (CODE)

__blit_splash::
	clr	ea
	clr	SPI_CS

	mov	a,__blit_splash_PARM_2 + 1
	rrc	a			; only lsb used, width must be even and < 512!
	mov	a,__blit_splash_PARM_2
	rrc	a			; height / 2
	mov	resy2, a
	mov	a,dph
	rrc	a			; only lsb used, height must be even and < 512!
	mov	a,dpl
	rrc	a			; width / 2
	mov	resx2, a
;
	mov	spibuf, #SPM_READ
	jnb	_SPIPND, .
	mov	spibuf, #(splash_offs >> 16) ; Address H
	jnb	_SPIPND, .
	mov	spibuf, #(splash_offs >> 8)  ; Address M
	jnb	_SPIPND, .
	mov	spibuf, #splash_offs	     ; Address L
	jnb	_SPIPND, .
	xch	a, spibuf   ; one dummy
;
	jnb	_SPIPND, .
	xch	a, spibuf
	mov	r1, a		; width L
	jnb	_SPIPND, .
	xch	a, spibuf	; width H
	rrc	a		; only lsb used, image width must be even and < 512!
	jz	wok
	mov	a, resx2	; width >= 512 ??
	ajmp	wnok
wok:	mov	a,r1
	rrc	a		; image width / 2
wnok:	mov	r5, a
;
	jnb	_SPIPND, .
	xch	a, spibuf
	mov	r1, a		; height L
	jnb	_SPIPND, .
	xch	a, spibuf
	rrc	a		; only lsb used, image height must be even and < 512!
	jz	hok
	mov	a, resy2	; height >= 512 ??
	ajmp	hnok
hok:	mov	a,r1
	rrc	a		; image height / 2
hnok:
;
; compute # lines
;
	mov	r3, a		; # of image lines / 2
	mov	a, resy2
	clr	c
	subb	a, r3
	mov	r4, a		; # of lines to fill after img / 2
	jnc	w1
	mov	r4, #0
	mov	r3, resy2	; # of image lines to draw / 2
	;
; compute # cols
;
w1:	mov	r7, #0
	mov	a, resx2
	clr	c
	subb	a, r5		; # of image cols / 2
	mov	r6, a		; # of cols to fill right of image / 2
	jnc	lloop
	mov	r7, a
	clr	c
	clr	a
	subb	a, r7
	mov	r7, a		; # of image cols to skip / 2
	mov	r6, #0
	mov	r5, resx2	; # of image cols to draw / 2
;
; here: R3 - # of image-lines / 2
;       R4 - # of background-color lines / 2
;       R5 - # of image-cols / 2
;       R6 - # of background-color cols / 2
;       R7 - # of image-cols per line to skip / 2
;
lloop:	acall	doline
	acall	doline
	djnz	r3, lloop
;
	mov	a, r4
	jz	exit
cloop:	mov	r2, resx2
	acall	clrline
	mov	r2, resx2
	acall	clrline
	djnz	r4, cloop
;
exit:	setb	SPI_CS
	mov	a, spibuf
	setb	ea
	ret
;
clrline:
	mov	a, _g_bgcol + 1
	lcall	otp_lcd_write
	mov	a, _g_bgcol
	lcall	otp_lcd_write
	mov	a, _g_bgcol + 1
	lcall	otp_lcd_write
	mov	a, _g_bgcol
	lcall	otp_lcd_write
	djnz	r2, clrline
	ret
;
doline:
	mov	a, r5
	mov	r1, a
bsl:	jnb	_SPIPND, .
	xch	a, spibuf
	lcall	otp_lcd_write
	jnb	_SPIPND, .
	xch	a, spibuf
	lcall	otp_lcd_write
	jnb	_SPIPND, .
	xch	a, spibuf
	lcall	otp_lcd_write
	jnb	_SPIPND, .
	xch	a, spibuf
	lcall	otp_lcd_write
	djnz	r1, bsl
	mov	a, r6
	mov	r2, a
	jnz	clrline
	mov	a, r7
	jz	x2
	mov	r2, a
bsl1:	jnb	_SPIPND, .
	xch	a, spibuf
	jnb	_SPIPND, .
	xch	a, spibuf
	jnb	_SPIPND, .
	xch	a, spibuf
	jnb	_SPIPND, .
	xch	a, spibuf
	djnz	r2, bsl1
x2:	ret

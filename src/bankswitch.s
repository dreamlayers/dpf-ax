; 
; Bank switching implementation for dynamic code loading from flash
;
	.module bankswitch
	.include 'dpf.inc'
	.include 'config.inc'

	.globl	G_bank
	
	.area REG_BANK_3	(REL, DATA)

	; Global register bank variable (IRAM)
G_bank:
	.ds	1

	.area HOME (CODE)

__sdcc_banked_call::
	push	G_bank	; (
	xch	a, r0	; target addr L
	push	acc	; {
	mov	a, r1	; target addr H
	push	acc	; {
	mov	a, r2	; Bank
	cjne	a, G_bank, load_data	; reload if bank has changed.
	xch	a, r0	; restore A
	ret	; Jump into target addr

__sdcc_banked_ret::
	xch	a, r0	; save a
	pop	acc	; )
	cjne	a, G_bank, load_data	; reload if bank not changed.
	xch	a, r0	; restore A
	ret

; Wrapper to only load memory segments without bank switch
; Argument: Bank number
_bank_load::
	mov	a, dpl
	sjmp	load
	
load_data:
	; r0 still contains original value of a
	mov	G_bank, a	; Store bank index

load:
	push	ie	; Save interrupt flags	; {
	clr	ea	; Disable IRQs


	; We have to deal with two cases, as we multiply by 8:
	; a <= 31: no problem
	; a > 31 : We have to deal with overflow

	; For now, we're lazy and don't cover case B

	; a <<= 3:
	swap	a
	rr	a

	anl	a, #0xf8	; Mask former MSBs
	add	a, #(jumptable_offset >> 0)
	push	acc	; {
	clr	a
	addc	a, #(jumptable_offset >> 8)
	mov	r1, a	; address M
	clr	a
	addc	a, #(jumptable_offset >> 16)
	mov	r2, a	; address H
	
	; Now read dest, end, and address from SPI:

	mov	a, #SPM_READ
	clr	SPI_CS
	mov	spibuf, a	; Write CMD
	mov	a, r2	; Addr H
	acall	spi_act
	mov	a, r1	; Addr M
	acall	spi_act
	pop	acc	; } Addr L
	acall	spi_act

	acall	spi_act ; One dummy transaction for read

	; save dp, we're gonna modify it
	push	dpl	; {
	push	dph	; {

	acall	spi_act ; Now read data:
	mov	dph, a	; Addr H
	acall	spi_act
	mov	dpl, a	; Addr L
	acall	spi_act
	mov	r2, a	; End H
	acall	spi_act
	mov	r1, a	; End L

	; Now read offset addr:
	; This stores it in reverse order we actually want it later:
	acall	spi_act
	push	acc	; {
	acall	spi_act
	push	acc	; {
	acall	spi_act
	push	acc	; {
w0:	jnb	_SPIPND, w0
	setb	SPI_CS	; Finish transaction
	mov	a, spibuf
	
spi_load:
	mov	a, #SPM_READ
	clr	SPI_CS	; Select SPI
	mov	spibuf, a
; Trick: We just have '3' in a:
; So pop the next 3 address values:
load3b:
	jnb	_SPIPND, load3b
	pop	spibuf	; }}}
	djnz	acc, load3b

	acall	spi_act	; one dummy
	; Save dpcon for reentrancy
	push	dpcon	; {
	orl	dpcon, #DPAID
	; Now start reading:
repeat:
	jnb	_SPIPND, repeat
	xch	a, spibuf
	movx	@dptr, a
	mov	a, r1
	cjne	a, dpl, repeat
	mov	a, r2
	cjne	a, dph, repeat
w1:	jnb	_SPIPND, w1
	setb	SPI_CS

	pop dpcon; }

	pop	dph	; }
	pop	dpl	; }
	; lcall	0x0fae	; Unknown
	pop	ie	; }
	xch	a, r0	; restore a
	; And jump into target address:
	ret	; }}

spi_act:
	jnb	_SPIPND, spi_act
	xch	a, spibuf
	ret


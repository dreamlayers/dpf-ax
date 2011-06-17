; Patch for startup (mod41 on DX_blue)
	.include 'dpf.inc'
	.include 'dpf_int.inc'
	.include 'hack.inc'

	.area HOME (CODE)
	.area BANK0 (CODE, ABS)

	.org 0x1418
	lcall	0x13ac
        orl     wdtcon,#0x20
        mov     a,i_G_f
        jbc     acc.3, usbact	; If this bit is 1, USB is plugged in.
	sjmp	continue
usbact:
	ljmp	hack

continue:
	ljmp	tramp_return

hack:
	clr	ea	; No IRQ
	mov	dptr, #(0x196a - 0x800)
	mov	a, #cloned_jumptable_offset >> 16
	movx	@dptr, a

	pop	acc	; Fixup stack, as we did a jsr
	pop	acc	; ..and a bank jsr

	; Jump into our firmware
	mov	a,#(50 - 1)
	mov	dptr,#entry_addr
	ljmp	tramp_jmp


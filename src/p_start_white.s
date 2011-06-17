; Patch for startup (mod37 on white)
	.include 'dpf.inc'
	.include 'dpf_int.inc'
	.include 'hack.inc'

	.area HOME (CODE)
	.area BANK0 (CODE, ABS)

	.org 0x1330

        orl     wdtcon,#0x20
        mov     a,i_G_f
        jbc     acc.3, usbact	; If this bit is 1, USB is plugged in.
	sjmp	continue
usbact:
	ljmp	hack

	.org 0x133c
continue:

	.org 0x1426
hack:
	clr	ea	; No IRQ
	mov	dptr, #(0x196a - 0x800)
	mov	a, #cloned_jumptable_offset >> 16
	movx	@dptr, a

	; Jump into usb main:
	mov	a,#(53 - 1)
	mov	dptr,#entry_addr
	ljmp	tramp_jmp


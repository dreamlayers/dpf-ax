; Somewhat more efficient USB to screen blitting:

	.include 'dpf.inc'

	.area UHANDLER (CODE)

	EP1_PKTSIZE = 64

usb_commit:
	anl	usbcon,#~USBRNW
	orl	usbcon,#USBKS
wait3:	jnb	_USBDONE,wait3
	ret

usb_wait_rxf:
	mov	usbadr, #OutCSR1
repeat1:	jb _g_usb_active, poll1
	ret
poll1:
	orl	usbcon, #(USBRNW | USBKS)
wait1:	jnb	_USBDONE, wait1
	clr	_USBDONE
	mov	a, usbdata
	jnb	acc.0, repeat1
	ret

usb_ack_rx:
	mov	usbadr, #OutCSR1
	mov	usbdata, #0x00
	sjmp	usb_commit

usb_readbyte:
	orl	usbcon, #(USBKS | USBRNW)
wait:	jnb	_USBDONE, wait
	mov	a, usbdata
	ret

_usb_to_screen::
	; Get length from provided buffer
	mov	r0, dpl
	mov	a, @r0
	mov	r3, a
	inc	r0
	mov	a, @r0
	mov	r4, a
	inc	r0
	mov	a, @r0
	mov	r5, a

usb_to_screen:
	; Length is in r3:r4:r5 (LE)
	mov	a, r3
	mov	b, a	; save low byte
	rlc	a
	xch	a, r4
	rlc	a
	xch	a, r5
	rlc	a
	mov	r3, a
	; now rotated by 1 in r4:r5:r3
	mov	a, r4
	rlc	a
	xch	a, r5
	rlc	a
	xch	a, r3
	rlc	a
	mov	r4, a
	; now rotated by 2 in r5:r3:r4
	; and we're only interested in r3:r4 for the outer loop

	; Calculate remainder:
	mov	a, b
	rr	a; >>= 1
	anl	a, #0x1f
	mov	r5, a

next_pkt:
	mov	a, r3
	orl	a, r4
	jz	write_remainder

	; Wait for a RX buffer:
	acall	usb_wait_rxf
	; Now we have a packet ready, process it:
	mov	r7, #(EP1_PKTSIZE >> 1)
	mov	usbadr, #FIFO1
repeat32:
	acall	usb_readbyte
	lcall	lcd_write	; write color byte 0
	acall	usb_readbyte
	lcall	lcd_write	; write color byte 1
	djnz	r7, repeat32
	acall	usb_ack_rx	; finish packet rx

	dec	r3
	cjne	r3, #0xff, no_wrap
	dec	r4
no_wrap:

	sjmp	next_pkt

	; Now send remaining <64 byte packet

write_remainder:
	mov	a, r5
	jz	skip_remainder	; If no remainder, skip it

	acall	usb_wait_rxf
	mov	usbadr, #FIFO1
repeat_n:
	acall	usb_readbyte
	lcall	lcd_write	; write color byte 0
	acall	usb_readbyte
	lcall	lcd_write	; write color byte 1
	djnz	r5, repeat_n
	acall	usb_ack_rx	; finish packet rx
skip_remainder:
	ret


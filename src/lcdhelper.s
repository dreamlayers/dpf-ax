	.include 'dpf.inc'

;
; Helpers for lcdinit.s
;
	.area LCDAUX (CODE)

lcd_reset::
	mov	p3dir,#0
	anl	p1dir,#0xe8
	anl	p2dir,#0xfd
        orl     wdtcon,#0x20
	clr	LCD_CS
	clr	LCD_RST
	mov	a,#0x96
	lcall	delay
	setb	LCD_RST
	mov	a,#0xc8
	lcall	delay
        ret
;
delay:	mov	r5,a
dl1:	mov	r6,#0x18*2
dl2:	mov	r7,#0xfa
dl3:	djnz	r7,dl3
	djnz	r6,dl2
	djnz	r5,dl1
	ret	

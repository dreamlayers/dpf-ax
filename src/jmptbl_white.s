; This is the jump table patch for the 'white' type DPF
; It's activated through the first stage hack
; This is necessary, because the first sector can not be patched safely.

	.area HOME(CODE)
	.area DSEG (DATA,ABS)

	.include 'hack.inc'

	TERMINATOR = 0x00

; Module p53:
	code_start.53	= start_addr  ; DO NOT CHANGE. ABSOLUTE.
	code_end.53	= end_addr
	offset.53		= load_offset
	
	.org 0x0228
p53:
; New mod53:
	.dw	code_start.53 - 0x800, code_end.53 - 0x800
	.db	offset.53, offset.53 >> 8, offset.53 >> 16
	.db	TERMINATOR

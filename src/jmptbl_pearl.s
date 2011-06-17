; This is the jump table patch for the pearl type DPF
; It's activated through the first stage hack
; This is necessary, because the first sector can not be patched safely.

	.area HOME(CODE)
	.area DSEG (DATA,ABS)

	TERMINATOR = 0x00

	.include 'hack.inc'

; Module p54:
	code_start.54	= start_addr  ; DO NOT CHANGE. ABSOLUTE.
	code_end.54	= end_addr
	offset.54		= load_offset
	
	.org 0x0230
p54:
; New mod54:
	.dw	code_start.54 - 0x800, code_end.54 - 0x800
	.db	offset.54, offset.54 >> 8, offset.54 >> 16
	.db	TERMINATOR

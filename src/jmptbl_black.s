; This is the jump table patch for the 'black' type DPF
; It's activated through the first stage hack
; This is necessary, because the first sector can not be patched safely.

	.area HOME(CODE)
	.area DSEG (DATA,ABS)

	.include 'hack.inc'

	TERMINATOR = 0x00

; Module p51:
	code_start.51	= start_addr  ; DO NOT CHANGE. ABSOLUTE.
	code_end.51	= end_addr
	offset.51		= load_offset
	
	.org 0x0218
p51:
; New mod53:
	.dw	code_start.51 - 0x800, code_end.51 - 0x800
	.db	offset.51, offset.51 >> 8, offset.51 >> 16
	.db	TERMINATOR

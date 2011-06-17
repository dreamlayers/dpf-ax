; This is the jump table patch for the silver2 type DPF
; It's activated through the first stage hack
; This is necessary, because the first sector can not be patched safely.

	.area HOME(CODE)
	.area DSEG (DATA,ABS)

	.include 'hack.inc'

	TERMINATOR = 0x00

; This is the bootstrap for our own firmware:
	code_start.52	= start_addr    ; Start of HOME
	code_end.52	= end_addr
	offset.52	= load_offset
	
; User code escape bootstrap:
	.org 0x228
p52:
	.dw	code_start.52 - 0x800, code_end.52 - 0x800
	.db	offset.52, offset.52 >> 8, offset.52 >> 16
	.db	TERMINATOR

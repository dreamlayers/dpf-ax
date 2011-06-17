; This is the jump table patch for the pearl type DPF
; It's activated through the first stage hack
; This is necessary, because the first sector can not be patched safely.

	.area HOME(CODE)
	.area DSEG (DATA,ABS)

	.include 'hack.inc'

	TERMINATOR = 0x00

; Module p50:
; This is the bootstrap for our own firmware:
	code_start.50	= start_addr  ; Start of HOME
	code_end.50	= end_addr
	offset.50	= load_offset
	
; User code escape bootstrap:
	.org 0x210
p50:
	.dw	code_start.50 - 0x800, code_end.50 - 0x800
	.db	offset.50, offset.50 >> 8, offset.50 >> 16
	.db	TERMINATOR

	.include 'dpf.inc'

	.area INIT (CODE)

; Just a nop at the moment - only landscape supported!
; If you want other orientations, add code here,
; create a correspondig method to lcdblit_*.s and modify lcdinit_*.c accordingly.

_lcd_custom_setorientation::
	ret

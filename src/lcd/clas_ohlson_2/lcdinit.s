	.include 'dpf.inc'

	.area LCDAUX (CODE)

_lcd_custom_init::
	lcall	lcd_reset
;
	mov	dptr,#_custom_initseq
	ljmp	_lcd_init_by_table

;
; If custom backlight handling is needed, uncomment the following label
; and set LCD_BACKLIGHT_CUSTOM in dpfmodel.h
; r3 contains brightness value (0 .. LCD_MAX_BRIGHTNESS_VALUE)
;
;_lcd_custom_setbacklight::

;
; If custom contrast handling is needed, uncomment the following label
; and set LCD_CONTRAST_CUSTOM in dpfmodel.h
; r3 contains contrast value (1 .. LCD_MAX_CONTRAST_VALUE)
;
;_lcd_custom_setcontrast::

	.area LCDAUX (CODE)


;backlight_table::
;	.db	If needed, put data for custom backlight handling here
;_custom_backlighttbl_len::  .db  . - backlight_table


;contrast_table::
;	.db	If needed, put data for custom contrast handling here
;_custom_contrasttbl_len::  .db  . - contrast_table

_custom_contrasttbl::
	.db	0x26, 0x30, 0x32, 0x34, 0x36, 0x38, 0x3a
_custom_contrasttbl_len::  .db  7

_custom_contrasttbl2::
	.db	0x71, 0x25, 0x91, 0x36, 0xff
_custom_contrasttbl2_len::  .db  5

_custom_contrasttbl2_offsets::
	.db	0x03	;0x05 <-- org fw value WORNG
_custom_contrasttbl2_offsets_len::  .db  1

_custom_backlighttbl::
	.db	0x00, 0x12, 0x24, 0x36, 0x48, 0x5a, 0x6c, 0x9c, 0xa8, 0xb4
	.db	0xc0, 0xcc, 0xd8, 0xe4, 0xf0, 0xfc, 0xfc, 0xcc, 0xd8, 0xe4
	.db	0xf0, 0xfc
_custom_backlighttbl_len::  .db  22

_custom_backlighttbl2::
_custom_backlighttbl2_len::  .db  0

_custom_initseq::
	.db	0x71, 0x01, 0xb0, 0x32, 0x71, 0x11, 0xb0, 0xc8, 0x71, 0xb1
	.db	0x92, 0x05, 0x01, 0x71, 0xc0, 0x92, 0x04, 0x1a, 0x71, 0xc1
	.db	0x91, 0x03, 0x71, 0xc2, 0x91, 0x05, 0x71, 0xc5, 0x92, 0x4f
	.db	0x40, 0x71, 0xec, 0x91, 0x02, 0x71, 0xf2, 0x91, 0x01, 0x71
	.db	0xe0, 0x9f, 0x3b, 0x2b, 0x2a, 0x2d, 0x28, 0x0c, 0x57, 0xf0
	.db	0x41, 0x12, 0x15, 0x12, 0x11, 0x10, 0x04, 0x71, 0xe1, 0x9f
	.db	0x04, 0x14, 0x15, 0x12, 0x17, 0x13, 0x28, 0x60, 0x3e, 0x0d
	.db	0x2a, 0x2d, 0x2e, 0x2f, 0x3b, 0x71, 0x36, 0x91, 0xc0, 0x71
	.db	0x3a, 0x91, 0x65, 0x71, 0x2a, 0x94, 0x00, 0x02, 0x00, 0x81
	.db	0x71, 0x2b, 0x94, 0x00, 0x03, 0x00, 0x82, 0x71, 0x29, 0xb0
	.db	0x64, 0x71, 0x2c, 0xff
_custom_initseq_len::  .db  104


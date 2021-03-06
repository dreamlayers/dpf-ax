	.include 'dpf.inc'

	.area LCDAUX (CODE)

_lcd_custom_init::
	anl	p2dir,#0xf7	;start with backlight off
	anl	p2,#0xf7
;
	mov	dptr,#_custom_initseq
	ljmp	_lcd_init_by_table

;
; If custom backlight handling is needed, uncomment the following label
; and set LCD_BACKLIGHT_CUSTOM in dpfmodel.h
; r3 contains brightness value (0 .. LCD_MAX_BRIGHTNESS_VALUE)
;
_lcd_custom_setbacklight::
	anl	p2dir,#0xf7
	mov	a,r3
	jnz	backlight_on
	anl	p2,#0xf7
	ret
backlight_on:
	orl	p2,#0x08
	ret
;
;
; If custom contrast handling is needed, uncomment the following label
; and set LCD_CONTRAST_CUSTOM in dpfmodel.h
;
;_lcd_custom_setcontrast::

	.area LCDAUX (CODE)


;backlight_table::
;	.db	If needed, put data for custom backlight handling here
;_custom_backlighttbl_len::  .db  . - backlight_table


;contrast_table::
;	.db	If needed, put data for custom contrast handling here
;_custom_contrasttbl_len::  .db  . - contrast_table

;
; NOTE from identify.py: not used
;
;_custom_contrasttbl::
;  .db  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
;_custom_contrasttbl_len::  .db  7

;_custom_contrasttbl2::
;  .db  0x71, 0x00, 0x71, 0x13, 0x91, 0x0c, 0x91, 0x00, 0xff
;_custom_contrasttbl2_len::  .db  9

;_custom_contrasttbl2_offsets::
;  .db  0x07
;_custom_contrasttbl2_offsets_len::  .db  1

;
; NOTE from identify.py: not used
;
;_custom_backlighttbl::
;  .db  0x32, 0x3c, 0x46, 0x50, 0x5a, 0x64, 0x6e, 0x78, 0x82, 0x8c
;  .db  0x96, 0xa0, 0xaa, 0xb4, 0xbe, 0xc8, 0xd2, 0xdc, 0xe6, 0xf0
;  .db  0xff
;_custom_backlighttbl_len::  .db  21

;_custom_backlighttbl2::
;_custom_backlighttbl2_len::  .db  0

_custom_initseq::
  .db  0x00, 0xb0, 0x64, 0x11, 0xb0, 0x0a, 0x10, 0xb0, 0x64, 0x11
  .db  0xb0, 0xff, 0x71, 0x11, 0xb0, 0x78, 0x71, 0xcf, 0x91, 0x00
  .db  0x91, 0x81, 0x91, 0x30, 0x71, 0xed, 0x91, 0x67, 0x91, 0x03
  .db  0x91, 0x12, 0x91, 0x81, 0x71, 0xcd, 0x91, 0x39, 0x91, 0x2c
  .db  0x91, 0x00, 0x91, 0x34, 0x91, 0x02, 0x71, 0xea, 0x91, 0x00
  .db  0x91, 0x00, 0x71, 0xe8, 0x91, 0x85, 0x91, 0x0a, 0x91, 0x78
  .db  0x71, 0xc0, 0x91, 0x21, 0x71, 0xc1, 0x91, 0x11, 0x71, 0xc5
  .db  0x91, 0x3f, 0x91, 0x3c, 0x71, 0xc7, 0x91, 0xa5, 0x71, 0x3a
  .db  0x91, 0x55, 0x71, 0x36, 0x91, 0x6c, 0x71, 0xb1, 0x91, 0x00
  .db  0x91, 0x1b, 0x71, 0xb6, 0x91, 0x0a, 0x91, 0xa2, 0x71, 0xf2
  .db  0x91, 0x00, 0x71, 0x26, 0x91, 0x01, 0x71, 0xe0, 0x91, 0x0f
  .db  0x91, 0x26, 0x91, 0x24, 0x91, 0x0b, 0x91, 0x0e, 0x91, 0x09
  .db  0x91, 0x54, 0x91, 0xa8, 0x91, 0x46, 0x91, 0x0c, 0x91, 0x17
  .db  0x91, 0x09, 0x91, 0x0e, 0x91, 0x07, 0x91, 0x00, 0x71, 0xe1
  .db  0x91, 0x00, 0x91, 0x19, 0x91, 0x1b, 0x91, 0x04, 0x91, 0x10
  .db  0x91, 0x07, 0x91, 0x2a, 0x91, 0x47, 0x91, 0x39, 0x91, 0x03
  .db  0x91, 0x06, 0x91, 0x06, 0x91, 0x30, 0x91, 0x38, 0x91, 0x0f
  .db  0x71, 0x29, 0xf0, 0x0a, 0xff
_custom_initseq_len::  .db  175

; Original DPF firmare static definitions

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; DEFINES
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Button event codes:

BUT_UP		= 0x11
BUT_DOWN	= 0x12
BUT_MENU	= 0x16

; Background color, FIXME: Put into global variable

BGCOL_0 = 0x00
BGCOL_1 = 0x00
; Globals

; global bits:
b_usb_act 	= 0x04

; global (hack) variables:
G_button        = 0x1e
G_bankno        = 0x1f
i_G_f		= 0x20	; some USB flags, set by IRQ handler
slideshow_t	= 0xc8	; Slide show time

g_usb_tabindex  = 0x50
g_usb_rec       = 0x51

; LCD 'cursor' -- this is the screen context area
lcd_cxl = 0x77 ; 
lcd_cxh = 0x78 ; 
lcd_cyl = 0x79 ; 
lcd_cyh = 0x7a ; 
lcd_dxl = 0x7b ; 
lcd_dxh = 0x7c ; 
lcd_dyl = 0x7d ; 
lcd_dyh = 0x7e ; 

; Only for 128x128 DPFs:
screen_len_h          = 0x40
screen_len_l          = 0x00


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 8052 memory:
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
usb_buf		= 0x80
replybuf1	= 0x84
replybuf2	= 0x9f

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; FUNCTIONS in common area (might be dumped later)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

tramp_return          = 0x1934
tramp_jsr             = 0x1938
tramp_jmp             = 0x193a
tramp_dyn             = 0x193e
common_1818           = 0x1818

lcd_address           = 0x1280  ; The common display configuration routine



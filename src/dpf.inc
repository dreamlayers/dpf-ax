; DPF program environment definitions

	.include 'ax206.inc'
; SPI flash definitions
	.include 'spiflash.inc'

; Global variables;

; LCD pins
LCD_RST	=	_P10
LCD_WR	=	_P11
LCD_RD	=	_P12
LCD_A0	=	_P14
LCD_CS	=	_P21
LCD_LED	=	_P23

; SPI pins
SPI_CS	= _P20

; FUNCTIONS in OTP
otp_lcd_write         = 0x0f25 ; The display write function



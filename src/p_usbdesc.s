; Hacked DPF descriptors

	.module descriptors

VENDOR_ID	= 0x1908	;
PRODUCT_ID	= 0x0102	;
PRODUCT_VERSION	= 0x0200	; 2.0


	.include 'usb.inc'

;; -----------------------------------------------------------------------------
	.area	HOME	(CODE,ABS)
	.org    0x1508

_usb_desc_device::
	.db	USB_DT_DEVICE_SIZE
	.db	USB_DT_DEVICE
	.db	0x10, 0x1	; USB version 2.0 (BCD)
	.db	0xff		; device class
	.db	0xff		; sub class
	.db	0xff		; protocol (0xff == vendor-specific)
	.db	8		; EP0 max packet size
	; Note little endianness:
	.db	<VENDOR_ID, >VENDOR_ID;
	.db	<PRODUCT_ID, >PRODUCT_ID;
	.db	<PRODUCT_VERSION, >PRODUCT_VERSION;
	.db	2		; manufacturer string idx
	.db	3		; product string idx
	.db	1		; serial number string idx
	.db	1		; number of configurations

; string table:
	.org 0x151a
strings: 
	.dw  _usb_desc_string_language
	.dw  _usb_desc_string_serial
	.dw  _usb_desc_string_manufacturer
	.dw  _usb_desc_string_product
	.dw  _usb_desc_string_iface

; note: strings are UNICODE
_usb_desc_string_language::
	.db	4		; length
	.db	USB_DT_STRING
	.db	0x09, 0x04	; UNICODE language ID

; string 1
_usb_desc_string_manufacturer::
	.db	(usb_desc_string_manufacturer_end - _usb_desc_string_manufacturer)
	.db	USB_DT_STRING
	.db	'h, 0
	.db	'a, 0
	.db	'c, 0
	.db	'k, 0
	.db	'f, 0
	.db	'i, 0
	.db	'n, 0
usb_desc_string_manufacturer_end:

_usb_desc_string_serial::
	.db	(usb_desc_string_serial_end - _usb_desc_string_serial)
	.db	USB_DT_STRING
	.db	'0, 0
	.db	'0, 0
	.db	'0, 0
	.db	'0, 0
usb_desc_string_serial_end:

; string 2
_usb_desc_string_product::
	.db	(usb_desc_string_product_end - _usb_desc_string_product)
	.db	USB_DT_STRING
	.db	'U, 0
	.db	'S, 0
	.db	'B, 0
	.db	'-, 0
	.db	'D, 0
	.db	'i, 0
	.db	's, 0
	.db	'p, 0
	.db	'l, 0
	.db	'a, 0
	.db	'y, 0
usb_desc_string_product_end:

; string 3
_usb_desc_string_iface::
	.db	(usb_desc_string_iface_end - _usb_desc_string_iface)
	.db	USB_DT_STRING
	.db	'l, 0
	.db	'i, 0
	.db	'b, 0
	.db	'u, 0
	.db	's, 0
	.db	'b, 0

usb_desc_string_iface_end:

	.org 0x159a

_usb_desc_config::
	.db	USB_DT_CONFIG_SIZE
	.db	USB_DT_CONFIG
	.db	(usbdesc_config_end - _usb_desc_config) & 0xff
	.db	(usbdesc_config_end - _usb_desc_config) >> 8
	.db	1		; number of interfaces
	.db	1		; configuration number
	.db	0		; configuration string idx
	.db	0x80		; flags: self-powered, remote wakeup
	.db	100		; FIXME: bus power in 2mA units

	.db	USB_DT_INTERFACE_SIZE
	.db	USB_DT_INTERFACE
	.db	0		; interface number
	.db	0		; alternate setting
	.db	2		; number of endpoints
	.db	0xff		; interface class (0xff == vendor specific)
	.db	0x00		; sub-class
	.db	0		; protocol
	.db	4		; interface string idx

	.db	USB_DT_ENDPOINT_SIZE
	.db	USB_DT_ENDPOINT
	.db	(1 | USB_DIR_IN)	; EP number and direction
	.db	USB_ET_BULK
	.db	64			; maximum packet size (lsb)
	.db	0			; maximum packet size (msb)
	.db	0			; polling interval

	.db	USB_DT_ENDPOINT_SIZE
	.db	USB_DT_ENDPOINT
	.db	(1 | USB_DIR_OUT)	; EP number and direction
	.db	USB_ET_BULK
	.db	64			; maximum packet size (lsb)
	.db	0			; maximum packet size (msb)
	.db	0			; polling interval

usbdesc_config_end:   

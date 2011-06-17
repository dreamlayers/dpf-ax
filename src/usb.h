/* USB prototypes and data
 *
 */

#define EP0_PKTSIZE 8
#define EP1_PKTSIZE 64

extern volatile
__bit g_usb_reset;     // Reset USB flag
extern volatile
__bit g_usb_ep0_pkt;    // EP0 packet ready

#define usb_select(reg) usbadr = reg
#define usb_flag_set(flag) (usbdata & flag)

extern __data
	unsigned char ep0buf[EP0_PKTSIZE];

unsigned char handler_usb_ep0(void) __banked;
char handler_usb_ep1(void) __banked;
void usb_config(void) __banked;
unsigned char usb_poll(void) __banked;

#define USB_REQ_GET_STATUS              0x00
#define USB_REQ_CLEAR_FEATURE           0x01
#define USB_REQ_SET_FEATURE             0x03
#define USB_REQ_SET_ADDRESS             0x05
#define USB_REQ_GET_DESCRIPTOR          0x06
#define USB_REQ_SET_DESCRIPTOR          0x07
#define USB_REQ_GET_CONFIGURATION       0x08
#define USB_REQ_SET_CONFIGURATION       0x09
#define USB_REQ_GET_INTERFACE           0x0A
#define USB_REQ_SET_INTERFACE           0x0B
#define USB_REQ_SYNCH_FRAME             0x0C

// Request definitions
#define REQTYPE_RCP_MASK                0x1f

#define USB_RCP_DEVICE                  0x00
#define USB_RCP_INTERFACE               0x01
#define USB_RCP_ENDPOINT                0x02
#define USB_RCP_OTHER                   0x03

// Feature request types
#define USB_FT_ENDPOINT_HALT               0
#define USB_FT_DEVICE_REMOTE_WAKEUP        1
#define USB_FT_TEST_MODE                   2

// Descriptor tokens:
#define USB_DT_DEVICE                   0x01
#define USB_DT_CONFIG                   0x02
#define USB_DT_STRING                   0x03
#define USB_DT_INTERFACE                0x04
#define USB_DT_ENDPOINT                 0x05
#define USB_DT_DEVICE_QUALIFIER         0x06
#define USB_DT_OTHER_SPEED_CONFIG       0x07
#define USB_DT_INTERFACE_POWER          0x08

// USB EP definitions
#define USB_DIR_IN  0x80
#define USB_DIR_OUT 0x00
#define USB_EP0        0
#define USB_EP1        1

// Request types
#define USB_RT_DIR_MASK                 0x80

#define REQTYPE_TYPE_MASK               0x60

#define USB_RT_STD                      0x00
#define USB_RT_CLASS                    (1 << 5)
#define USB_RT_VENDOR                   (2 << 5)

unsigned char usb_get_status(void) __banked;
unsigned char usb_clear_feature(void) __banked;
unsigned char usb_set_feature(void) __banked;

// Common:
void usb_putc(unsigned char c);
unsigned char usb_readbyte(void);


// Descriptor structures:

typedef __code unsigned char *DescStringP;
typedef __code unsigned char DescString[];

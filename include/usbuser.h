/* USB user commands
 *
 * Only temporary. Should move to dpflib or into a dclib configuration.
 *
 */

#define PROTOCOL_VERSION  1

/** Our vendor specific USB commands to do stuff on the DPF */

#define USBCMD_GETPROPERTY  0x00    ///< Get property
#define USBCMD_SETPROPERTY  0x01    ///< Set property
#define USBCMD_MEMREAD      0x04    ///< Memory read
#define USBCMD_APPLOAD      0x05    ///< Load and run applet
#define USBCMD_FILLRECT     0x11    ///< Fill screen rectangle
#define USBCMD_BLIT         0x12    ///< Blit to screen
#define USBCMD_COPYRECT     0x13    ///< Copy screen rectangle
#define USBCMD_FLASHLOCK    0x20    ///< Lock USB for flash access
#define USBCMD_PROBE        0xff    ///< Get version code (probe)

/* Some special return codes */
#define USB_IN_SEQUENCE     0x7f    ///< We're inside a command sequence

// Property handling:

#define PROPERTY_BRIGHTNESS  0x01
#define PROPERTY_FGCOLOR     0x02
#define PROPERTY_BGCOLOR     0x03
#define PROPERTY_ORIENTATION 0x10

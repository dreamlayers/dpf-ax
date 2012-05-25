/** Simple experimental support for overlay data
 *
 * Normally, data that is shared among modules (strings, etc) go in here.
 * The reason is, that banked call switches destroy code memory.
 * Thus, we need to take care of the overlaying ourselves.
 * 
 * If we need to write to this segment, we need to do it via xdata.
 * Example: to change last digit of g_usbserial to '9':
 * __xdata unsigned char *p = ((__xdata unsigned char *) &g_usbserial[6]) - 0x800;
 * *p = '9';
 */

#pragma constseg DATA0

#include "dpf.h"

// For later.
// #ifndef OVLDATA
// #error "You must define OVLDATA for the overlay data area"
// #endif

#include "global.h"

const OVLAREA char g_rebootmsg[] = "Rebooting...";

OVLAREA char g_usbserial[] =
                    { sizeof(g_usbserial),      // length
                     0x03,                      // USB_DT_SERIAL
                     '0', 0,                    // Serial #
                     '0', 0,
                     '1', 0 };

//
// CAUTION! No overlapp-check with following segments MENUx!
// Make sure max data length < 0x40 bytes!
//


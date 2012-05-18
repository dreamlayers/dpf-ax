/** Simple experimental support for overlay data
 * Currently a stub.
 *
 * Normally, data that is shared among modules (strings, etc) go in here.
 * The reason is, that banked call switches destroy code memory.
 * Thus, we need to take care of the overlaying ourselves.
 */

#pragma constseg DATA0

#include "dpf.h"

// For later.
// #ifndef OVLDATA
// #error "You must define OVLDATA for the overlay data area"
// #endif

#include "global.h"

const OVLAREA char g_rebootmsg[] = "Rebooting...";

//
// CAUTION! No overlapp-check with following segments MENUx!
// Make sure max data length < 0x40 bytes!
//


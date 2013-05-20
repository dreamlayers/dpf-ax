/* Core DPF access library header for AX206 based HW.
 *
 * Includes only code necessary to access a hacked frame.
 * For the full library see http://dpf-ax.sourceforge.net/.
 *
 * 05/2013 superelchi <superelchi AT wolke7.net>
 *
 */
	
/*
 ********************************************************************
 * 
 * Include this file in your drivers distribution
 * or cut & paste the following code at the start of your driver.
 *
 ********************************************************************
 */

//###################################################################
// Start dpfcore4driver.h
// See http://dpf-ax.sourceforge.net/
//###################################################################

#ifndef	_DPF_AX_H
#define _DPF_AX_H 1

#ifdef __cplusplus
extern "C" {
#endif

#define DPFAXHANDLE void *	// Handle needed for dpf_ax access
#define DPF_BPP 2		//bpp for dfp-ax is currently always 2!

/**
 * Open DPF device.
 * 
 * Device must be string in the form "usbX" or "dpfX", with X = 0 .. number of connected dpfs.
 * The open function will scan the USB bus and return a handle to access dpf #X.
 * If dpf #X is not found, returns NULL.
 *
 * \param dev	device name to open
 * \return		device handle or NULL
 */
DPFAXHANDLE dpf_ax_open(const char *device);

/**
 *  Close DPF device.
 */
void dpf_ax_close(DPFAXHANDLE h);

/** Blit data to screen.
 *
 * \param buf     buffer to 16 bpp RGB 565 image data
 * \param rect    rectangle tuple: [x0, y0, x1, y1]
 */
void dpf_ax_screen_blit(DPFAXHANDLE h, const unsigned char *buf, short rect[4]);

/** Set backlight brightness.
 * 
 * \param value       Backlight value 0..7 (0 = off, 7 = max brightness)
 */
void dpf_ax_setbacklight(DPFAXHANDLE h, int value);

/** Get screen width.
 * 
 * \return width in pixel
 */
int dpf_ax_getwidth(DPFAXHANDLE h);

/** Get screen height.
 * 
 * \return height in pixel
 */
int dpf_ax_getheight(DPFAXHANDLE h);

#ifdef __cplusplus
}
#endif
#endif /* _DPF_AX_H */

//###################################################################
// End dpfcore4driver.h
//###################################################################

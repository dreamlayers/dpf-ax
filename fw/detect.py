#!/usr/bin/python
# Simple detection module for AX206 based DPF hardware
# (c) 2011 <hackfin@section5.ch>
#
# Run with argument:
#
# - generic scsi device, e.g. /dev/sg1
# or:
# - "usb0" for hacked device
#
# NOTE: The LCD detection will only work on a hacked device

import struct
import sys
sys.path.append("../Debug")
import dpf

g_spansion_types = {
	0x13: ("s25fl008", 16, 0x10000)
}

g_stm_types = {
	0x14: ("m25p80", 16, 0x10000),
	0x15: ("m25p16", 32, 0x10000),
	0x16: ("m25p32", 64, 0x10000)
}

g_sst_types = {
	0x8e: ("SST-25VF080B", 32, 0x10000),
}

g_amic_types = {
	0x13: ("a25l080", 16, 0x10000),
}

g_vendors = {
	0x00: ("ST legacy", g_stm_types),
	0x01: ("Spansion", g_spansion_types),
	0x20: ("ST/Numonyx", g_stm_types),
	0x1c: ("EON", g_stm_types),
	0x37: ("AMIC", g_amic_types),
	0xc2: ("Macronix", g_stm_types),
	0xbf: ("SST", g_sst_types),
	0xc8: ("Apple", g_stm_types),
	0xef: ("Winbond", g_stm_types)
}

g_lcdmanu = {
	0x45: "Philips/NXP",
	0x5c: "Sitronix",
	0x54: "Ilitek"
}

g_lcdctrl = {
	0x66: "ILI9163B",
	0xf0: "ST7735R"
}


def detect_lcd(d):
	d.loadApp("app_detect.ihx")
	d.run(0x18a0)
	buf = d.readMemory(0x1900, 4)

	v = struct.unpack("BBBB", buf)

	if v[1] != 0xff:
		try:
			manu = g_lcdmanu[v[1]]
			print "Manufacturer:", manu
			print "Module/driver version:", hex(v[2])
			print "Identified module:", g_lcdctrl[v[3]]
		except KeyError:
			print "Unknown module id", hex(v[1]), hex(v[2]), hex(v[3])
	else:
		print "Does not support RDID"

def detect_flash(d):
	manu, dev0, dev1 = d.probeFlash()
	try:
		print "Manufacturer: %s" % g_vendors[manu][0]
		f = g_vendors[manu][1][dev1]
	except KeyError:
		print "Unknown Manufacturer"
		print "Got id[3]: %x, %x, %x" % (manu, dev0, dev1)
		try:
			f = g_stm_types[1][dev1]
			print "Compatible  : %s" % f[0]
		except KeyError:
			print "Unable to detect flash, just assuming 2MB size"
			f = ("m25p16", 32, 0x10000)
	bytes = f[1] * f[2]
	print "Size        : %d MB" % (bytes / (1024 * 1024))
	return bytes

if __name__ == "__main__":
	d = dpf.open(sys.argv[1])
	detect_flash(d)
	# detect_lcd(d)
	d.close()

# Script to extract flash
#
# <hackfin@section5.ch>
#
# Argument is either:
#
# /dev/sgX : scsi device (unhacked mode)
# usb0     : developer mode (already hacked)

import sys
sys.path.append("../Debug")
import dpf

import detect

d = dpf.open(sys.argv[1])

size = detect.detect_flash(d)

# Offset, size
print "Reading %x bytes..." % size
buf = d.readFlash(0x00, size)

f = open("full.bin", "wb")
f.write(buf)
print "Wrote full.bin"
f.close()

d.close()


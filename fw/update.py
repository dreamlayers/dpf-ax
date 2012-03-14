import struct
import sys
sys.path.append("./Debug")
import dpf
import time

d = dpf.open("usb0")

offset = 0x380000

print "Writing firmware..."
d.patchSector(0x0, offset, sys.argv[1])

d.close()


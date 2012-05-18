import struct
import sys
sys.path.append("../Debug")
import dpf
import time

SECTOR_SIZE = 0x10000

if len(sys.argv) != 5:
	print "Usage: %s <rgb-file> <width> <height> <device>" % sys.argv[0]
	sys.exit(1)

splashbin = open(sys.argv[1], "r")
splash = splashbin.read()
splashbin.close()

resx = int(sys.argv[2])
resy = int(sys.argv[3])
if resx > 511 or resx & 0x01:
	print "Image width must be < 512 and even!"
	sys.exit(1)
if resy > 511 or resy & 0x01:
	print "Image height must be < 512 and even!"
	sys.exit(1)
if len(splash) != (resx * resy * 2):
	print "Length of imagefile does not match given image with/height!"
	sys.exit(1)
	
print "Sending splashfile %s to flash..." % sys.argv[1]
d = dpf.open(sys.argv[4])
data = d.readFlash(0x00, 0x80)
if data[0x50:0x58] != "20120101" or int(data[0x73:0x75]) < 0x03 or data[0x77] != "1":
	print "Wrong firmware!"
	d.close()
	sys.exit(1)

b = struct.unpack("BBB", data[0x20:0x23])
offset = b[0] + (b[1] << 8) + (b[2] << 16)

l = struct.pack("BBBB", resx & 0xff, (resx >> 8) & 0xff, resy & 0xff, (resy >> 8) & 0xff)
data = l + splash

print "offset: 0x%x" % offset

sectors = (len(data) + SECTOR_SIZE - 1) / SECTOR_SIZE
for i in range(sectors):
        sector = offset + (i * SECTOR_SIZE)
        print "Erasing sector at 0x%06x..." % sector
	d.eraseFlash(sector)

for i in range(sectors):
	so = i * SECTOR_SIZE
	print "Flashing sector at 0x%06x..." % (offset + so)
	d.writeFlash(offset + so, data[so:so + SECTOR_SIZE])

d.close()


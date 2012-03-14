import struct
import sys
sys.path.append("./Debug")
import dpf
import detect
import time

SECTOR_SIZE = 0x10000

def flash_restore(d, data):
	sectors = len(data) / SECTOR_SIZE
	d.eraseFlash() # erase full flash
	# for i in range(sectors):
		# print "Erasing sector %d..." % i
		# d.eraseFlash(i * SECTOR_SIZE)

	for i in range(sectors):
		offset = i * SECTOR_SIZE
		print "Flashing sector %d..." % i
		d.writeFlash(offset, data[offset:offset + SECTOR_SIZE])
	

d = dpf.open("usb0")

try:
	file = sys.argv[1]
except:
	file = "../reverse/silver2/full_image.bin"

f = open(file, "r")
data = f.read()

size = detect.detect_flash(d)
n = len(data)
if (n == size) or sys.argv[-1] == "-f":
	flash_restore(d, data)
	d.run(0x136b) # Call reset
else:
	print "Flash size mismatch, specify -f to force"

d.close()


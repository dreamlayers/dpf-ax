# Compiles banked executable into flash image
# (c) 2010-2011, <hackfin@section5.ch>
#

import intelhex
import struct
import sys

import crc

# Set True if you want lots of output
verbose = False

# Absolute max number of banks can currently be 32
# For more, we have to enhance bankswitch.s
NBANKS   = 32 # max number of banks

F_SPECIAL = 0x80
BOOTSTRAP  = 0x00
XDATA      = 0x10
IGNORE     = 0x30

GROUP_MASK  = 0xe0 # Group mask

class BootHeader:
	def __init__(self, sector_addr, start, size, jmpt_offset, code_offset):
		self.xdata = 0x1040 - 0x800
		self.start = start
		self.jmpt_offset = jmpt_offset
		self.flash_start = code_offset
		self.size = size
		self.crc = 0 # Checksum over boot code
		self.crc2 = 0 # Checksum over header
		self.flags = 0
		self.key = 0xabba
		self.sector = sector_addr
		self.nbanks = NBANKS
		self.table_tag = "ProcTblX" # Do not change length	

	def jmpentry(self, i):
		start, end, offset, mode = i
		start &= 0xffff
		end &= 0xffff
		if mode != XDATA:
			if start >= 0x800:
				if start < 0x1b00:
					start -= 0x800
					end -= 0x800
				else:
					start -= 0x1a00
					end -= 0x1a00
					if (start - end) > 0x100:
						raise ValueError, "XSEG too big!"
			else:
				print start
				raise ValueError, "Relocation address for xram < 0x800!"
		end += 1 # non-inclusive, we add 1
		t = (self.sector + self.flash_start + offset, start, end)

		return t

	def genJumptable(self, index):
		jumpt = self.table_tag

		for i in xrange(self.nbanks):
			try:
				r = index[i]
				offs, start, end = self.jmpentry(r)
				len = end - start
				if verbose:
					t = (i, offs, start, len)
					print "[%d] offset: %06x, start: %04x len 0x%04x" % t
			except KeyError:
				offs, start, end = (0xffffffff, 0xffff, 0xffff)

			jumpt += struct.pack(">HH", start, end)
			jumpt += struct.pack("<I", offs)

		self.jumptable = jumpt

	def genhdr(self):
		a = ""

		a += struct.pack(">HH", self.xdata, self.start)
		v = self.flash_start
		a += struct.pack("BBB", v >> 16, (v >> 8) & 0xff, v & 0xff)
		a += struct.pack(">H", self.size)
		a += struct.pack(">H", self.crc)
		a += struct.pack("B", self.flags)
		a += struct.pack(">H", self.key)

		return a
				
	def gencrc(self, buf):
		# Generate CRC from boot block:
		c = crc.compute(buf[:self.size])
		self.crc = c
		# Now generate CRC for header:
		hdr = self.genhdr()
		c = crc.compute(hdr)
		self.crc2 = c
		
	def dump(self, out):
		a = self.genhdr()
		a += struct.pack(">H", self.crc2)

		a += 14 * '\0'
		a += struct.pack(">H", 0x55aa) # Terminator magic

		out.puts(0x000000, a)
		out.puts(self.jmpt_offset - len(self.table_tag), self.jumptable)


def overlap(x, y):
	if (x[0] >= y[0] and x[0] <= y[1]) or (x[1] >= y[0] and x[1] <= y[1]):
		return 1
	else:
		return 0

def merge_segments(index):
	table = {}
	groups = {}
	
	for i in index.keys():
		bank = i & ~GROUP_MASK
		g = i & GROUP_MASK
		t = index[i]

		a = t[0]
		b = t[1]

		if g & F_SPECIAL:
			bank = g

		if table.has_key(bank):
			s = table[bank]
			if a < s[0]:
				s[0] = a
			if b > s[1]:
				s[1] = b
		else:
			table[bank] = [a, b, 0, 0]

		if g & F_SPECIAL:
			print "Ignoring segment %x in overlap check" % i
			continue

		a &= 0xffff
		b &= 0xffff

		if groups.has_key(g):
			s = groups[g]
			# Merge physical addresses:
			if a < s[0]:
				s[0] = a
			if b > s[1]:
				s[1] = b
			s[2].append(bank)
		else:
			groups[g] = [a, b, [bank]]


	vals = groups.values()
	vals.sort(lambda x, y: cmp(x[0], y[0]))
	for i in range(len(vals) - 1):
		if overlap(vals[i], vals[i+1]):
			print "Module ranges:"
			for i in vals:
				print "0x%x 0x%x, banks: %s" % (i[0], i[1], i[2])
			raise ValueError, "Bank overlap !"


	return table

I = intelhex.IntelHex(sys.argv[1])
out = intelhex.IntelHex()

min = I.minaddr()
max = I.maxaddr()


if verbose:
	print "min: %x, max: %x" % (min, max)


addrs = I.addresses()

index = {}

bank = addrs[0] >> 16
bank &= ~GROUP_MASK
start = addrs[0]
mode = 0
for i in addrs:
	b = (i >> 16)
	if (b & GROUP_MASK) == F_SPECIAL | IGNORE: # Special bank for RAM apps only
		continue
	nmode = b & GROUP_MASK

	if b != bank:
		if index.has_key(bank):
			raise ValueError, "Same bank specified twice!"
		index[bank] = (start, prev, 0, mode)
		#print "%x %x %x %x" % index[bank]
		bank = b
		mode = nmode
		start = i

	prev = i

index[bank] = (start, prev, 0, mode)
#print "%x %x %x %x" % index[bank]

print "----------------"

for i in index.keys():
	t = index[i]
	print "%02x  %06x %06x" % (i, t[0], t[1])

index = merge_segments(index)

buf = ""

# Output bootstrap:

r = index[0x80]
start, end, d, mode = r
print "Bootstrap:"
print hex(start), hex(end)
size = end - start + 1
s = I.tobinstr(start, end)
buf += s
print "offset: %x" % len(buf)

for i in xrange(NBANKS):
	try:
		r = index[i]
		print "mod[%d] %08x %08x mode: %02x" % (i, r[0], r[1], r[3])
		start, end, d, mode = r
		s = I.tobinstr(start, end)
		r[2] = len(buf)
		buf += s
	except KeyError:
		pass

print "-------"

# now build jump table:

sector_addr = int(sys.argv[3], 16)
start = int(sys.argv[4], 16)
jmpt = int(sys.argv[5], 16)
code = int(sys.argv[6], 16)

hdr = BootHeader(sector_addr, start, size, jmpt, code)
hdr.gencrc(buf)
hdr.genJumptable(index)
hdr.dump(out)
out.puts(hdr.flash_start, buf)

out.dump()
outhex = sys.argv[2]
# Output HEX file:
out.tofile(outhex, "hex")

# Output BIN image:
outbin = outhex.split(".")
outbin = ".".join(outbin[:-1]) + ".bin"
sys.stderr.write("Write BIN file '%s'\n" % outbin)
fontbin = open("font4x8.bin", "r")
chartable = fontbin.read()
fontbin.close()
out.puts(0x10000, chartable)
out.tofile(outbin, "bin")


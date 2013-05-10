# DPF dump splitter
# <hackfin@section5.ch>
#
# Takes a binary dump of the DPF and splits it into the banked modules
# Usage:
# 1. First, simply run this script after having dumped your entire DPF
#    into full_image.bin. It will generate a lot of dump* files.
# 2. Then analyze the addresses in dump00.asm. The code is loading more
#    common code segments from other addresses. To extract this code,
#    enhance or create a record in g_dpfs. Later on, we'll add some
#    firmware version detection...
#

import os
import sys
import struct
# import scantool

IMGLEN1 = 128 * 128 / 8
IMGLEN16 = 128 * 128 * 2

# menu table in code memory:
table_addr = 0x1600

g_dpfs = {
	'default' : [ 
		[], [], [], [],
	],
	'silver2' : [ 
		[
			(0x2f0, 0x1934, 0x19f2),
			(0x3ae, 0x1003, 0x1270), 
			(0x6a0, 0x1280, 0x12bc),
		], 
		[],
		[],
		[],
	],
	'black' : [ 
		[
			(0x2e0, 0x1934, 0x19f2),
			(0x39e, 0x1003, 0x1270), 
			(0x682, 0x1280, 0x12d0),
		], 
		[],
		[],
		[],
	],
	'white' : [ 
		[
			(0x2f0, 0x1934, 0x19f2),
			(0x3ae, 0x1003, 0x1270), 
			(0x69c, 0x1280, 0x12d0),
			(0x2fec, 0x1800, 0x189d),
		], 
		[],
		[],
		[],
	],
	'visualland' : [ 
		[
			(0x2c0, 0x1934, 0x19f2),
			(0x37e, 0x1003, 0x1270), 
			(0x662, 0x1280, 0x12d0),
		], 
		[],
		[],
		[],
	],
	'blue' : [ 
		[ # special code segments: fileoffset, start, end
			(0x2c0, 0x1934, 0x19f2),
			(0x37e, 0x1003, 0x1270), 
			(0x662, 0x1280, 0x12d8),
			(0x3474, 0x1800, 0x189d),

			# Dynamic jump table:
			(0x0e54, 0xce7 + 0x800, 0xce7 + 0x800 + 0x2c2), 
		],
		[ # images: name, fileoffset, rel_start, rel_end
			("img0", 0x55bbb, 0x0, IMGLEN16), 
			("img1", 0x4eb89, 0x0, IMGLEN1),
			("usb_conn", 0x4ec89, 0x0, 0x80 * 0x48),
			("menu0", 0x4e289, 0x0, IMGLEN1),
			("menu1", 0x4e389, 0x0, IMGLEN1),
			("menu2", 0x4ee89, 0x0, IMGLEN1),
			("menu3", 0x4e489, 0x0, IMGLEN1),
			("menu4", 0x4e589, 0x0, IMGLEN1),
			("menu5", 0x4e789, 0x0, IMGLEN1),
			("menu7", 0x4e989, 0x0, IMGLEN1),
			("menu8", 0x4ea89, 0x0, IMGLEN1),
			("menu9", 0x4e889, 0x0, IMGLEN1),
			("mainmenu", 0x4c7f9, 0x0, IMGLEN1),
			("mainmenu-ch", 0x4f589, 0x0, IMGLEN1),
			("jpgx", 0x4b744, 0x0, IMGLEN16), # mod13
			("menu0", 0x4daf9, 0x0, IMGLEN1),
		],
		[ # data buffers: fileoffset, start, end
			(0x1b02 + 4, table_addr, table_addr + 0x28a),  # Menu bitmap indices?
			(0x1874 + 4, table_addr, table_addr + 0x28a),  # Menu bitmap indices?
			(0x53557, 0xfd7, 0xfd7 + 64),
		],
		[ # Dynamic applet load: reloc, fileoffset, , size
			(0x14e7, 0x0e54, 15 )
		]
	],
	'silver' : [ 
		[ # special code segments: fileoffset, start, end
			(0x2c0, 0x1934, 0x19f2),
			(0x37e, 0x1003, 0x1270), 
			(0x662, 0x1280, 0x12d8),
		],
		[],
		[],
		[],
	],
	'pink' : [ 
		[ # code segments:
			(0x2f0, 0x1934, 0x19f2),
			(0x3ae, 0x1003, 0x1270), 
			(0x69c, 0x1280, 0x12d1),
			(0x2fc0, 0x1800, 0x189d), # USB main loop, -> mod18_145b

			(0x0e88, 0xce7 + 0x800, 0xce7 + 0x800 + 0x2c2), 
		],
		[ # images
			("img0", 0x5320b, 0x0, IMGLEN16), 
			("img1", 0x54f4b, 0x0, IMGLEN1), 
			("lowpower", 0x51a3b, 0x0, IMGLEN1), 
			("img2", 0x4c244, 0x0, IMGLEN16), 
			("title", 0x4f03b, 0x0, IMGLEN1), 
			("title", 0x4f13b, 0x0, IMGLEN1), 
			("title", 0x4f23b, 0x0, IMGLEN1), 
		],
		# data:
 		[], 
		# dynamic:
		[
			(0x14e7, 0x0e88, 15 )
		]
	],
	'focal' : [ 
		[ # code segments:
			(0x2e0, 0x1934, 0x19f2),
			(0x39e, 0x1003, 0x1279), 
			(0x682, 0x1280, 0x12f3),
			(0x3056, 0x1800, 0x189d), # USB main loop, -> mod17_145b

			(0x0e88, 0xce7 + 0x800, 0xce7 + 0x800 + 0x2c2), 
		],
		[ # images
			("img0", 0x5320b, 0x0, IMGLEN16), 
		],
		# data:
 		[], 
		# dynamic:
		[
			(0x14e7, 0x0e88, 15 )
		]
	],
	'pearl' : [ 
		[ # code segments:
			(0x2e0, 0x1934, 0x19f2),
			(0x39e, 0x1003, 0x1279), 
			(0x682, 0x1280, 0x12f3),
			(0x3056, 0x1800, 0x189d), # USB main loop, -> mod17_145b

			(0x0e88, 0xce7 + 0x800, 0xce7 + 0x800 + 0x2c2), 
		],
		[ # images
			("img0", 0x5320b, 0x0, IMGLEN16), 
		],
		# data:
 		[], 
		# dynamic:
		[
			(0x14e7, 0x0e88, 15 )
		]
	],
	'micca' : [ 
		[ # code segments:
			(0x2e0, 0x1934, 0x19f2),
			(0x39e, 0x1003, 0x1279), 
			(0x682, 0x1280, 0x12f3),
			(0x3056, 0x1800, 0x189d), # USB main loop, -> mod17_145b

			(0x0e88, 0xce7 + 0x800, 0xce7 + 0x800 + 0x2c2), 
		],
		[ # images
			("img0", 0x5320b, 0x0, IMGLEN16), 
		],
		# data:
 		[], 
		# dynamic:
		[
			(0x14e7, 0x0e88, 15 )
		]
	],

}

RET = chr(0x22)
LJMP = chr(0x02)
LCALL = chr(0x12)

bswap = lambda x: ( (x >> 8) & 0xff ) | ((x << 8) & 0xff00)

class ProcEntry:
	format = "<HHI"
	
	def __init__(self):
		self.start = 0
		self.end = 0
		self.offset = 0
		self.size = struct.calcsize(self.format)

	def fromBuffer(self, b):
		start, end, self.offset, = struct.unpack(self.format, b)
		self.start = bswap(start)
		self.end = bswap(end)


	def __repr__(self):
		return "< %04x, %04x, %08x >" % (self.start, self.end, self.offset)

def extract_code(data, fname):
	out = data
	outf = open(fname, "w")
	outf.write(out)
	outf.close()

def write_ctl(name, offset, fileoffset, size = 0):
	outf = open(name, "w")
	outf.write("; control file -- autogenerated\n")
	outf.write("; file offset: %08x\n" % fileoffset)
	outf.write("# %04x\n" % offset)
	outf.write("# %04x dump file offset: %08x\n" % (offset, fileoffset))
	outf.write("o %04x\n" % offset)
	if size:
		outf.write("b %04x-%04x\n" % (offset, offset + size - 1))
	outf.close()

def exists(file):
	try:
		a = os.stat(file)
		return 1
	except:
		return 0

def generate_ctl(which, prefix, record, entries):
	fname = "%s%02d.in" % (prefix, which)
	if exists(fname):
		print "%s exists, not creating" % fname
		return

	outf = open(fname, "w")
	outf.write("; control file for %s%d.bin -- autogenerated\n" % (prefix, which))
	outf.write("; file offset: %08x\n" % int(record.offset))
	offset = 0x800 + record.start
	outf.write("# %04x ----- SNIP : mod%d -----\n" % (offset, which))
	outf.write("# %04x\n" % offset)
	outf.write("# %04x dump file offset: %08x\n" % (offset, int(record.offset)))
	outf.write("o %04x\n" % offset)
	for i in entries:
		outf.write("%s\n" % i)
	outf.close()
	
def build_tab(data, start, end):
	l = []

	offset = start

	i = 0
	
	while 1:
		e = ProcEntry()
		d = data[offset : offset + e.size]
		e.fromBuffer(d)
		if d == "-EndTbl-" or offset > end:
			break
		offset += e.size
		l.append(e)
		i += 1
	
	return l

val = lambda x: ord(x)

# Some codes to scan for:

scan_banksw = [
	0x74, val, 0x90, val, val, val, 0x19, val
]

scan_loadcode = [
	0x90, val, val, 0x7b, val, 0x7c, val, 0x7d, val, 0x7e, val , 0x7f, val
]


# Stolen from scantool:
class Scanner:
	def __init__(self, data, seq):
		self.data = data
		self.args = []
		self.scansequence = seq

	def scan(self, context, cb):
		p = 0
		n = 0
		while p < len(self.data):
			if type(self.scansequence[n]) == type(val):
				args.append(val(data[p]))
				n += 1
			elif chr(self.scansequence[n]) == data[p]:
				n += 1
			else:
				n = 0
				args = []

			if n == len(self.scansequence):
				cb(context, args, p)
				n = 0
				args = []

			p += 1

def make_symentry(context, args, p):
	e, offset = context
	if args[3] in [0x12, 0x02]:
		e.append("x %04x mod%d_%04x" % (offset + p - 4, args[0] + 1, (args[1] << 8) + args[2]))


# TODO: Use scantool stuff
def analyze(data, offset):
	e = []

	scanner = Scanner(data, scan_loadcode)
	scanner.scan((e, offset), make_symentry)

	return e

def make_record(context, args, p):
	e, rec = context
	r = args[0] << 8 | args[1]
	e.append("s %04x code_%04x" % (r, r + 0x800))
	r = args[2] << 8 | args[3]
	s = (args[4] << 16) | (args[5] << 8) | args[6]
	e.append("# %04x load code from %06x to xmem:%04x" % (rec.start + 0x800 + p - 9, s, r))


def analyze_bootblock(data):
	rec = ProcEntry()

	e = []
	s = struct.unpack(">H", data[5:7])[0] # start code in flash
	reloc = struct.unpack(">H", data[2:4])[0] # relocation

	print hex(reloc), hex(s)

	rec.start = reloc - s - 0x800
	rec.offset = s

	offset = reloc - s

	e.append("b %04x-%04x" % (offset, (reloc - 1)))
	e.append("l %04x start" % reloc)
	e.append("l %04x jumptable" % (offset +  0x88))

	# Just copy the scan hack -- no more scantool
	scanner = Scanner(data, scan_loadcode)
	scanner.scan((e, rec), make_record)

	return rec, e


def write_rawimage(d, fname):
	outf = open(fname, "wb")
	outf.write(d)
	outf.close()

try:
	# Choose here which DPF to dump:
	dpf = g_dpfs[sys.argv[1]]
	f = open(sys.argv[2], "r")
except (KeyError, IndexError):
	print "Usage: %s <frameid>" % sys.argv[0]
	print "where frameid is one of:"
	for i in g_dpfs.keys():
		print "  ", i
	print "No DPF specified given, running basic dump"
	dpf = g_dpfs['default']
	f = open("full_image.bin", "r")
	

data = f.read()
f.close()


for i in dpf[0]:
	o, start, end = i
	end += 1
	d = data[o:o + end - start]
	extract_code(d, "code_%04x.bin" % start)
	write_ctl("code_%04x.ctl" % start, start, o)

for i in dpf[1]:
	name, o, start, end = i
	end += 1
	d = data[o:o + end - start]
	write_rawimage(d, "img_%s-%06x.raw" % (name, o))

for i in dpf[2]:
	o, start, end = i
	end += 1
	d = data[o:o + end - start]
	extract_code(d, "data_%04x.bin" % o)
	write_ctl("data_%04x.ctl" % o, start, o, end - start)

for i in dpf[3]:
	codeoffs, foffs, n = i
	c0 = codeoffs
	c1 = codeoffs - foffs
	labels = {}
	for j in range(n):
		sz = [0,0]
		offs = [0, 0]
		o = foffs + j * 8
		d = data[o:o + 8]
		
		sz[0] = ord(d[0]) + (ord(d[1]) << 8)
		offs[0] = foffs + ord(d[2]) + (ord(d[3]) << 8)
		sz[1] = ord(d[4]) + (ord(d[5]) << 8)
		offs[1] = foffs + ord(d[6]) + (ord(d[7]) << 8)

		p = (c0 + j * 8 + 7, offs[0], sz[0], offs[1], sz[1])
		print "! %04x dyn_%04x[%d], dyn_%04x[%d]" % p
		labels[c1 + offs[0]] = "dyn_%04x" % offs[0]
		labels[c1 + offs[1]] = "dyn_%04x" % offs[1]
	for j in labels.keys():
		print "l %04x %s" % (j, labels[j])

format = ">HHBH"
s = struct.unpack(format, data[:7])
imem_loadaddr, code_startaddr, dummy, startcode = s

l = build_tab(data, 0x88, startcode)

print 80 * "-"

start = l[0].offset
d = data[0:start]
extract_code(d, "dump00.bin")
rec, entries = analyze_bootblock(d)
generate_ctl(0, "dump", rec, entries)

index = 1
for i in l:
	size = i.end - i.start
	d = data[i.offset: i.offset + size]
	print "[dump%d] %04x  %04x %08x (%d)" % (index, i.start, i.end, i.offset, size)
	extract_code(d, "dump%02d.bin" % index)
	entries = analyze(d, i.start + 0x800)
	generate_ctl(index, "dump", i, entries)
	index += 1


# No scantool here.
# scantool.guess(scantool.MC8052, data, "dump")



#!/usr/bin/python
#
import struct
import struct
import sys
import time
import binascii
import os

# Nedded for fulldump
sys.path.append("../Debug")
import dpf
import detect

# DPF profiles
import knowntypes



############################################################################

verbose = 0

JUMPTABLE_OFFSET_BUILDWIN = 0x80
JUMPTABLE_OFFSET_COBY = 0x180

jumptable_offset = 0

bswap = lambda x: ( (x >> 8) & 0xff ) | ((x << 8) & 0xff00)

def find_dpf_by_version_string(buf):
	version = (buf[0x50:0x58], buf[0x60: 0x70], buf[jumptable_offset:jumptable_offset+8])
	for i in knowntypes.KNOWN_TYPES:
		if 'Version' in i:
			v = i['Version']
			if v[0] == version[0] and v[1] == version[1] and v[2] == version[2]:
				return i
	return None

def get_module(buf, n):
	global jumptable_offset
	n *= 8
	n += jumptable_offset
	start, end, flashaddr = struct.unpack("<HHI", buf[n:n+8])

	start = bswap(start)
	end = bswap(end)

	return start, end, flashaddr

def isBuildwinFw(buf):
	global jumptable_offset
	version = (buf[0x50:0x58], buf[0x60: 0x70], buf[JUMPTABLE_OFFSET_BUILDWIN:JUMPTABLE_OFFSET_BUILDWIN+8], buf[JUMPTABLE_OFFSET_COBY:JUMPTABLE_OFFSET_COBY+8])
	if version[0] == "20120101":
		return -1
	if version[0].isdigit():
		if version[1][3] == " " and version[1][6] == " " and version[1][7:10].isdigit():
			if version[2].startswith("ProcTbl"):
				jumptable_offset = JUMPTABLE_OFFSET_BUILDWIN
			elif version[3].startswith("ProcTbl"):
				jumptable_offset = JUMPTABLE_OFFSET_COBY
			if jumptable_offset != 0:
				i = 1
				p = jumptable_offset + 8

				while buf[p:p+8] != "-EndTbl-" and i < 60:
					p += 8
					i += 1
				if i > 10 and i < 60:
					return i
	return 0

val = lambda x: ord(x)

scan_locate_displaysize = [
0xE4, 0xF5, val, 0xF5, val, 0xF5, val, 0xF5, val,
0x75, val, val, 0x75, val, val, 0x75, val, val, 0x75, val, val, 
0x12, val, val,
0xE4, 0xFB, 0xFC, 0xE4, 0xFD, 0xFE, 0xE5
]
scan_locate_lcdinit = [
0x75, 0xEC, 0x00, 0x53, 0xEA, 0xE8,
]
scan_locate_stdlcdinit = [
0x75, 0xEC, 0x00, 0x53, 0xEA, 0xE8, 0x53, 0xEB, 0xFD, 0x53, 0xA0, 0xFD,
0x12, val, val, 0x12, val, val, 0x12, val, val, 0x12, val, val, 0x12, val, val,
0x53, 0x90, 0xFE,
0x12, val, val, 0x12, val, val, 0x12, val, val, 0x12, val, val, 0x12, val, val, 0x12, val, val, 0x12, val, val, 0x12, val, val,
0x43, 0x90, 0x01, 0x7F
]
scan_locate_initbl = [
0x70, 0x01, 0x22, 0x90, val, val, 0xC0, 0xF0, 0xF5, 0xF0, 0xE4, 0x93, 0xC3, 0x25, 0x82, 0xC5, 0x82
]
scan_locate_openwin = [
0x90, 0x0a, 0x80, 0x7b, val, 0x7c, val, 0x7d, 0x00, 0x7e, val, 0x7f, val, 0x12
]
scan_locate_std_Lcd_Contrast = [
0xeb, 0x78, 0xc4, 0xf6, 0x7f, 0x01, 0x74, val, 0x90, val, val, 0x12
]
scan_locate_Lcd_Contrast = [
0x78, 0xc5, 0xe6, 0xfb, 0x12, val, val, 0x78, 0xc4, 0xe6, 0xfb, 0x12, val, val
]
scan_locate_contrast_backlight_default = [
0x78, 0xc4, 0x76, val, 0x08, 0x76, val, 0x08, 0x76
]

# Stolen from dump.py, stolen from scantool:
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
				self.args.append(val(self.data[p]))
				n += 1
			elif chr(self.scansequence[n]) == self.data[p]:
				n += 1
			else:
				n = 0
				self.args = []

			if n == len(self.scansequence):
				cb(context, self.args, p - len(self.scansequence) + 1)
				n = 0
				self.args = []

			p += 1


def add_offs(context, args, p):
	e, offset = context
	e.append(p)
	for i in range(len(args) / 2):
		off = args[i*2] << 8 | args[i*2 + 1]
		e.append(off)

def add_vals(context, args, p):
	e, offset = context
	e.append(p)
	for i in range(len(args)):
		e.append(args[i])

def dump_tables(tbls):
	outf = open("identify.out/lcdinitbl_tmp.txt", "w")
	for tbl in tbls:
		outf.write(tbl[0])
		outf.write("::")
		for  i in range(0, len(tbl[1])):
			if (i % 10) == 0:
				outf.write("\n  .db  ")
			else:
				outf.write(", ")
			outf.write("0x%02x" % struct.unpack("B", tbl[1][i])[0])
		outf.write("\n%s_len::  .db  %d" % (tbl[0], len(tbl[1])))
		outf.write("\n\n")
	outf.close()


def find_setcontrast(buf, module):
	start, end, flashaddr = get_module(buf, module)
	l = end - start
	start += 0x800
	data = buf[flashaddr : flashaddr + l]
	# look for custom 
	contrastoffs = []
	scanner = Scanner(data, scan_locate_Lcd_Contrast)
	scanner.scan((contrastoffs, start), add_offs)
	if len(contrastoffs) == 3:
	    p = contrastoffs[2] - start
	    print "Module %d:" % module
	    lcdcontrast_tmp = data[p:p+0x400]
	    # check for standard Lcd_Contrast
	    contrastoffs = []
	    scanner = Scanner(lcdcontrast_tmp, scan_locate_std_Lcd_Contrast)
	    scanner.scan((contrastoffs, 0), add_vals)
	    if len(contrastoffs) == 4:
	        print "LCD_Contrast (Tbl) at 0x%04x (0x%06x)" % (p + start, p + flashaddr)
	        return True
	    # save non-standard Lcd_Contrast
            print "LCD_Contrast !NOTBL! at 0x%04x (0x%06x)" % (p + start, p + flashaddr)
	    outf = open("identify.out/setcontrast_tmp.bin", "wb")
	    outf.write(lcdcontrast_tmp)
	    outf.close()
	    outf = open("identify.out/setcontrast_tmp.sh", "w")
	    outf.write("#!/bin/sh\n#\ncp ../identify_d52.in setcontrast_tmp.ctl\nd52 -p -b -n -d setcontrast_tmp.bin x%04x\n" % (p + start))
	    outf.close()
	    print "Written to identify.out/setcontrast_tmp.bin."
	    return True

	return False

lcdinit_found = False

def find_initbl(buf, module):
	global lcdinit_found
	tbls = []
	start, end, flashaddr = get_module(buf, module)
	l = end - start
	start += 0x800
	data = buf[flashaddr : flashaddr + l]

	crc_init = 0

	initbloffs = []
	scanner = Scanner(data, scan_locate_stdlcdinit)
	scanner.scan((initbloffs, start), add_offs)
	if len(initbloffs) > 0:
		if verbose:
			p = initbloffs[0]
			print
			print "Module %d:" % module
			print "LcdInit (Tbl)   at 0x%04x (0x%06x)" % (p + start, p + flashaddr)
		lcdinit_found = True
	else:
		initbloffs = []
		scanner = Scanner(data, scan_locate_lcdinit)
		scanner.scan((initbloffs, start), add_offs)
		if len(initbloffs) > 0:
			p = initbloffs[0]
			lcdinit_found = True
			crc_init = binascii.crc32(data[p:p+100]) & 0xffffffff
			if verbose:
				print
				print "Module %d:" % module
				print "LcdInit !NOTBL! at 0x%04x (0x%06x), CRC = 0x%x" % (p + start, p + flashaddr, crc_init)
				if verbose > 1:
					lcdinit_tmp = data[p:p+0x800]
					outf = open("identify.out/lcdinit_tmp.bin", "wb")
					outf.write(lcdinit_tmp)
					outf.close()
					outf = open("identify.out/lcdinit_tmp.sh", "w")
					outf.write("#!/bin/sh\n#\ncp ../identify_d52.in lcdinit_tmp.ctl\nd52 -p -b -n lcdinit_tmp.bin x%04x\n" % (p + start))
					outf.close()
					print "Written to identify.out/lcdinit_tmp.bin."
		
	crc_tbl = 0

	initbloffs = []
	scanner = Scanner(data, scan_locate_initbl)
	scanner.scan((initbloffs, start), add_offs)
	if len(initbloffs) == 2:
	    try:
		p = initbloffs[1] - start
		i = struct.unpack("B", data[p])[0]
		if verbose:
			if not lcdinit_found:
				print
			print "Module %d:" % module
			print "LcdScheduleTbl  at 0x%04x (0x%06x), len 0x%02x" % (p + start, p + flashaddr, i)
			#tbls.append(["_custom_scheduletbl", data[p+1:p+i+1]])
		p += i + 1
		i = struct.unpack("B", data[p])[0]
		lcdcontrasttbl_paracount = struct.unpack("B", data[p+1])[0]
		if verbose:
			print "LcdContrastTbl  at 0x%04x (0x%06x), len 0x%02x" % (p + start, p + flashaddr, i)
			l = struct.unpack("B", data[p+2])[0]
			tbls.append(["_custom_contrasttbl", data[p+3:p+l+3]])
		p += i + 1
		i = struct.unpack("B", data[p])[0]
		if verbose:
			print "LcdContrastTb2  at 0x%04x (0x%06x), len 0x%02x" % (p + start, p + flashaddr, i)
			ctbl2_offs = []
			for j in range(lcdcontrasttbl_paracount):
				ctbl2_offs.append(struct.pack("B", struct.unpack("<H", data[p+j+1 : p+j+3])[0] - p - start - 2))
			ctbl2 = data[p+len(ctbl2_offs)*2+2:p+i+1];
			ctbl2 += '\xFF';
			tbls.append(["_custom_contrasttbl2", ctbl2])
			tbls.append(["_custom_contrasttbl2_offsets", ctbl2_offs])
		p += i + 1
		i = struct.unpack("B", data[p])[0]
		if verbose:
			  print "LcdBacklightTbl at 0x%04x (0x%06x), len 0x%02x" % (p + start, p + flashaddr, i)
			  tbls.append(["_custom_backlighttbl", data[p+1:p+i+1]])
		p += i + 1
		j = i
		i = struct.unpack("B", data[p])[0]
		if (i == j):   		# seems to have two LcdBackLightTbls...
			if verbose:
				print "LcdBacklightTb2 at 0x%04x (0x%06x), len 0x%02x" % (p + start, p + flashaddr, i)
				tbls.append(["_custom_backlighttbl2", data[p+1:p+i+1]])
			p += i + 1
		else:
			tbls.append(["_custom_backlighttbl2", ""])

		l = struct.unpack("<H", data[p : p+2])[0]
		p += 2
		if (p + l) < len(data):
		    lcdinitbl = data[p:p+l]
		    if verbose:
			print "LcdIniTbl       at 0x%04x (0x%06x), len 0x%02x," % (p + start, p + flashaddr, l),
			tbls.append(["_custom_initseq", data[p:p+l] + "\xFF"])
		    crc_tbl = binascii.crc32(lcdinitbl) & 0xffffffff
		    if verbose:
				print "CRC = 0x%x" % crc_tbl
			        if verbose > 1:
				    dump_tables(tbls)
				    print "Written to identify.out/lcdinitbl_tmp.txt."
	    except:
		if verbose:
			print "Invalid LcdIniTbl!"

	return crc_tbl, crc_init

def find_openwin(buf):
	openwinoffs = []
	c = 0
	b = buf[:7]
	xmem, code, dummy, offset = struct.unpack(">HHBH", b)
	scanner = Scanner(buf[offset:0x10000], scan_locate_openwin)
	scanner.scan((openwinoffs, offset), add_offs)
	if len(openwinoffs) == 3:
		openwin_found = True
		p = openwinoffs[2]
		l = openwinoffs[1] - 0xa80
		if verbose:
			print
			print "OpenWin        at 0x1280 (0x%06x), len 0x%02x," %  (p, l),
		openwin = buf[p:p+l]
		if verbose > 1:
			outf = open("identify.out/openwin_tmp.bin", "wb")
			outf.write(openwin)
			outf.close()
			outf = open("identify.out/openwin_tmp.sh", "w")
			outf.write("#!/bin/sh\n#\nif [ ! -e openwin_tmp.ctl ] ; then\n  cp ../identify_d52.in openwin_tmp.ctl\nfi\nd52 -p -b -n openwin_tmp.bin x1280\n")
			outf.write("echo 'If openwin_tmp.d52 contains the string $$$_0x77_$$$, edit openwin_tmp.ctl and try again!'\necho")
			outf.close()
		c = binascii.crc32(openwin) & 0xffffffff
		if verbose:
			print "CRC = 0x%x" % c
			if verbose > 1:
				print "Written to identify.out/openwin_tmp.bin."
	else:
		c = 0
		if len(openwinoffs) > 3:
			print "More than one match!"
			if verbose:
				for i in range(len(openwinoffs) / 3):
					print "Match %d at flashoffset 0x%06x, adr 0x%06x len 0x%04x" % (i, openwinoffs[i*3], openwinoffs[i*3 + 2], (openwinoffs[i*3 + 1]) - 0xa80)
		else:
			print "Not found."

	return c

def match_crc(crc, checksums):
	#if crc != 0:
	j = 0
	try:
		j = checksums.index(crc)
	except ValueError:
		j = -1
	if j != -1:
		return True
	return False

def recognize_dpf(dump):
	global jumptable_offset
	partial_match = []

	# Check if its a buildwin fw
	print "Looking for firmware.............:",
	num_modules = isBuildwinFw(dump)
	if num_modules > 0:
		w, h = struct.unpack("<HH", dump[0x23:0x27])
		mfg = "buildwin"
		if jumptable_offset == JUMPTABLE_OFFSET_COBY:
			mfg = "coby"
		print "Found (%s, %dx%d px)." % (mfg, w, h)
	else:
		print "Not found."
		print
		if num_modules == -1:
			print "This is already a custom firmware!"
		else:
			print "This in no known firmware!"
		return None, partial_match

	# Try to find OpenWin and LcdIniTbl
	print "Looking for Openwin..............:",
	crc_openwin = find_openwin(dump)
	if crc_openwin == 0:
		print "WARNING: This is no \"standard\" buildwin software!"
	elif not verbose:
		print "Found."

	print "Looking for LcdIniTbl............:",
	initbl_count = 0
	crc_initbl = 0
	crc_init = 0
	for i in range(1, num_modules):
		ct, ci = find_initbl(dump, i)
		if ct != 0:
			crc_initbl = ct
			initbl_count += 1
		if ci != 0:
			crc_init = ci

	if crc_initbl == 0:
		crc_initbl = crc_init
	if initbl_count == 0:
		print "None."
	elif initbl_count > 1:
		print "%d matches." % initbl_count
	elif not verbose:
		print "Found."
	if not crc_initbl:
		print "WARNING: This is no \"standard\" buildwin software!"

        # some additional infos (for development only)
        default_contrast = 0
        default_backlight = 0
	if verbose > 1:
	    print "Looking for lcd_contrast.........:"
	    setcontrast_count = 0
	    for i in range(1, num_modules):
		c = find_setcontrast(dump, i)
	        if c == True:
		    setcontrast_count += 1
	    if setcontrast_count == 0:
		print "WARNING: This fw has no detectable Lcd_Contrast routine!"
	    elif setcontrast_count > 1:
		print "WARNING: %d Lcd_Contrast matches, using last one!" % setcontrast_count

	    defaultcb = []    
	    scanner = Scanner(dump[0:0x10000], scan_locate_contrast_backlight_default)
	    scanner.scan((defaultcb, 0), add_vals)
	    if len(defaultcb) == 3:
	        default_contrast = defaultcb[1]
		default_backlight = defaultcb[2]

	if verbose > 1:
		print
		print "CRCs (OpenWin, Init)  = 0x%x, 0x%x" % (crc_openwin, crc_initbl)
		if default_contrast > 0:
	            print "Default contrast  = %d" % default_contrast
		    print "Default backlight = %d" % default_backlight
	        else:
		    print "Default contrast/backlight init not found!"
		print

	# Now look for display known by their signatur of LcdIniTbl and OpenWin (new way)
	print "Looking for known signatures.....:",
	if verbose:
		print
	for i in knowntypes.KNOWN_TYPES:
		if 'CRC' in i:
			record = i['CRC']
			for r in record:
				if crc_openwin == r[0] and crc_initbl == r[1]:
					if not verbose:
						print "Found."
					return i, None
			if i['Status'] == 'supported':
				for r in record:
					if crc_openwin == r[0]:
						partial_match.append(i['Type'])
						if verbose:
							print "OpenWin:   match at model %s" % i['Type']
					if crc_initbl == r[1]:
						partial_match.append(i['Type'])
						if verbose:
							print "LcdIniTbl: match at model %s" % i['Type']

	print "None."

	# Fallback; look for display known by their fw and build-date (old way)
	print "Looking for known version info...:",
	dpf = find_dpf_by_version_string(dump)
	if dpf:
		print "Found."
		return dpf, partial_match
	print "None."

	return None, partial_match
	
#
#
# MAIN

argerr = False

if len(sys.argv) == 2:
	arg = sys.argv[1]
elif len(sys.argv) == 3:
	if sys.argv[1] == "-v":
		verbose = 1
	elif sys.argv[1] == "-d":
		verbose = 2
	else:
		argerr = True
	if argerr == False:
		arg = sys.argv[2]
else:
	argerr = True
	
if argerr == True:
	print "Usage: %s [option] dumpfile|dpf-devicenode\n  -v   show more infos\n  -d  build developer support files in identify.out" % sys.argv[0]
	sys.exit(-1)

# Make dir for writing temp code
if not os.path.exists("identify.out"):
	os.makedirs("identify.out")
else:
	os.system("rm -f identify.out/*")

dumpfile = arg
if arg.startswith("/dev/"):
#
# copy of fulldump.py
#
	print "Detecting & reading dpf flash..."
	d = dpf.open(arg)
	size = detect.detect_flash(d)
	# Offset, size
	print "Reading %x bytes..." % size
	buf = d.readFlash(0x00, size)
	f = open("identify.out/full.bin", "wb")
	f.write(buf)
	f.close()
	d.close()
	print "Flash written to file 'identify.out/full.bin'."
	print
	dumpfile = "identify.out/full.bin"
	
f = open(dumpfile, "rb")
data = f.read()
f.close()
dpf, pm = recognize_dpf(data)

print
    
if dpf:
	if dpf['Status'] == "supported":
		print "Your dpf is compatible with model %s." % dpf['Type']
	elif dpf['Status'] == "unsupported":
		print "Your dpf is compatible with unsupported model %s." % dpf['Type']
		print "Sorry, this dpf in NOT supported by dpf-ax."
	elif dpf['Status'] == "wip":
		print "Your dpf is compatible with currently unsupported model %s." % dpf['Type']
		print "Sorry, this dpf in NOT supported by dpf-ax at the moment."
		print "Work in progress. Check back later."
else:
	print "Sorry, no matching dpf found."
	if len(pm) > 0:
		print "But following models have a partial signature match:",
		print pm
print

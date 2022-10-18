#!/usr/bin/python
#

import struct
import sys
import time
import binascii
import os
import datetime

if os.name == 'posix':
    # Nedded for fulldump
    sys.path.append("../Debug")
    import dpf
    import detect
else:
    from readflash_win import read_flash_win

# DPF profiles
import knowntypes



############################################################################

verbose = 0

JUMPTABLE_OFFSET_BUILDWIN = 0x80
JUMPTABLE_OFFSET_COBY = 0x180
JUMPTABLE_OFFSET_COBY2 = 0x100

SCAN_NOTFOUND = 0
SCAN_TABLE = 1
SCAN_CUSTOM = 2
SCAN_VALUE = 3

jumptable_offset = 0

bswap = lambda x: ( (x >> 8) & 0xff ) | ((x << 8) & 0xff00)

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
        if version[0] == b"20120101":
                return -1
        if not version[0].isdigit():
                version = (buf[0x60:0x68], buf[0x80: 0x90], buf[JUMPTABLE_OFFSET_BUILDWIN:JUMPTABLE_OFFSET_BUILDWIN+8], buf[JUMPTABLE_OFFSET_COBY:JUMPTABLE_OFFSET_COBY+8], buf[JUMPTABLE_OFFSET_COBY2:JUMPTABLE_OFFSET_COBY2+8])
        if version[0].isdigit():
                if version[1][3] == 32 and version[1][6] == 32 and version[1][7:10].isdigit():
                        if version[2].startswith(b"ProcTbl"):
                                jumptable_offset = JUMPTABLE_OFFSET_BUILDWIN
                        elif version[3].startswith(b"ProcTbl"):
                                jumptable_offset = JUMPTABLE_OFFSET_COBY
                        elif version[4].startswith(b"ProcTbl"):
                                jumptable_offset = JUMPTABLE_OFFSET_COBY2
                        if jumptable_offset != 0:
                                i = 1
                                p = jumptable_offset + 8

                                while buf[p:p+8] != b"-EndTbl-" and i < 100:
                                        p += 8
                                        i += 1
                                if i > 10 and i < 100:
                                        return i
        return 0

val = lambda x: x

scan_locate_displaysize = [
0xE4, 0xF5, val, 0xF5, val, 0xF5, val, 0xF5, val,
0x75, val, val, 0x75, val, val, 0x75, val, val, 0x75, val, val, 
0x12, val, val,
0xE4, 0xFB, 0xFC, 0xE4, 0xFD, 0xFE, 0xE5
]
scan_locate_lcdinit = [
[ 0x75, 0xEC, 0x00, 0x53, 0xEA, 0xE8  ],
# ?? Init leadin for x4241 ??
[ 0xd2, 0x90, 0x53, 0xeb, 0xfd, 0x53, 0xea, 0xfe ]
]

scan_locate_stdlcdinit = [
[
0x75, 0xEC, 0x00, 0x53, 0xEA, 0xE8, 0x53, 0xEB, 0xFD, 0x53, 0xA0, 0xFD,
0x12, val, val, 0x12, val, val, 0x12, val, val, 0x12, val, val, 0x12, val, val,
0x53, 0x90, 0xFE,
0x12, val, val, 0x12, val, val, 0x12, val, val, 0x12, val, val, 0x12, val, val, 0x12, val, val, 0x12, val, val, 0x12, val, val,
0x43, 0x90, 0x01, 0x7F
],
[
0x75, 0xEC, 0x00, 0x53, 0xEA, 0xE8, 0x53, 0xEB, 0xFD, 0x43, 0xbb, 0x20, 0xc2, 0xa1, 0xc2, 0x90,
0x74, val, 0x12, val, val, 0xd2, 0x90, 0x74, val, 0x12, val, val, 0x7f
],
[
0x75, 0xEC, 0x00, 0x53, 0xEA, 0xE8, 0x53, 0xEB, 0xFD, 0xc2, 0xa1, 0xc2, 0x90,
0x74, val, 0x12, val, val, 0xd2, 0x90, 0x74, val, 0x12, val, val, 0x7f
]
]

 
scan_locate_initbl = [
0x70, 0x01, 0x22, 0x90, val, val, 0xC0, 0xF0, 0xF5, 0xF0, 0xE4, 0x93, 0xC3, 0x25, 0x82, 0xC5, 0x82
]
scan_locate_tmr1init = [
0x75, 0xe4, val, 0x75, 0xe5, val
]

scan_locate_tmr1coninit_p23 = [
0x75, 0xe1, 0x43
]

scan_locate_tmr1coninit_p40 = [
0x75, 0xe1, 0x83
]

scan_locate_openwin = [
0x90, 0x0a, 0x80, 0x7b, val, 0x7c, val, 0x7d, 0x00, 0x7e, val, 0x7f, val, 0x12
]
scan_locate_std_Lcd_Contrast = [
0xeb, 0x78, 0xc4, 0xf6, 0x7f, 0x01, 0x74, val, 0x90, val, val, 0x12
]
scan_locate_std_BacklightFun1 = [
0xeb, 0x78, 0xc5, 0xf6, 0x7f, 0x01, 0x74, val, 0x90, val, val, 0x12
]
scan_locate_std_BacklightFun2 = [
0xeb, 0x12, val, val, 0x78, 0xc5, 0xf6, 0x7f, 0x01, 0x74, val, 0x90, val, val, 0x12
]
scan_locate_Lcd_Backlight = [
0x78, 0xc5, 0xe6, 0xfb, 0x12, val, val
]
scan_locate_Lcd_Contrast = [
0x78, 0xc5, 0xe6, 0xfb, 0x12, val, val, 0x78, 0xc4, 0xe6, 0xfb, 0x12, val, val
]
scan_locate_contrast_backlight_default = [
0x78, 0xc4, 0x76, val, 0x08, 0x76, val, 0x08, 0x76
]
scan_locate_Power_On_Init1 = [
#                                 Port_init       Timer0_Init     ADC_Batt_Ini    Lcd_init        ClrScr
0xc2, 0xbf, 0x75, val, val, 0x12, val, val, 0x12, val, val, 0x12, val, val, 0x12, val, val, 0x12, val, val
]
scan_locate_Power_On_Init2 = [
0xc2, 0xbf, 0x75, val, val, 0x75, val, val, 0x12, val, val, 0x12, val, val, 0x12, val, val, 0x12, val, val, 0x12, val, val
]
scan_locate_Power_On_Init3 = [
0xc2, 0xbf, 0x75
]
scan_locate_Power_On_Init_backlight_high = [
#                                                             Port_init       Timer0_Init     ADC_Batt_Ini    Lcd_init        ClrScr    orl p2,#0x08      anl p2dir,#0xf7   
0xc2, 0xbf, 0x75, val, val, 0x75, val, val, 0xc2, 0x14, 0x12, val, val, 0x12, val, val, 0x12, val, val, 0x12, val, val, 0x12, val, val, 0x43, 0xa0, 0x08, 0x53, 0xeb, 0xf7
]
scan_locate_Power_On_Init_backlight_low = [
#                                                             Port_init       Timer0_Init     ADC_Batt_Ini    Lcd_init        ClrScr    anl p2,#0xf7      anl p2dir,#0xf7   
0xc2, 0xbf, 0x75, val, val, 0x75, val, val, 0xc2, 0x14, 0x12, val, val, 0x12, val, val, 0x12, val, val, 0x12, val, val, 0x12, val, val, 0x53, 0xa0, 0xf7, 0x53, 0xeb, 0xf7
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
                        elif self.scansequence[n] == self.data[p]:
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
        for i in range(int(len(args) / 2)):
                off = args[i*2] << 8 | args[i*2 + 1]
                e.append(off)

def add_vals(context, args, p):
        e, offset = context
        e.append(p)
        for i in range(len(args)):
                e.append(args[i])

def dump_tables(tbls):
        outf = open("lcd.tmp/tmp_initbl.txt", "w")
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

def find_tmr1init(data, start, s):
        tmr1offs = []
        scanner = Scanner(data, scan_locate_tmr1init)
        scanner.scan((tmr1offs, start), add_offs)
        per = 0
        if len(tmr1offs) == 2:
            per = ((tmr1offs[1] & 0xFF)) << 8 | (tmr1offs[1] >> 8)
        if verbose:
            if per != 0:
                print("Timer1 %s init found (tmr1per = 0x%04x)." % (s, per))
            else:
                print("Timer1 init NOT found.")
        return per

tmr1per_custom = 0
tmr1con = 0
def find_setbacklight_and_contrast(buf, module):
        start, end, flashaddr = get_module(buf, module)
        l = end - start
        start += 0x800
        data = buf[flashaddr : flashaddr + l]

        # look for backlight init
        bcoffs = []
        scanner = Scanner(data, scan_locate_Lcd_Backlight)
        scanner.scan((bcoffs, start), add_offs)

        hasBacklightFun = SCAN_NOTFOUND
        if len(bcoffs) == 2:
            if verbose:
                print("\nModule %d:" % module)
            # check for standard BacklightFun
            p = bcoffs[1] - start
            lcdbf_tmp = data[p:p+0x400]
            bfoffs = []
            scanner = Scanner(lcdbf_tmp, scan_locate_std_BacklightFun1)
            scanner.scan((bfoffs, 0), add_vals)
            if (len(bfoffs) == 4):
                hasBacklightFun = SCAN_TABLE
            else:
                bfoffs = []
                scanner = Scanner(lcdbf_tmp, scan_locate_std_BacklightFun2)
                scanner.scan((bfoffs, 0), add_vals)
                if len(bfoffs) == 6:
                    hasBacklightFun = SCAN_TABLE
            if hasBacklightFun == SCAN_TABLE:
                if verbose:
                    print("LCD_Backlight (Tbl) at 0x%04x (0x%06x)" % (p + start, p + flashaddr))
            else:
                if verbose:
                    print("LCD_Backlight !NOTBL! at 0x%04x (0x%06x)" % (p + start, p + flashaddr))
                if verbose > 1:
                    # save non-standard BacklightFun
                    outf = open("lcd.tmp/tmp_setbacklight.bin", "wb")
                    outf.write(lcdbf_tmp)
                    outf.close()
                    outf = open("lcd.tmp/disassemble.sh", "a")
                    outf.write("#!/bin/sh\n#\ncp ../identify_d52.in tmp_setbacklight.ctl\nd52 -p -b -n -d tmp_setbacklight.bin x%04x\n" % (p + start))
                    outf.close()
                    print("Written to lcd.tmp/tmp_setbacklight.bin.")
                # Find the timer1 init values
                global tmr1per_custom
                tmr1per_custom = find_tmr1init(data, start, "custom")
                
                hasBacklightFun = SCAN_CUSTOM
                
            # look for timer1 control init
            global tmr1con

            bcoffs = []
            scanner = Scanner(data, scan_locate_tmr1coninit_p23)
            scanner.scan((bcoffs, start), add_vals)
            if len(bcoffs) == 1:
                tmr1con = 0x43
            else:
                bcoffs = []
                scanner = Scanner(data, scan_locate_tmr1coninit_p40)
                scanner.scan((bcoffs, start), add_vals)
                if len(bcoffs) == 1:
                    tmr1con = 0x83
            if verbose:
                if tmr1con != 0:
                    p = bcoffs[0]
                    print("Tmr1con init found at 0x%04x (0x%06x), value = 0x%02x" % (p + start, p + flashaddr, tmr1con))
                else:
                    print("Tmr1con init NOT found")
            
            # look for contrast init
            bcoffs = []
            scanner = Scanner(data, scan_locate_Lcd_Contrast)
            scanner.scan((bcoffs, start), add_offs)
            if len(bcoffs) == 3:
                # check for standard Lcd_Contrast
                p = bcoffs[2] - start
                lcdcontrast_tmp = data[p:p+0x400]
                contrastoffs = []
                scanner = Scanner(lcdcontrast_tmp, scan_locate_std_Lcd_Contrast)
                scanner.scan((contrastoffs, 0), add_vals)
                if len(contrastoffs) == 4:
                    if verbose:
                        print("LCD_Contrast (Tbl) at 0x%04x (0x%06x)" % (p + start, p + flashaddr))
                    return SCAN_TABLE, hasBacklightFun, False
                if verbose:
                    print("LCD_Contrast !NOTBL! at 0x%04x (0x%06x)" % (p + start, p + flashaddr))
                if verbose > 1:
                    # save non-standard Lcd_Contrast
                    outf = open("lcd.tmp/tmp_setcontrast.bin", "wb")
                    outf.write(lcdcontrast_tmp)
                    outf.close()
                    outf = open("lcd.tmp/disassemble.sh", "a")
                    outf.write("#!/bin/sh\n#\ncp ../identify_d52.in tmp_setcontrast.ctl\nd52 -p -b -n -d tmp_setcontrast.bin x%04x\n" % (p + start))
                    outf.close()
                    print("Written to lcd.tmp/tmp_setcontrast.bin.")
                return SCAN_CUSTOM, hasBacklightFun, False

        foundInit = False
        if verbose > 1:
            # No detectable standard backlight/contrast setting found
            # Try to find standard Power_On_Init
            # Only needed for development
            bcoffs = []
            scanner = Scanner(data, scan_locate_Power_On_Init1)
            scanner.scan((bcoffs, start), add_offs)
            if len(bcoffs) == 7:
                foundInit = True
            else:
                bcoffs = []
                scanner = Scanner(data, scan_locate_Power_On_Init2)
                scanner.scan((bcoffs, start), add_offs)
                if len(bcoffs) == 8:
                    foundInit = True
                else:
                    bcoffs = []
                    scanner = Scanner(data, scan_locate_Power_On_Init3)
                    scanner.scan((bcoffs, start), add_offs)
                    if len(bcoffs) == 1:
                        foundInit = True
            if foundInit == True:
                print("Module %d:" % module)
                p = bcoffs[0]
                print("Power_On_Init at 0x%04x (0x%06x)" % (p + start, p + flashaddr))
                init_tmp = data[p:p+0x400]
                # save Power_On_Init for further inspection
                outf = open("lcd.tmp/tmp_poweroninit.bin", "wb")
                outf.write(init_tmp)
                outf.close()
                outf = open("lcd.tmp/disassemble.sh", "a")
                outf.write("#!/bin/sh\n#\ncp ../identify_d52.in tmp_poweroninit.ctl\nd52 -p -b -n -d tmp_poweroninit.bin x%04x\n" % (p + start))
                outf.close()
                print("Written to lcd.tmp/tmp_poweroninit.bin.")
        
        return SCAN_NOTFOUND, hasBacklightFun, foundInit


lcdinit_type = SCAN_NOTFOUND
tmr1per = 0
contrast_table_len = 0
backlight_table2 = False

TABLE_CONTRAST = 1
TABLE_BACKLIGHT = 2
TABLE_INIT = 3

#init_tables = []

def find_initbl(buf, module):
        global lcdinit_type
        global init_tables
        init_tables = []
        start, end, flashaddr = get_module(buf, module)
        l = end - start
        start += 0x800
        data = buf[flashaddr : flashaddr + l]

        # Try to find standard Power_On_Init with a call to Lcd_init
        bcoffs = []
        foundInit = False
        foundStdInit = False
        scanner = Scanner(data, scan_locate_Power_On_Init1)
        scanner.scan((bcoffs, start), add_offs)
        if len(bcoffs) == 7:
            foundInit = True
            p = bcoffs[5] - start
        else:
            bcoffs = []
            scanner = Scanner(data, scan_locate_Power_On_Init2)
            scanner.scan((bcoffs, start), add_offs)
            if len(bcoffs) == 8:
                foundInit = True
                p = bcoffs[6] - start

        if foundInit == True:
            for t in scan_locate_stdlcdinit:
                foundStdInit = True
                p1 = p
                for c in t:
                    if type(c) != type(val):
                        if c != struct.unpack("B", data[p1:p1+1])[0]:
                            foundStdInit = False
                            break;
                    p1 += 1
                if foundStdInit == True:
                    break;

        crc_init = 0
        found = False
        for t in scan_locate_stdlcdinit:
            initbloffs = []
            if foundStdInit == True:
                initbloffs.append(p)
            elif foundInit == False:
                scanner = Scanner(data, t)
                scanner.scan((initbloffs, start), add_offs)
            if len(initbloffs) > 0:
                if verbose:
                        p = initbloffs[0]
                        print()
                        print("Module %d:" % module)
                        if foundStdInit:
                            print("LcdInit (Tbl) (found by call) at 0x%04x (0x%06x)" % (p + start, p + flashaddr))
                        else:
                            print("LcdInit (Tbl) (found by scan) at 0x%04x (0x%06x)" % (p + start, p + flashaddr))
                lcdinit_type = SCAN_TABLE
                found = True
                break;
        if found == False:
                for t in scan_locate_lcdinit:
                    initbloffs = []
                    if foundInit == True:
                        initbloffs.append(p)
                    else:
                        scanner = Scanner(data, t)
                        scanner.scan((initbloffs, start), add_offs)
                    if len(initbloffs) > 0:
                        p = initbloffs[0]
                        lcdinit_type = SCAN_CUSTOM
                        crc_init = binascii.crc32(data[p:p+100]) & 0xffffffff
                        if verbose:
                                print()
                                print("Module %d:" % module)
                                if foundInit == True:
                                    print("LcdInit !NOTBL! (found by call) at 0x%04x (0x%06x), CRC = 0x%x" % (p + start, p + flashaddr, crc_init))
                                else:
                                    print("LcdInit !NOTBL! (found by scan) at 0x%04x (0x%06x), CRC = 0x%x" % (p + start, p + flashaddr, crc_init))
                                if verbose > 1:
                                        lcdinit_tmp = data[p:p+0x800]
                                        outf = open("lcd.tmp/tmp_lcdinit.bin", "wb")
                                        outf.write(lcdinit_tmp)
                                        outf.close()
                                        outf = open("lcd.tmp/disassemble.sh", "a")
                                        outf.write("#!/bin/sh\n#\ncp ../identify_d52.in tmp_lcdinit.ctl\nd52 -p -b -n tmp_lcdinit.bin x%04x\n" % (p + start))
                                        outf.close()
                                        print("Written to lcd.tmp/tmp_lcdinit.bin.")
                        break
        crc_tbl = 0

        initbloffs = []
        scanner = Scanner(data, scan_locate_initbl)
        scanner.scan((initbloffs, start), add_offs)
        if len(initbloffs) == 2:
            try:
                p = initbloffs[1] - start
                i = struct.unpack("B", data[p:p+1])[0]
                if verbose:
                        if lcdinit_type == SCAN_NOTFOUND:
                                print()
                        print("Module %d:" % module)
                        print("LcdScheduleTbl  at 0x%04x (0x%06x), len 0x%02x" % (p + start, p + flashaddr, i))
                        #init_tables.append(["_custom_scheduletbl", data[p+1:p+i+1]])
                p += i + 1
                i = struct.unpack("B", data[p:p+1])[0]
                lcdcontrasttbl_paracount = struct.unpack("B", data[p+1:p+2])[0]
                if verbose:
                        print("LcdContrastTbl  at 0x%04x (0x%06x), len 0x%02x" % (p + start, p + flashaddr, i))
                        l = struct.unpack("B", data[p+2:p+3])[0]
                        init_tables.append([TABLE_CONTRAST, "_custom_contrasttbl", data[p+3:p+l+3]])
                        global contrast_table_len
                        contrast_table_len = l
                p += i + 1
                i = struct.unpack("B", data[p:p+1])[0]
                if verbose:
                        print("LcdContrastTb2  at 0x%04x (0x%06x), len 0x%02x" % (p + start, p + flashaddr, i))
                        ctbl2_offs = []
                        for j in range(lcdcontrasttbl_paracount):
                                ctbl2_offs.append(struct.pack("B", struct.unpack("<H", data[p+j+1 : p+j+3])[0] - p - start - 2))
                        ctbl2 = data[p+len(ctbl2_offs)*2+2:p+i+1];
                        ctbl2 += b'\xFF';
                        init_tables.append([TABLE_CONTRAST, "_custom_contrasttbl2", ctbl2])
                        init_tables.append([TABLE_CONTRAST, "_custom_contrasttbl2_offsets", ctbl2_offs])
                p += i + 1
                i = struct.unpack("B", data[p:p+1])[0]
                if verbose:
                          print("LcdBacklightTbl at 0x%04x (0x%06x), len 0x%02x" % (p + start, p + flashaddr, i))
                          init_tables.append([TABLE_BACKLIGHT, "_custom_backlighttbl", data[p+1:p+i+1]])
                p += i + 1
                j = i
                i = struct.unpack("B", data[p:p+1])[0]
                if (i == j):            # seems to have two LcdBackLightTbls...
                        if verbose:
                                print("LcdBacklightTb2 at 0x%04x (0x%06x), len 0x%02x" % (p + start, p + flashaddr, i))
                                init_tables.append([TABLE_BACKLIGHT, "_custom_backlighttbl2", data[p+1:p+i+1]])
                        if i > 0:
                                global backlight_table2
                                backlight_table2 = True
                        p += i + 1
                else:
                        init_tables.append([TABLE_BACKLIGHT, "_custom_backlighttbl2", ""])

                l = struct.unpack("<H", data[p : p+2])[0]
                p += 2
                if (p + l) < len(data):
                    lcdinitbl = data[p:p+l]
                    if verbose:
                        print("LcdIniTbl       at 0x%04x (0x%06x), len 0x%02x," % (p + start, p + flashaddr, l), end=' ')
                        init_tables.append([TABLE_INIT, "_custom_initseq", data[p:p+l] + b"\xFF"])
                    crc_tbl = binascii.crc32(lcdinitbl) & 0xffffffff
                    if verbose:
                                print("CRC = 0x%x" % crc_tbl)
                                if verbose > 1:
                                #    dump_tables(init_tables)
                                    print("Written to lcd.tmp/lcdinit.s.")
                # Find the timer1 init values
                global tmr1per
                tmr1per = find_tmr1init(data, start, "standard")
            except:
                if verbose:
                        print("Invalid LcdIniTbl!")
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
                        print()
                        print("OpenWin        at 0x1280 (0x%06x), len 0x%02x," %  (p, l), end=' ')
                openwin = buf[p:p+l]
                if verbose > 1:
                        outf = open("lcd.tmp/tmp_lcdblit.bin", "wb")
                        outf.write(openwin)
                        outf.close()
                        outf = open("lcd.tmp/disassemble.sh", "a")
                        outf.write("#!/bin/sh\n#\ncp ../identify_d52.in tmp_lcdblit.ctl\nd52 -p -b -n tmp_lcdblit.bin x1280\n")
                        #outf.write("echo 'If tmp_lcdblit.d52 contains the string $$$_0x77_$$$, edit tmp_lcdblit.ctl and try again!'\necho")
                        outf.write("grep -q \"_0x77_\" tmp_lcdblit.d52\nif [ $? -eq 0 ] ; then\n    echo \"Wrong _g_blit mapping -- trying again...\"\n")
                        outf.write("    sed \"s/^\(r 7[7-9,ab-f]\)/;;\\1/\" < ../identify_d52.in > /tmp/tmp_lcdblit.ctl\n")
                        outf.write("    sed \"s/^;\(r 7[7-9,ab-f]\)/\\1/\" < /tmp/tmp_lcdblit.ctl > tmp_lcdblit.ctl\n")
                        outf.write("    d52 -p -b -n tmp_lcdblit.bin x1280\nfi\n")
                        outf.close()
                c = binascii.crc32(openwin) & 0xffffffff
                if verbose:
                        print("CRC = 0x%x" % c)
                        if verbose > 1:
                                print("Written to lcd.tmp/tmp_lcdblit.bin.")
        else:
                c = 0
                if len(openwinoffs) > 3:
                        print("More than one match!")
                        if verbose:
                                for i in range(len(openwinoffs) / 3):
                                        print("Match %d at flashoffset 0x%06x, adr 0x%06x len 0x%04x" % (i, openwinoffs[i*3], openwinoffs[i*3 + 2], (openwinoffs[i*3 + 1]) - 0xa80))
                else:
                        print("Not found.")

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
        crc_match = []
        partial_match = []

        # Check if its a buildwin fw
        print("Looking for firmware.............:", end=' ')
        num_modules = isBuildwinFw(dump)
        if num_modules > 0:
                w, h = struct.unpack("<HH", dump[0x23:0x27])
                mfg = "buildwin"
                if jumptable_offset == JUMPTABLE_OFFSET_COBY:
                        mfg = "coby"
                elif jumptable_offset == JUMPTABLE_OFFSET_COBY2:
                        mfg = "coby2"
                print("Found (%s, %dx%d px)." % (mfg, w, h))
        else:
                print("Not found.")
                print()
                if num_modules == -1:
                        print("This is already a custom firmware!")
                else:
                        print("This in no known firmware!")
                return None, partial_match, crc_match

        # Try to find OpenWin and LcdIniTbl
        print("Looking for Openwin..............:", end=' ')
        crc_openwin = find_openwin(dump)
        if crc_openwin == 0:
                print("WARNING: This is no \"standard\" buildwin software!")
        elif not verbose:
                print("Found.")

        print("Looking for LcdIniTbl............:", end=' ')
        initbl_count = 0
        crc_initbl = 0
        crc_init = 0
        global init_tables
        init_tables_for_lcdinit = []
        for i in range(1, num_modules):
                ct, ci = find_initbl(dump, i)
                if ct != 0:
                        crc_initbl = ct
                        initbl_count += 1
                if ci != 0:
                        crc_init = ci
                if len(init_tables) > 0:
                    init_tables_for_lcdinit = init_tables

        if crc_initbl == 0:
                crc_initbl = crc_init
        if initbl_count == 0:
                print("None.")
        elif initbl_count > 1:
                print("%d matches." % initbl_count)
        elif not verbose:
                print("Found.")
        if not crc_initbl:
                print("WARNING: LcdInit NOT FOUND! This is no \"standard\" buildwin software!")

        # some additional infos (for development only)
        print("Looking for backlight & contrast.:", end=' ')
        setcontrast_count = 0
        setbacklight_count = 0
        poweroninit_count = 0
        setcontrast_type = 0
        setbacklight_type = 0
        for i in range(1, num_modules):
            c1, c2, c3 = find_setbacklight_and_contrast(dump, i)
            if c1 != SCAN_NOTFOUND:
                setcontrast_count += 1
                setcontrast_type = c1
            if c2 != SCAN_NOTFOUND:
                setbacklight_count += 1
                setbacklight_type = c2
            if c3 == True:
                poweroninit_count += 1
        if verbose:
            if setbacklight_count == 0:
                print("WARNING: This fw has no detectable Lcd_Backlight routine!")
            elif setbacklight_count > 1:
                print("WARNING: %d Lcd_Backlight matches, using last one!" % setcontrast_count)
            if setcontrast_count == 0:
                print("WARNING: This fw has no detectable Lcd_Contrast routine!")
            elif setcontrast_count > 1:
                print("WARNING: %d Lcd_Contrast matches, using last one!" % setcontrast_count)
            if verbose > 1:
                if setbacklight_count == 0 or setcontrast_count == 0:
                    if poweroninit_count == 0:
                        print("WARNING: This fw has no detectable Power_On_Init routine!")
                    elif poweroninit_count > 1:
                        print("WARNING: %d Power_On_init matches, using last one!" % poweroninit_count)
                    if poweroninit_count > 0:
                        print("         Use poweroninit_tmp for further investigation!")
        else:
            if setbacklight_type != SCAN_NOTFOUND:
                print("Found,", end=' ')
            else:
                print("None,", end=' ') 
            if setcontrast_type != SCAN_NOTFOUND:
                print("Found.")
            else:
                print("None.") 

        if verbose > 1:
            default_contrast = 0
            default_backlight = 0
            defaultcb = []    
            scanner = Scanner(dump[0:0x10000], scan_locate_contrast_backlight_default)
            scanner.scan((defaultcb, 0), add_vals)
            if len(defaultcb) == 3:
                default_contrast = defaultcb[1]
                default_backlight = defaultcb[2]

                print()
                print("Signature (OpenWin, Init, Backlight, Contrast): 0x%x, 0x%x, %d, %d" % (crc_openwin, crc_initbl, setbacklight_type, setcontrast_type))
                if default_contrast > 0:
                    print("Default contrast  = %d" % default_contrast)
                    print("Default backlight = %d" % default_backlight)
                else:
                    print("Default contrast/backlight init not found!")
                print()
                
            # build a skeleton dpfmodel.h
            outf = open("lcd.tmp/dpfmodel.h", "w")
            outf.write("//\n// Type: \n// Comment: \n//\n// Status: (supported/wip/unsupported)\n// Credit: By \n// Date: %s\n//\n" % datetime.datetime.now().strftime("%Y-%m-%d"))
            outf.write("// Url: \n//\n// The signature identify.py detected (OpenWin, Init, Backlight, Contrast)\n")
            outf.write("// CRC: 0x%x, 0x%x, %d, %d\n//\n//\n" % (crc_openwin, crc_initbl, setbacklight_type, setcontrast_type))
            outf.write("#define LCD_CONTROLLER_CUSTOM\n")
            outf.write("#define LCD_WIDTH  %03dL\n" % w)
            outf.write("#define LCD_HEIGHT %03dL\n\n" % h)
            if setbacklight_type == SCAN_NOTFOUND:
                outf.write("//\n//NOTE from identify.py: no detectable LCD_Backlight routine!\n//\n")
                outf.write("//")
            outf.write("#define LCD_DEFAULT_BRIGHTNESS_VALUE %d\n" % default_backlight)
            if default_backlight == 0 or setbacklight_type == SCAN_NOTFOUND:
                outf.write("//")
            outf.write("#define LCD_USER_ADJUSTABLE_BRIGHTNESS   //brightness adjustable in menu\n")
            outf.write("//#define LCD_MAX_BRIGHTNESS_VALUE 21      //undefine & change, if max != 21\n\n")

            if setcontrast_type == SCAN_NOTFOUND:
                outf.write("//\n// NOTE from identify.py: no detectable LCD_Contrast routine!\n//\n")
            if default_contrast == 0 or setcontrast_type == SCAN_NOTFOUND:
                outf.write("//")
            outf.write("#define LCD_DEFAULT_CONTRAST_VALUE %d\n" % default_contrast)
            if default_contrast == 0 or setcontrast_type == SCAN_NOTFOUND:
                outf.write("//")
            global contrast_table_len
            if contrast_table_len > 0:
                outf.write("#define LCD_MAX_CONTRAST_VALUE %d\n" % contrast_table_len)
            else:
                outf.write("#define LCD_MAX_CONTRAST_VALUE ???        // NOTE from identify.py: value not detected\n")
            outf.write("//#define LCD_USER_ADJUSTABLE_CONTRAST     //undefine to make contrast adjustable in menu\n\n")

            global tmr1con
            outf.write("//NOTE: PWM_NONE - pwm output disabled, timer1 stopped\n")
            outf.write(  "//      PWM_P23  - pwm output on P2.3, timer1 running\n")
            outf.write(  "//      PWM_P40  - pwm output on P4.0, timer1 running\n")
            if tmr1con == 0x43:
                outf.write("//#define LCD_TIMER1_PWM_NONE\n#define LCD_TIMER1_PWM_P23\n//#define LCD_TIMER1_PWM_P40\n\n")
            elif tmr1con == 0x83:
                outf.write("\n//#define LCD_TIMER1_PWM_NONE\n//#define LCD_TIMER1_PWM_P23\n#define LCD_TIMER1_PWM_P40\n\n")
            else:
                outf.write("\n#define LCD_TIMER1_PWM_NONE\n//#define LCD_TIMER1_PWM_P23\n//#define LCD_TIMER1_PWM_P40\n\n")

            outf.write("//NOTE: Undefine this if there are backlight-flashes on power-on\n//      or if backlight stays on after menu|off\n//#define LCD_P23_ACTIVE_HIGH\n\n")

            global tmr1per, tmr1per_custom, backlight_table2
            outf.write("//NOTE: Init value for tmr1per\n")
            outf.write("//      Undefine if _custom_backlighttbl2 used or lcd_custom_setbacklight initializes period\n")
            outf.write("//      (pwm output on P2.3 or P4.0, timer running, period not initialized)\n")
            if backlight_table2 == True:
                outf.write("\n//#define LCD_TIMER1_PERIOD ---  //_custom_backlighttbl2 used - LCD_TIMER1_PERIOD not used!\n")
            else:
                if tmr1per_custom != 0:
                    outf.write("#define LCD_TIMER1_PERIOD 0x%04x  //custom value, standard table value = 0x%04x\n" % (tmr1per_custom, tmr1per))
                elif tmr1per != 0:
                    outf.write("#define LCD_TIMER1_PERIOD 0x%04x  //standard table value\n" % tmr1per)
                else:
                    outf.write("//#define LCD_TIMER1_PERIOD ???  // NOTE from identify.py: value not detected!\n")

            outf.write("\n//NOTE: uncomment only one of the following 4 values!\n")
            if setbacklight_type == SCAN_TABLE:
                outf.write("//#define LCD_BACKLIGHT_NONE\n//#define LCD_BACKLIGHT_VALUE\n#define LCD_BACKLIGHT_TABLE\n//#define LCD_BACKLIGHT_CUSTOM\n")
            elif setbacklight_type == SCAN_VALUE:
                outf.write("//#define LCD_BACKLIGHT_NONE\n#define LCD_BACKLIGHT_VALUE\n//#define LCD_BACKLIGHT_TABLE\n//#define LCD_BACKLIGHT_CUSTOM\n")
            elif setbacklight_type == SCAN_CUSTOM:
                outf.write("//#define LCD_BACKLIGHT_NONE\n//#define LCD_BACKLIGHT_VALUE\n//#define LCD_BACKLIGHT_TABLE\n#define LCD_BACKLIGHT_CUSTOM\n")
            else:
                outf.write("#define LCD_BACKLIGHT_NONE                // NOTE from identify.py: have a look at poweroninit.d52!\n")
                outf.write("//#define LCD_BACKLIGHT_VALUE\n//#define LCD_BACKLIGHT_TABLE\n//#define LCD_BACKLIGHT_CUSTOM\n")

            outf.write("\n// NOTE: uncomment only one of the following 3 values!\n")
            if setcontrast_type == SCAN_TABLE:
                outf.write("//#define LCD_CONTRAST_NONE\n#define LCD_CONTRAST_TABLE\n//#define LCD_CONTRAST_CUSTOM\n")
            elif setcontrast_type == SCAN_CUSTOM:
                outf.write("//#define LCD_CONTRAST_NONE\n//#define LCD_CONTRAST_TABLE\n#define LCD_CONTRAST_CUSTOM\n")
            else:
                outf.write("#define LCD_CONTRAST_NONE                // NOTE from identify.py: have a look at poweroninit.d52!\n")
                outf.write("//#define LCD_CONTRAST_TABLE\n//#define LCD_CONTRAST_CUSTOM\n")

            outf.write("\n// NOTE: uncomment to swap the buttons for up/down menu navigation\n")
            outf.write("//#define LCD_LRBUTTON_SWAP\n")

            outf.write("\n// NOTE: uncomment to use alt buttons for up/down menu navigation\n")
            outf.write("//#define LCD_LR_ALT_BUTTONS\n")
                
            outf.close()

            # build skeleton lcdsetor.s
            outf = open("lcd.tmp/lcdsetor.s", "w")
            outf.write("	.include 'dpf.inc'\n\n	.area INIT (CODE)\n\n; Just a nop at the moment - only landscape supported!\n")
            outf.write("; If you want other orientations, add code here,\n; create a correspondig method to lcdblit_*.s and modify lcdinit_*.c accordingly.\n\n")
            outf.write("_lcd_custom_setorientation::\n	ret\n")
            outf.close()

            # build skeleton lcdblit.s
            outf = open("lcd.tmp/lcdblit.s", "w")
            outf.write("\t.include 'dpf.inc'\n\n\t.area BLIT (CODE)\n")
            outf.write(";\n; Call to ROM lcd write routine\n; Replace, if custom lcd write is required\n;\n")
            outf.write("lcd_write::\n\tljmp\totp_lcd_write\n\n\n")
            outf.write("ar0 = 0x00\n\n")
            if h == w:
                outf.write("_custom_blit::\n")
            elif h > w:
                outf.write("_custom_portrait_blit::\n")
            else:
                outf.write("_custom_landscape_blit::\n")
            outf.write("\n***** PUT CODE FROM tmp_lcdblit.d52 HERE *****\n")
            outf.close()

            # build skeleton lcdinit.s
            outf = open("lcd.tmp/lcdinit.s", "w")
            outf.write("\t.include 'dpf.inc'\n\n\t.area LCDAUX (CODE)\n\n_lcd_custom_init::\n")
            s = ""
            global lcdinit_type
            if lcdinit_type == SCAN_CUSTOM:
               outf.write(";\n***** PUT CODE FROM tmp_lcdinit.d52 HERE *****\n;")
               outf.write("\n;\n; If initcode ends with something like\n;\n;\tmov\ta,#0x??\n;\tmov\tdptr,#X????\n;\tlcall\ttramp_jsr\n;\tret\n;\n")
               outf.write("; Replace it with the following code:\n;\n")
               s = ";"
            else:
                outf.write("\tlcall\tlcd_reset\n;\n")
            outf.write("%s\tmov\tdptr,#_custom_initseq\n%s\tljmp\t_lcd_init_by_table\n\n" % (s, s))

            if setbacklight_type != SCAN_CUSTOM:
                outf.write(";\n; If custom backlight handling is needed, uncomment the following label\n; and set LCD_BACKLIGHT_CUSTOM in dpfmodel.h\n; r3 contains brightness value (0 .. LCD_MAX_BRIGHTNESS_VALUE)\n;\n")
                outf.write(";_lcd_custom_setbacklight::\n\n")
            else:
                outf.write("_lcd_custom_setbacklight::\n\tmov\ta,r3\n\tjnz\tdoset\n\tret\ndoset:\n;\n***** PUT CODE FROM tmp_setbacklight.d52 HERE *****\n;\n\n")

            if setcontrast_type != SCAN_CUSTOM:
                outf.write(";\n; If custom contrast handling is needed, uncomment the following label\n; and set LCD_CONTRAST_CUSTOM in dpfmodel.h\n; r3 contains contrast value (1 .. LCD_MAX_CONTRAST_VALUE)\n;\n")
                outf.write(";_lcd_custom_setcontrast::\n\n")
            else:
                outf.write("_lcd_custom_setcontrast::\n;\n***** PUT CODE FROM tmp_setcontrast.d52 HERE *****\n;\n\n")

            outf.write("\t.area LCDAUX (CODE)\n\n")

            outf.write("\n;backlight_table::\n;\t.db\tIf needed, put data for custom backlight handling here\n")
            outf.write(";_custom_backlighttbl_len::  .db  . - backlight_table\n\n")
            outf.write("\n;contrast_table::\n;\t.db\tIf needed, put data for custom contrast handling here\n")
            outf.write(";_custom_contrasttbl_len::  .db  . - contrast_table\n\n")

            bcomment = False
            ccomment = False
            for tbl in init_tables_for_lcdinit:
                s = ""
                if tbl[0] == TABLE_BACKLIGHT and setbacklight_type != SCAN_TABLE:
                    if bcomment == False:
                        outf.write(";\n; NOTE from identify.py: not used\n;\n")
                        bcomment = True
                    s = ";"
                if tbl[0] == TABLE_CONTRAST and setcontrast_type != SCAN_TABLE:
                    if ccomment == False:
                        outf.write(";\n; NOTE from identify.py: not used\n;\n")
                        ccomment = True
                    s = ";"
                outf.write(s)
                outf.write(tbl[1])
                outf.write("::")
                for  i in range(0, len(tbl[2])):
                        if (i % 10) == 0:
                                outf.write("\n%s\t.db\t" % s)
                        else:
                                outf.write(", ")
                        if isinstance(tbl[2][i], bytes):
                            outf.write("0x%02x" % struct.unpack("B", tbl[2][i:i+1][0])[0])
                        else:
                            outf.write("0x%02x" % struct.unpack("B", tbl[2][i:i+1])[0])
                outf.write("\n%s%s_len::  .db  %d" % (s, tbl[1], len(tbl[2])))
                outf.write("\n\n")

            outf.close()


        # Now look for display known by their signatur LcdInit / OpenWin / Backlight / Contrast (new way)
        print("Looking for known signatures.....:", end=' ')
        if verbose:
                print()
        for i in knowntypes.KNOWN_TYPES:
                if 'CRC' in i:
                        record = i['CRC']
                        crcmatch = False
                        for r in record:
                                if crc_openwin == r[0] and crc_initbl == r[1]:
                                    crcmatch = True
                                    if verbose:
                                        print("OpenWin & LcdIniTbl: match at model %s" % i['Type'])
                                        if len(r) > 2:
                                            if setbacklight_type == r[2]:
                                                print(" + Backlight Type match")
                                            if setcontrast_type == r[3]:
                                                print(" + Contrast Type match")
                                        else:
                                            print("NOTE: Backlight & Contrast Type not in signature!")
                                    found = False
                                    if len(r) < 3:
                                        found = True
                                    else:
                                        if setbacklight_type == r[2] and setcontrast_type == r[3]:
                                            found = True

                                    if found:
                                        if not verbose:
                                            print("Found.")
                                        return i, partial_match, crc_match
                        if crcmatch == True:
                            crc_match.append(i['Type'])
                                
                        if i['Status'] == 'supported' and crcmatch == False:
                                for r in record:
                                        if crc_openwin == r[0]:
                                                if not i['Type'] in partial_match:
                                                    partial_match.append(i['Type'])
                                                if verbose:
                                                        print("OpenWin:   match at model %s" % i['Type'])
                                        if crc_initbl == r[1]:
                                                if not i['Type'] in partial_match:
                                                    partial_match.append(i['Type'])
                                                if verbose:
                                                        print("LcdIniTbl: match at model %s" % i['Type'])

        print("None.")

        return None, partial_match, crc_match
        
#
#
# MAIN

if os.name == 'posix':
################ LINUX ################

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
        print("Usage: %s [option] dumpfile|dpf-devicenode\n  -v   show more infos\n  -d  build developer support files in lcd.tmp" % sys.argv[0])
        sys.exit(-1)

    # Make dir for writing temp code
    if not os.path.exists("lcd.tmp"):
        os.makedirs("lcd.tmp")
    else:
        os.system("rm -f lcd.tmp/*")

    dumpfile = arg

    if arg.startswith("/dev/"):
    #
    # copy of fulldump.py
    #
        print("Detecting & reading dpf flash...")
        d = dpf.open(arg)
        size = detect.detect_flash(d)
        # Offset, size
        print("Reading %x bytes from flash..." % size)
        buf = d.readFlash(0x00, size)
        dump_filename="fulldump_" + datetime.datetime.now().strftime("%Y%m%d-%H%M%S") + ".bin"        
        f = open(dump_filename, "wb")
        f.write(buf)
        f.close()
        d.close()
        print("Flash written to file '%s'." % dump_filename)
        print()
        dumpfile = dump_filename
else:
################ WINDOWS ################    
    if len(sys.argv) != 2:
        print("Usage: %s dumpfile|DPF_CDROM_driveletter\n" % sys.argv[0])
        sys.exit(-1)
        
    if len(sys.argv[1]) <= 2:
        print("Running on windows - unable to detect flash size")
        print("Reading %x bytes from flash..." % (1024*1024))
        buf = read_flash_win(sys.argv[1])
        dump_filename="fulldump_win_" + datetime.datetime.now().strftime("%Y%m%d-%H%M%S") + ".bin"        
        f = open(dump_filename, "wb")
        f.write(buf)
        f.close()
        print("Flash written to file '%s'." % dump_filename)
        print()
        dumpfile = dump_filename
    else:
        dumpfile = sys.argv[1]
                
f = open(dumpfile, "rb")
data = f.read()
f.close()
dpf, pm, cm = recognize_dpf(data)

print()
    
if dpf:
        if dpf['Status'] == "supported":
                print("Your dpf is compatible with model %s." % dpf['Type'])
        elif dpf['Status'] == "unsupported":
                print("Your dpf is compatible with unsupported model %s." % dpf['Type'])
                print("Sorry, this dpf in NOT supported by dpf-ax.")
        elif dpf['Status'] == "wip":
                print("Your dpf is compatible with currently unsupported model %s." % dpf['Type'])
                print("Sorry, this dpf in NOT supported by dpf-ax at the moment.")
                print("Work in progress. Check back later.")
else:
        print("Sorry, no matching dpf found.")
        s = "But the"
        if len(cm) > 0:
            print()
            s = "And the"
            if len(cm) > 1:
                print("But the following models have very similar signatures:", end=' ')
                for model in cm:
                    print(model[0], end=' ')
                print("\nIt is likely that one of these firmwares will work for your dpf.")
            else:
                print("But the following model has a very similar signature:", end=' ')
                print("%s.\nIt is likely that this firmware will work for your dpf." % cm[0][0])
        if len(pm) > 0:
            print()
            if len(pm) > 1:
                print("%s following models have partial signature matches:" % s, end=' ')
                for model in pm:
                    print(model[0], end=' ')
                print("\nOne of these firmwares MAY work for your dpf.")
            else:
                print("%s following model has a partial signature match:" % s, end=' ')
                print("%s.\nThis firmware MAY work for your dpf." % pm[0][0])
print()

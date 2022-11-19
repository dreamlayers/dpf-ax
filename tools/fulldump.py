#!/usr/bin/python
# Script to extract flash
#
# <hackfin@section5.ch>
#
# Argument is either:
#
# For linux:
# /dev/sgX : scsi device (unhacked mode)
# usb0     : developer mode (already hacked)
#
# For windows:
# X: Buildwin CDROM drive letter

import sys
import os
import datetime

if os.name == 'posix':
    sys.path.append("../Debug")
    import dpf
    import detect
else:
    from readflash_win import read_flash_win
        
argerr = 0

dump_filename="fulldump_" + datetime.datetime.now().strftime("%Y%m%d-%H%M%S") + ".bin"        
if os.name == 'posix':
################ LINUX ################
    if len(sys.argv) != 2:
        print("Usage: %s /dev/sgX|/dev/srX|USB0\n" % sys.argv[0])
        sys.exit(-1)

    d = dpf.open(sys.argv[1])

    size = detect.detect_flash(d)

    # Offset, size
    print("Reading %x bytes from flash..." % size)
    buf = d.readFlash(0x00, size)
    d.close()
else:
################ WINDOWS ################    
    dump_filename="fulldump_win_" + datetime.datetime.now().strftime("%Y%m%d-%H%M%S") + ".bin"        
    if len(sys.argv) != 2:
        argerr = 1
    elif len(sys.argv[1]) > 2:
        argerr = 1
    if argerr:
        print("Usage: %s DPF_CDROM_driveletter\n"  % sys.argv[0])
        sys.exit(-1)

    print("Running on windows - unable to detect flash size")
    print("Reading %x bytes from flash..." % (1024*1024))
    buf = read_flash_win(sys.argv[1])

f = open(dump_filename, "wb")
f.write(buf)
print("Flash written to file '%s'." % dump_filename)
f.close()


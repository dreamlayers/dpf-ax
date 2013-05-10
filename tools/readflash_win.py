#
# Helper to read flash (windows)
#
# NOTE: Works ONLY for original firmware!
#       Dpf must be connected as CD-ROM.
#
import sys
import win32file
from ctypes import *
from ctypes.wintypes import *

class SCSI_PASS_THROUGH_DIRECT(Structure):
    _fields_ = [
        ( "Length", USHORT),
        ( "ScsiStatus", c_ubyte),
        ( "PathId", c_ubyte),
        ( "TargetId", c_ubyte),
        ( "Lun", c_ubyte),
        ( "CdbLength", c_ubyte), 
        ( "SenseInfoLength", c_ubyte),
        ( "DataIn", c_ubyte),
        ( "Align32", c_ubyte * 3),
        ( "DataTransferLength", ULONG),
        ( "TimeOutValue", ULONG),
        ( "DataBuffer", LPVOID),
        ( "SenseInfoOffset", ULONG),
        ( "Cdb", c_ubyte * 16) ]

SCSI_PASS_THROUGH_DIRECT_LENGTH = 44

class SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER(Structure):
    _fields_ = [
        ( "spt", SCSI_PASS_THROUGH_DIRECT),
        ( "Filler", ULONG),
        ( "ucSenseBuf", c_ubyte * 32) ]

SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER_LENGTH = 80
SENSEINFOOFFSET = SCSI_PASS_THROUGH_DIRECT_LENGTH + 4

IOCTL_SCSI_PASS_THROUGH_DIRECT = 0x4D014

def read_flashpart_win(dhandle, buf, offset, length):
        swb = SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER()

        swb.spt.Length = SCSI_PASS_THROUGH_DIRECT_LENGTH
        swb.spt.TimeOutValue = 2
        swb.spt.DataBuffer = cast(byref(buf), c_void_p)
        swb.spt.SenseInfoOffset = SENSEINFOOFFSET
        swb.spt.DataIn = 0x01   #SCSI_IOCTL_DATA_IN
        swb.spt.DataTransferLength = length
        swb.spt.CdbLength = 0x10
        swb.spt.Cdb = (c_ubyte * 16) (0xCD, 0, 0, 0, 0, 0,
                                      0x04, (length >> 16) & 0xFF, (length >> 8) & 0xFF, length & 0xFF,
                                      0x03, (offset >> 16) & 0xFF, (offset >> 8) & 0xFF, offset & 0xFF,
                                      0, 0)

        dwReturned = DWORD(0)

        windll.kernel32.DeviceIoControl.argtypes = [HANDLE, DWORD, LPVOID, DWORD, LPVOID, DWORD, POINTER(DWORD), c_void_p]
        windll.kernel32.DeviceIoControl.restype = BOOL

        res = BOOL(windll.kernel32.DeviceIoControl(dhandle.handle, DWORD(IOCTL_SCSI_PASS_THROUGH_DIRECT), byref(swb), SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER_LENGTH, byref(swb), SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER_LENGTH, byref(dwReturned), 0))

        if res.value == 0:
            print("Failed: DeviceIoControl - Error code: %d" % windll.kernel32.GetLastError())
            sys.exit(-1)

def read_flash_win(driveletter):
    drivepath = "\\\\.\\" + driveletter
    if len(driveletter) == 1:
        drivepath += ":"

    try:
        drive_handle=win32file.CreateFile(drivepath,
                                     win32file.GENERIC_READ|win32file.GENERIC_WRITE,
                                     win32file.FILE_SHARE_READ,
                                     None,
                                     win32file.OPEN_EXISTING,
                                     0,
                                     None);
    except:
        drive_handle = win32file.INVALID_HANDLE_VALUE
        
    if drive_handle == win32file.INVALID_HANDLE_VALUE:
        print("Error: could not open drive '%s' - dpf connected to PC?\n" % driveletter)
        sys.exit(-1)

    addr = 0
    readbuf = create_string_buffer(0x1000)
    buf = b''
    while addr < (1024*1024):
        read_flashpart_win(drive_handle, readbuf, addr, 0x1000)
        addr += 0x1000
        buf += readbuf.raw
    return buf

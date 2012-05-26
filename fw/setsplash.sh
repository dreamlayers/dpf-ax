#!/bin/sh
#
# setplash.sh
#
# Simple image to rgb565 conversion & flashing
#
# 2012/05/25 superelchi
#   Added ffmpeg version detection
# 2012/04/25 superelchi
#   First version   
#
#

if [ $# -lt 3 ] ; then
    echo "Usage:"
    echo "  convert image to rgb565:   $0 <image-file> <width> <height>"
    echo "  convert & write to flash:  $0 <image-file> <width> <height> <device>"
    exit 1
fi

name=${1%.*}

ffmpeg -pix_fmt list 2>/dev/null | grep -q rgb565be
ret=$?
if [ $ret -eq 0 ] ; then
#
# ffmpeg supports rgb565be
#
echo "Using ffmpeg pix_fmt rgb565be"
ffmpeg -y -f image2 -i "$1" -vcodec rawvideo -f rawvideo -pix_fmt rgb565be -s $2x$3 "${name}.$2x$3.rgb" > ffmpeg.log 2>&1
ret=$?
if [ $ret -ne 0 ] ; then
    echo "ffmpeg error: $ret (see file ffmpeg.log)."
    exit 1
fi
else
#
# ffmpeg does not support rgb565be
#
echo "Using ffmpeg pix_fmt rgb565"
ffmpeg -y -f image2 -i "$1" -vcodec rawvideo -f rawvideo -pix_fmt rgb565 -s $2x$3 "${name}.tmp" > ffmpeg.log 2>&1
ret=$?
if [ $ret -ne 0 ] ; then
    echo "ffmpeg error: $ret (see file ffmpeg.log)."
    exit 1
fi
# for "le" systems:
dd if="${name}.tmp" of="${name}.$2x$3.rgb" conv=swab >/dev/null 2>&1 
#
#for "be" systems:
#mv "${name}.tmp" "${name}.$2x$3.rgb"
rm -f "${name}.tmp"
fi
rm -f ffmpeg.log

echo "Splashfile saved to ${name}.$2x$3.rgb"
if [ $# -ge 4 ] ; then
    python splash2flash.py "${name}.$2x$3.rgb" $2 $3 "usb0"
fi


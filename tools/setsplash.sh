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

ffmpeg -pix_fmts 2>/dev/null | grep -q rgb565be
ret=$?
if [ $ret -ne 0 ]; then
    echo "ffmpeg does not support rgb565be pix_fmt"
    exit 1
fi

ffmpeg -y -f image2 -i "$1" -vcodec rawvideo -f rawvideo -pix_fmt rgb565be -s $2x$3 "${name}.$2x$3.rgb" > ffmpeg.log 2>&1
ret=$?
if [ $ret -ne 0 ] ; then
    echo "ffmpeg error: $ret (see file ffmpeg.log)."
    exit 1
fi

rm -f ffmpeg.log

echo "Splashfile saved to ${name}.$2x$3.rgb"
if [ $# -ge 4 ] ; then
    python splash2flash.py "${name}.$2x$3.rgb" $2 $3 "usb0"
fi

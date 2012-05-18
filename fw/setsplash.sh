#!/bin/sh
#
# setplash.sh
#
# Simple image to rgb565 conversion & flashing
#
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

ffmpeg -y -f image2 -i "$1" -vcodec rawvideo -f rawvideo -pix_fmt rgb565be -s $2x$3 "${name}.$2x$3.rgb" > ffmpeg.log 2>&1
ret=$?
if [ $ret -ne 0 ] ; then
    echo "ffmpeg error: $ret (see file ffmpeg.log)."
    exit 1
fi
rm -f ffmpeg.log
#
# If your version of ffmpeg does not support rgb565be, use following commands:
#
#ffmpeg -y -f image2 -i ${name}.png -vcodec rawvideo -f rawvideo -pix_fmt rgb565 -s $2x$3 "${name}.tmp"
#dd if="${name}.tmp" of="${name}.$2x$3.rgb" conv=swab
#rm -f "${name}.tmp"

#
# Add width/height at start of .rgb (not used anymore)
#
#mv "${name}.$2x$3.rgb" "${name}.tmp"
#printf "0: %.4x" $2 | sed -E 's/0: (..)(..)/0: \2\1/' | xxd -r -g0 >"${name}.rgb"
#printf "0: %.4x" $3 | sed -E 's/0: (..)(..)/0: \2\1/' | xxd -r -g0 >>"${name}.rgb"
#dd bs=1 seek=4 if="${name}.tmp" of="${name}.$2x$3.rgb"
#rm -f "${name}.tmp"

echo "Splashfile saved to ${name}.$2x$3.rgb"
if [ $# -ge 4 ] ; then
    python splash2flash.py "${name}.$2x$3.rgb" $2 $3 "usb0"
fi


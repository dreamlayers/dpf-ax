#!/bin/sh

MODELS="blue white pink pearl pearl_landscape focal focal_landscape linkdelight agk_violet focalprice144 delightdigi_black coby_dp151_white dx27893"

for i in $MODELS; do
	echo --------------------------------------
	echo Building $i
	make TYPE=$i clean all > /dev/null
	make TYPE=$i BUILD_DEVEL=1 clean all > /dev/null
done

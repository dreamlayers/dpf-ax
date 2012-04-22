#!/bin/sh

MODELS="blue white pink pearl pearl_landscape focal focal_landscape linkdelight agk_violet focalprice144 delightdigi_black"

for i in $MODELS; do
	echo --------------------------------------
	echo Building $i
	make TYPE=$i clean all > /dev/null
	cp fw_$i.ihx hexfiles
done

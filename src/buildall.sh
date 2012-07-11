#!/bin/sh

MODELS="blue white pink pearl pearl_landscape focal focal_landscape"
CUSTOM_MODELS=$(ls -1 lcd)

for i in $MODELS $CUSTOM_MODELS; do
	echo --------------------------------------
	echo Building $i
	make TYPE=$i clean all > /dev/null
	make TYPE=$i BUILD_DEVEL=1 clean all > /dev/null
done


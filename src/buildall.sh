#!/bin/sh

MODELS=$(ls -1 lcd)

for i in $MODELS; do
	echo --------------------------------------
	echo Building $i
	make TYPE=$i clean all > /dev/null
	make TYPE=$i BUILD_DEVEL=1 clean all > /dev/null
done


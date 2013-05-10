#!/bin/sh

MODELS=$(ls -1 -I README lcd)

for i in $MODELS; do
	echo --------------------------------------
	cat lcd/$i/dpfmodel.h | grep -q "Status: supported"
	if [ $? -eq 0 ] ; then
	    echo Building $i
	    make TYPE=$i clean all > /dev/null
	    #make TYPE=$i BUILD_DEVEL=1 clean all > /dev/null
	 else
	     echo "$i: Status != supported -- skipping!"
	 fi
done



#!/bin/bash

PREREQUISITES='pkg-config aclocal automake autoconf'

for i in $PREREQUISITES; do
	if [ -z `which $i` ]; then
		echo missing tool "'$i'". Please install package.
		err=1
	fi
done
	
if [ $err ]; then
	echo "Error, cancelling"
	exit
fi

# Allow to pass on dpf distribution location, if not pwd:

if [ -z $1 ]; then
	DPFLIB_LOCATION=`pwd`
else
	DPFLIB_LOCATION=$1
fi
	
if [ -e lcd4linux ]; then
	echo "lcd4linux installed, not fetching"
else
	# Check out source from SVN
	svn co -r1142 https://ssl.bulix.org/svn/lcd4linux/trunk lcd4linux
fi

cd lcd4linux

# Apply patch
if [ -e drv_dpf.c ]; then
	echo "Existing drv_dpf.c found, not patching"
else
	patch -p1 < ../lcd4linux-svn1142-dpf.patch
fi

export CPPFLAGS="-I$DPFLIB_LOCATION/dpflib -I$DPFLIB_LOCATION/include"
export LDFLAGS=-L$DPFLIB_LOCATION/dpflib

if [ -e Makefile ]; then
	echo "Not configuring, Makefile found"
else
	./bootstrap
	# Feel free to configure this differently:
	./configure --with-drivers=DPF
fi

make

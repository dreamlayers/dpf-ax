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

if [ -e lcd4linux ]; then
	echo "lcd4linux installed, not fetching"
else
	# Check out source from SVN
	svn co https://ssl.bulix.org/svn/lcd4linux/trunk lcd4linux
fi

cd lcd4linux

if [ -e Makefile ]; then
	echo "Not configuring, Makefile found"
else
	./bootstrap
	# Feel free to configure this differently:
	./configure --with-drivers=DPF
fi

make

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
	# Tested with revision 1197. Should work with future revisions.
	#svn co -r1197 https://ssl.bulix.org/svn/lcd4linux/trunk lcd4linux
	svn co https://ssl.bulix.org/svn/lcd4linux/trunk lcd4linux
fi

cd lcd4linux

# Apply patch
if [ -e drv_dpf.c ]; then
	grep -q dpf_ax drv_dpf.c
	if [ -z $? ]; then
		echo "Existing drv_dpf.c already patched."
	else
		patch < ../lcd4linux-svn1197-dpf_ax.patch
	fi
else
	echo "No drv_dpf.c found - revision too old?."
	exit -1
fi

if [ -e Makefile ]; then
	echo "Not configuring, Makefile found"
else
	./bootstrap
	# Feel free to configure this differently:
	./configure --with-drivers=DPF
fi

make

# Cygwin/Linux-GCC Makefile
#
# $Id: Makefile 301 2010-09-30 13:09:13Z strubi $
#
# This is the GNU Makefile to create:
# - a Python module for the API tester
#

DEVLIB  = ..

############################################################################
# Configure these to your needs
LIBNAME = dpf
DLLCSRCS =
COMMONSRCDIR = $(DEVLIB)/python
DLLINSTALLDIR = ../$(CONFIG)
DLLCOMMONSRCS = py_device.c
DEBUGOPTS = -g

ifdef DEBUG
	DEBUGOPTS += -DDEBUG
endif

CLIENTLIB = -L$(DEVLIB)/dpflib -ldpf -lusb

############################################################################

MODULENAME = $(LIBNAME)

ifndef PLATFORM
PLATFORM=$(shell "uname")
endif

ifeq (Linux, $(PLATFORM))
	PYTHON=$(shell pyversions -d 2>/dev/null)
ifeq ("", "$(PYTHON)")
	# pyversions replacement for non-debian linux
	PYTHON=$(shell python -V 2>&1 | sed -re 's/.*ython ([0-9]\.[0-9])(\.[0-9])*/python\1/g')
endif
	include $(DEVLIB)/unixdll.mk
	DLLDEPS = $(CLIENTLIB) -l$(PYTHON)
	PYTHONINCLUDES = -I/usr/include/$(PYTHON)
else

	PYTHONSRC = $(HOME)/src/EXTERN/Python26

ifeq (mingw32, $(PLATFORM))
	include $(DEVLIB)/msdll.mk
	PYTHONINCLUDES = -I$(PYTHONSRC)/include
	CFLAGS += -DDLL_EXPORTS
	DLLDEPS = -L$(PYTHONSRC)/libs -lpython26
	DLLDEPS += $(CLIENTLIB) -lws2_32
	DLLTOOL = i586-mingw32msvc-dlltool
	DLLWRAP = i586-mingw32msvc-dllwrap
endif
endif

PYVERS:=$(shell echo $(PYTHON) | grep -Eo "[0-9]\.")
ifeq ($(PYVERS), 3.)
$(error Python3 not supported - use python2)
endif

INCLUDES = -I$(DEVLIB)/include -I$(DEVLIB)/dpflib
INCLUDES += $(PYTHONINCLUDES)

CFLAGS +=  -DMODULENAME=$(MODULENAME)
CFLAGS +=  -Wall

all: dll

install:: installdll
	@cp $(DLL) $(DLLINSTALLDIR)/$(INSTALLNAME)$(DLLEXT)
	@echo -------------------------------------------------------------------
	@echo Python module installed in:
	@echo `pwd`/$(DLLINSTALLDIR)/$(INSTALLNAME)$(DLLEXT)
	@echo -------------------------------------------------------------------
	@echo


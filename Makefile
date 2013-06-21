DISTVERSION := $(shell cat ./version)

CURDIR = $(shell pwd)

# Make sure this path is correct. Needs to point to dpf.h and libdpf.a
DPFLIB = dpflib
INC = include
PYDPF = python
SRC = src
TOOLS = tools

ifndef DESTDIR
DESTDIR = $(CURDIR)/..
endif

default: $(DPFLIB)/libdpf.a
	$(MAKE) -C $(PYDPF) install

all: default firmware lcd4linux

firmware:
	-[ -e $(SRC) ] && (cd $(SRC); ./buildall.sh)

$(DPFLIB)/libdpf.a:
	$(MAKE) -C $(DPFLIB) install

lcd4linux:
	./build-dpf-lcd4linux.sh

dist-all: dist dist-firmware dist-windows

dist: distclean
	-rm -rf /tmp/dpf-ax
	mkdir /tmp/dpf-ax
	cp -r $(DPFLIB) /tmp/dpf-ax
	cp -r $(TOOLS) /tmp/dpf-ax
	cp -r $(INC) /tmp/dpf-ax
	cp -r $(PYDPF) /tmp/dpf-ax
	cp $(shell find . -maxdepth 1 -type f -printf "%f ") /tmp/dpf-ax
	tar --create --gzip --numeric-owner\
	 --file=$(DESTDIR)/dpf-ax_$(shell date +"%Y%m%d").tgz\
	 --directory=/tmp dpf-ax
	-rm -rf /tmp/dpf-ax

dist-firmware: distclean
	-rm -rf /tmp/dpf-ax
	mkdir /tmp/dpf-ax
	cp -r $(SRC) /tmp/dpf-ax
	tar --create --gzip --exclude-vcs --numeric-owner\
	 --file=$(DESTDIR)/dpf-ax_firmware-src_$(shell date +"%Y%m%d").tgz\
	 --directory=/tmp dpf-ax
	(cd $(SRC); ./buildall.sh)
	-rm -rf /tmp/dpf-ax
	mkdir /tmp/dpf-ax
	mkdir /tmp/dpf-ax/src
	cp $(SRC)/fw_*.bin /tmp/dpf-ax/src
	#tar --create --gzip --exclude-vcs --numeric-owner\
	# --file=$(DESTDIR)/dpf-ax_firmware_$(shell date +"%Y%m%d").tgz\
	# --directory=/tmp dpf-ax
	(cd /tmp; zip -X -r -q $(DESTDIR)/dpf-ax_firmware_$(shell date +"%Y%m%d").zip dpf-ax)
	-rm -rf /tmp/dpf-ax

dist-windows:
	-rm -rf /tmp/dpf-ax
	mkdir /tmp/dpf-ax
	mkdir /tmp/dpf-ax/$(TOOLS)
	cp $(TOOLS)/fulldump.py /tmp/dpf-ax/$(TOOLS)
	cp $(TOOLS)/identify.py /tmp/dpf-ax/$(TOOLS)
	cp $(TOOLS)/knowntypes.py /tmp/dpf-ax/$(TOOLS)
	cp $(TOOLS)/readflash_win.py /tmp/dpf-ax/$(TOOLS)
	cp $(TOOLS)/knowntypes.html /tmp/dpf-ax/$(TOOLS)
	cp $(TOOLS)/README.windows /tmp/dpf-ax/$(TOOLS)
	(cd /tmp; zip -X -l -r -q $(DESTDIR)/dpf-ax_windows_$(shell date +"%Y%m%d").zip dpf-ax)
	-rm -rf /tmp/dpf-ax

clean:
	$(MAKE) -C $(DPFLIB) clean
	$(MAKE) -C $(PYDPF) clean
	$(MAKE) -C $(TOOLS) clean
	$(MAKE) -C $(SRC) clean
	[ -e lcd4linux/Makefile ] && (cd lcd4linux; make distclean) || true

distclean:
	$(MAKE) -C $(DPFLIB) distclean
	$(MAKE) -C $(PYDPF) clean
	$(MAKE) -C $(TOOLS) distclean
	$(MAKE) -C $(SRC) distclean
	[ -e lcd4linux ] && (rm -rf lcd4linux) || true


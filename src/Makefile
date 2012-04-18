# Makefile for source code running on the DPFmate
#
# 12/2010, <hackfin@section5.ch>
#
# 0.202devel 2012/04/01 by superelchi
#	Added support for ProgSPI flashing
# 0.203devel 2012/04/17 by superelchi
#	Added new model
#

# Specify here the model you wish to compile for. See dpfmodel.h for valid
# names.

ifndef TYPE
TYPE = white
endif

#
# Set custom flag
#
ifeq ($(TYPE),linkdelight)
LCD_CUSTOM = 1
endif

ifdef FLIX
COMPILE_OPTIONS = -DFLIX_MODE
LNK_OPTIONS = "-b FLIX = 0x6c1200"
else
LNK_OPTIONS = ";"
endif


ifndef NETPP
NETPP = $(HOME)/src/netpp/devices/dpf-ax206
endif

DPFLIB = ../dpflib
PYDPF  = ../python

# Firmware location in flash:
# Even if those addresses exceed some flash sizes: Due to the mirroring
# feature, they appear to work on smaller flashes as well.

FW_ADDR      = 0x000000
LOAD_ADDR    = 0x1140 # Target prog mem address for executable
JMPTBL_OFFS  = 0x248   # Jump table offset           --> BootHeader.jmpt_offset
CODE_OFFS    = 0x400  # Offset of code start sector --> BootHeader.flash_start
# Character table location in flash:
CHARTBL_ADDR = $(FW_ADDR) + 0x10000

VERSION = 0.203
STATUS  = devel

# Build number file:
PROG_BUILDNO = .buildno

BUILDNO = $(shell cat $(PROG_BUILDNO))

MEMMODEL = small

CC = sdcc
AS = asx8051
LD = aslink

ASFLAGS  = -olsxffg
CFLAGS   = -mmcs51 --model-$(MEMMODEL) # --Werror
CFLAGS  += --opt-code-size
# CFLAGS += --no-xinit-opt # Do not initialize XSEG
CFLAGS  += -I../include

CFLAGS  += $(COMPILE_OPTIONS)

# Feel free to add stuff, but note: Order matters!
OBJS = main.rel  # ihx file will be named after first element
OBJS += menu.rel ovldata.rel
OBJS += bankswitch.rel
# OBJS += boothdr.rel
OBJS += bootstrap.rel lcd.rel
ifdef LCD_CUSTOM
OBJS += lcdblit_$(TYPE).rel lcdinit_$(TYPE).rel lcdseto_$(TYPE).rel
endif
OBJS += printex.rel
OBJS += usbblit.rel 
OBJS += appload.rel
OBJS += usbaux.rel usb_ep1.rel
OBJS += irqh.rel irq.rel
OBJS += init.rel debug.rel
OBJS += blit.rel print.rel 
OBJS += flash.rel properties.rel
ifdef FLIX
OBJS += flix.rel
endif
# OBJS += app_test.rel

LIBS = dpf.lib
ifndef LCD_CUSTOM
LIBS += lcd.lib
endif

CFLAGS += -DDPFMODEL_$(TYPE)
ifeq ($(STATUS),devel)
	CFLAGS += -DBUILD_ID='$(TYPE)-$(VERSION)devel\#$(BUILDNO)'
else
	CFLAGS += -DBUILD_ID='$(TYPE)-$(VERSION)$(STATUS)'
endif

# Some shared overlay data area definitions:
# CFLAGS += -DOVLDATA=0x1180
 
PATCHES = $(wildcard p_*.s) $(wildcard jmptbl*.s)
APPLETS = $(wildcard app_*.s)

HANDLER = cmdhandler_14e5.ihx

TARGET += $(HANDLER)
TARGET += $(PATCHES:%.s=%.ihx) $(APPLETS:%.s=%.ihx)

-include local.mk

HEADERS += config.inc

all: $(PROG_BUILDNO) $(HEADERS) fwload Debug/dpf.so $(DUTIES) current.bin

include rules.mk

fwload:
	$(MAKE) -C $(DPFLIB) install DESTDIR=..
	[ -e $(PYDPF) ] && \
	$(MAKE) -C $(PYDPF) install

mod%.ihx: mod%.rel
	$(CC) -o $@ -Wl-bBANK0=0x1330 $<

# Patches (ABS)
p_%.ihx: p_%.rel
	$(CC) -o $@ $< $(ADDR_OPTS)

app%.ihx: app%.rel
	$(CC) -o $@ $<

# This actually compiles the whole habash. Also creates a .bin for the
# full firmware.
fw_$(TYPE).ihx: main.ihx
	python compile.py $< $@ $(FW_ADDR) $(LOAD_ADDR) \
		$(JMPTBL_OFFS) $(CODE_OFFS) > output.log

current.ihx: fw_$(TYPE).ihx 
	ln -sf $< $@

fw_$(TYPE).bin: fw_$(TYPE).ihx
	@touch $@ # Hack to update current.bin link

current.bin: fw_$(TYPE).bin
	ln -sf $< $@

app: app_test.ihx

TMPLNK = tmp.lnk

config.inc: Makefile
	@echo "; This file is generated from the Makefile" > $@
	@echo >> $@
	@echo "jumptable_offset = $(FW_ADDR) + $(JMPTBL_OFFS)" >> $@
	@echo "load_offset = $(FW_ADDR) + $(CODE_OFFS)" >> $@
	
bankswitch.rel: bankswitch.s config.inc
	$(AS) $(ASFLAGS) $<

main.ihx: $(OBJS) $(LIBS) bootstrap.lnk Makefile $(BUILDNO)
	@cp bootstrap.lnk $(TMPLNK)
	@echo $(LNK_OPTIONS) >> $ $(TMPLNK)
	@echo "-g chartbl_offs = $(CHARTBL_ADDR)" >> $(TMPLNK)
	@for i in $(OBJS); do \
		echo $$i >> $(TMPLNK); \
	done
	@echo -l dpf >> $(TMPLNK)
	@echo -l lcd >> $(TMPLNK)
	# End:
	@echo -e >> $(TMPLNK)
	@echo "Linking.."
	@$(LD) -f $(TMPLNK) -n > /dev/null

font4x8.bin: font4x8.pnm
	python chartbl.py font4x8

Debug/dpf.so: $(wildcard $(DPFLIB)/*.c) $(PYDPF)/py_device.c
	$(MAKE) -C $(DPFLIB) all
	$(MAKE) -C $(PYDPF) install

%.ihx: %.rel
	$(CC) -o $@ $<

RES128 = -Wl-gscreen_resx=128 -Wl-gscreen_resy=128

# Legacy command handler - we only build this one
cmdhandler_14e5.ihx: cmdhandler.rel
-       $(CC) -o $@ $(COM_CMD) $(RES128) -Wl-bENTRY=0x14e5 $<


HEXFILES += $(wildcard jmptbl_*.ihx)
HEXFILES += $(wildcard p_start_*.ihx)
HEXFILES += $(wildcard fw_*.ihx)

HEXDEST = hexfiles

hexfiles:
	mkdir hexfiles

install: all hexfiles
	cp $(HEXFILES) hexfiles

%.hex: %.bin
	hexdump -C $< > $@

%.inc: $(NETPP)/%.inc
	cp $< .

%.h: $(NETPP)/%.h
	cp $< .

headers: ax206.h ax206.inc ili9163.h ili9320.h otm3225.h otm3225.inc 

fwclean:
	rm -f $(OBJS)
	rm -f fw_*.ihx

libclean::

clean: libclean
	# $(MAKE) -C $(DPFLIB) clean
	# $(MAKE) -C $(PYDPF) clean
	rm -f fwload *.a *.d
	rm -fr *.ihx *.rel *.o *.asm
	rm -f *.mem *.sym *.map *.lst *.rst
	rm -f *.raw
	rm -f current.bin
	-rm config.inc

$(PROG_BUILDNO):
	echo "1" > $(PROG_BUILDNO)

reset:
	echo "1" > $(PROG_BUILDNO)

# Put DPF into bootloader mode by pressing RESET while holding
# the MENU button. It will then register as device with VID:PID = 1908:3318
# (see lsusb)

download:
#	sudo libhid-detach-device 1908:3318
	cd ../fw ; sudo ./hiddetach ; sudo python restore.py ../src/current.bin -f

# Make it phony, because we want to update the build version every time.
.PHONY: printex.c

$(BUILDNO): Makefile
	@if ! test -f $(PROG_BUILDNO); then echo 0 > $(PROG_BUILDNO); fi
	@echo $$(($$(cat $(PROG_BUILDNO)) + 1)) > $(PROG_BUILDNO)
	@echo Incrementing build number to $$(cat $(PROG_BUILDNO))


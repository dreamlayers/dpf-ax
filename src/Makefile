# Makefile for source code running on the DPFmate
#
# 12/2010, <hackfin@section5.ch>
#

# Specify here the model you wish to compile for. See dpfmodel.h for valid
# names.

ifndef TYPE
TYPE = white
endif

DPFLIB = ../dpflib
PYDPF  = ../python

# Firmware location in flash:
# Even if those addresses exceed some flash sizes: Due to the mirroring
# feature, they appear to work on smaller flashes as well.

FW_ADDR      = 0x380000
# Character table location in flash:
CHARTBL_ADDR = $(FW_ADDR) + 0x10000
JMPTBL_ADDR  = $(FW_ADDR) + 0x70000

VERSION = 0.107
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

# Feel free to add stuff, but note: Order matters!
OBJS = main.rel  # ihx file will be named after first element
OBJS += menu.rel ovldata.rel
OBJS += bootstrap.rel lcd.rel
OBJS += printex.rel
OBJS += usbblit.rel 
OBJS += appload.rel
OBJS += usbhandler.rel usbaux.rel
OBJS += irqh.rel irq.rel
OBJS += init.rel debug.rel
OBJS += blit.rel print.rel 
OBJS += flash.rel properties.rel
OBJS += flix.rel
# OBJS += app_test.rel

LIBS = dpf.lib lcd.lib

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

all: $(PROG_BUILDNO) $(HEADERS) fwload Debug/dpf.so $(DUTIES) current.ihx

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

fw_$(TYPE).ihx: main.ihx
	python compile.py $< $@ $(FW_ADDR) > output.log

current.ihx: fw_$(TYPE).ihx 
	ln -sf $< $@

app: app_test.ihx

TMPLNK = tmp.lnk

config.inc: Makefile
	@echo "; This file is generated from the Makefile" > $@
	@echo >> $@
	@echo "cloned_jumptable_offset = $(JMPTBL_ADDR) + 0x88" >> $@
	@echo "jumptable_offset = $(FW_ADDR) + 0x88" >> $@
	@echo "load_offset = $(FW_ADDR) + 0x200" >> $@
	
bankswitch.rel: bankswitch.s config.inc
	$(AS) $(ASFLAGS) $<

main.ihx: $(OBJS) $(LIBS) bootstrap.lnk Makefile $(BUILDNO)
	@cp bootstrap.lnk $(TMPLNK)
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


%.inc: $(NETPP)/%.inc
	cp $< .

%.h: $(NETPP)/%.h
	cp $< .

fwclean:
	rm -f $(OBJS)
	rm -f fw_*.ihx

libclean::

clean: libclean
	$(MAKE) -C $(DPFLIB) clean
	$(MAKE) -C $(PYDPF) clean
	rm -f fwload *.a *.d
	rm -fr *.ihx *.rel *.o *.asm
	rm -f *.mem *.sym *.map *.lst *.rst
	rm -f *.raw
	-rm config.inc

$(PROG_BUILDNO):
	echo "1" > $(PROG_BUILDNO)

reset:
	echo "1" > $(PROG_BUILDNO)


# Make it phony, because we want to update the build version every time.
.PHONY: printex.c

$(BUILDNO): Makefile
	@if ! test -f $(PROG_BUILDNO); then echo 0 > $(PROG_BUILDNO); fi
	@echo $$(($$(cat $(PROG_BUILDNO)) + 1)) > $(PROG_BUILDNO)
	@echo Incrementing build number to $$(cat $(PROG_BUILDNO))


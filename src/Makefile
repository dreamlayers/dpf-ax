# Makefile for source code running on the DPFmate
#
# 12/2010, <hackfin@section5.ch>
#
# 0.202devel 2012/04/01 by superelchi
#	Added support for ProgSPI flashing
# 0.203devel 2012/04/17 by superelchi
#	Added new model
# 0.204devel 2012/04/20 by superelchi
#	Changed detection of custom models
# 0.300 2012/05/18 by superelchi
#	Changes for "disp" and "devel" firmware
#

-include ./Make.config

VERSION = 0.312
ifdef BUILD_DEVEL
  STATUS  = devel
else
  STATUS  = disp
endif

#
# Set build type & type specific resoures
#
ifndef TYPE
  TYPE = $(DEFAULT_TYPE)
endif

#
# Set controller type
#
LCD_CONTROLLER=$(shell grep -EA4 DPFMODEL_$(TYPE)\\W config.h | grep -Eo LCD_CONTROLLER_\\w+)
RES=$(shell grep -EA4 DPFMODEL_$(TYPE)\\W config.h | grep -Eo LCD_[0-9]+x[0-9]+)
RESX=$(shell echo $(RES) | sed -r "s/LCD_([0-9]+)x([0-9]+)/\1/")
RESY=$(shell echo $(RES) | sed -r "s/LCD_([0-9]+)x([0-9]+)/\2/")

#
# Set spash image
#
ifndef BUILD_DEVEL
  SPLASH_FILE=res/splash.$(RESX)x$(RESY).rgb
endif

LNK_OPTIONS = ";"

ifdef BUILD_DEVEL
  BOOTSTRAP_LNK=bootstrap_dev.lnk
    ifdef FLIX
    COMPILE_OPTIONS = -DFLIX_MODE
    LNK_OPTIONS = "-b FLIX = 0x6c1200"
    endif
else
  BOOTSTRAP_LNK=bootstrap_dsp.lnk
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
CHARTBL4x8_ADDR = $(FW_ADDR) + 0x10000
CHARTBL9x16_ADDR = $(FW_ADDR) + 0x20000
SPLASH_ADDR = $(FW_ADDR) + 0x30000

# Build number file:
PROG_BUILDNO = .buildno

BUILDNO = $(shell cat $(PROG_BUILDNO))

MEMMODEL = small

SDCC_INCLUDE := $(shell pwd) 

CC = sdcc
AS = asx8051
LD = aslink

ASFLAGS  = -olsxffg
CFLAGS   = -mmcs51 --model-$(MEMMODEL) # --Werror
CFLAGS  += --opt-code-size
# CFLAGS += --no-xinit-opt # Do not initialize XSEG
CFLAGS  += -I../include

CFLAGS  += $(COMPILE_OPTIONS)

ifeq ($(LCD_CONTROLLER), LCD_CONTROLLER_CUSTOM)
    LCD_OBJS = lcd/$(TYPE)/lcdblit.rel lcd/$(TYPE)/lcdinit.rel lcd/$(TYPE)/lcdsetor.rel
endif
DEVEL_OBJS = ovldata.rel lcd_devel.rel menu.rel printex.rel blit.rel debug.rel
DISP_OBJS = ovlstr.rel
DISP_OBJS += xmenu.rel xmenustr.rel
DISP_OBJS += xmenu0data.rel xmenu1data.rel xmenu2data.rel xmenu2handler.rel
DISP_OBJS += xmenu3data.rel xmenu4data.rel xmenu5data.rel xmenu5handler.rel
DISP_OBJS += splash.rel usbdesc.rel initconf.rel blitchar.rel blitimg.rel


# Feel free to add stuff, but note: Order matters!
OBJS = main.rel  # ihx file will be named after first element
OBJS += bankswitch.rel
# OBJS += boothdr.rel
OBJS += bootstrap.rel lcd.rel lcdadj.rel
OBJS += $(LCD_OBJS)
OBJS += usbblit.rel 
OBJS += appload.rel
OBJS += usbaux.rel usb_ep1.rel
OBJS += irqh.rel irq.rel
OBJS += init.rel
OBJS += print.rel 
OBJS += flash.rel properties.rel
ifdef BUILD_DEVEL
    OBJS += $(DEVEL_OBJS)
    ifdef FLIX
        OBJS += flix.rel
    endif
else
    OBJS += $(DISP_OBJS)
endif
# OBJS += app_test.rel

LIBS = dpf.lib lcd.lib

CFLAGS += -DDPFMODEL_$(TYPE)
CFLAGS += -DDEFAULT_SPLASH=$(DEFAULT_SPLASH)

ifdef BUILD_DEVEL
  CFLAGS += -DBUILD_DEVEL
endif

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

HEADERS += config.inc userconfig.h

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
fw_$(STATUS)_$(TYPE).ihx: main.ihx
	python compile.py $< $@ $(FW_ADDR) $(LOAD_ADDR) \
		$(JMPTBL_OFFS) $(CODE_OFFS) $(RESX) $(RESY) $(SPLASH_FILE) > output.log

current.ihx: fw_$(STATUS)_$(TYPE).ihx 
	ln -sf $< $@

fw_$(STATUS)_$(TYPE).bin: fw_$(STATUS)_$(TYPE).ihx
	@touch $@ # Hack to update current.bin link

current.bin: fw_$(STATUS)_$(TYPE).bin
	ln -sf $< $@

app: app_test.ihx

TMPLNK = tmp.lnk

config.inc: Makefile Make.config
	@echo "; This file is generated from the Makefile" > $@
	@echo >> $@
	@echo "jumptable_offset = $(FW_ADDR) + $(JMPTBL_OFFS)" >> $@
	@echo "load_offset = $(FW_ADDR) + $(CODE_OFFS)" >> $@
ifdef BUILD_DEVEL
	@echo "BUILD_DEVEL = 1" >> $@
endif

userconfig.h: Makefile Make.config
	@echo "// This file is generated from the Makefile" > $@
	@echo >> $@
ifdef CUSTOM_MENU_COLOR
	@echo "#define MENU_COLOR RGB565($(CUSTOM_MENU_COLOR))" >> $@
endif
ifdef CUSTOM_MENU_BGCOLOR
	@echo "#define MENU_BGCOLOR RGB565($(CUSTOM_MENU_BGCOLOR))" >> $@
endif
	
bankswitch.rel: bankswitch.s config.inc
	$(AS) $(ASFLAGS) $<

main.ihx: $(OBJS) $(LIBS) $(BOOTSTRAP_LNK) Makefile Make.config $(BUILDNO)
	@cp $(BOOTSTRAP_LNK) $(TMPLNK)
	@echo $(LNK_OPTIONS) >> $ $(TMPLNK)
	@echo "-g chartbl4x8_offs = $(CHARTBL4x8_ADDR)" >> $(TMPLNK)
ifndef BUILD_DEVEL
	@echo "-g chartbl9x16_offs = $(CHARTBL9x16_ADDR)" >> $(TMPLNK)
	@echo "-g splash_offs = $(SPLASH_ADDR)" >> $(TMPLNK)
endif
	@for i in $(OBJS); do \
		echo $$i >> $(TMPLNK); \
	done
	@echo -l dpf >> $(TMPLNK)
	@echo -l lcd >> $(TMPLNK)
	# End:
	@echo -e >> $(TMPLNK)
	@echo "Linking.."
	@$(LD) -f $(TMPLNK) -n > /dev/null

#font4x8.bin: font4x8.pnm
#	python chartbl.py font4x8

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

distclean: fwclean clean
	rm -f $(PROG_BUILDNO)
	rm -f output.log
	rm -f tmp.lnk

fwclean:
	rm -f $(OBJS)
	rm -f fw_*.ihx
	rm -f fw_*.bin

libclean::

clean: libclean
	# $(MAKE) -C $(DPFLIB) clean
	# $(MAKE) -C $(PYDPF) clean
	rm -f fwload *.a *.d
	rm -fr *.ihx *.rel *.o *.asm
	rm -f *.mem *.sym *.map *.lst *.rst
	rm -fr lcd/**/*.sym lcd/**/*.lst lcd/**/*.rst lcd/**/*.rel
	rm -f *.raw
	rm -f current.bin
	-rm userconfig.h config.inc

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


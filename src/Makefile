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

DPFLIB = ../dpflib
PYDPF  = ../python

-include ./Make.config

VERSION = $(shell cat ../version)
ifdef BUILD_DEVEL
  DEVEL_  = devel_
endif

#
# Set build type & type specific resoures
#
ifndef TYPE
  TYPE = $(DEFAULT_TYPE)
endif

#
# Set controller type + display with / height
#
LCD_CONTROLLER:=$(shell cat lcd/$(TYPE)/dpfmodel.h | grep -Eo LCD_CONTROLLER_\\w+)
RESX:=$(shell grep -Eo "LCD_WIDTH\s+[0-9]+" lcd/$(TYPE)/dpfmodel.h | sed -r "s/LCD_WIDTH\s+([0-9]+)/\1/")
RESY:=$(shell grep -Eo "LCD_HEIGHT\s+[0-9]+" lcd/$(TYPE)/dpfmodel.h | sed -r "s/LCD_HEIGHT\s+([0-9]+)/\1/")

#
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

MEMMODEL = small

SDCC_INCLUDE := $(shell pwd) 

CC = sdcc
SDCC_VERSION:=$(shell sdcc -v | grep -Eo "[0-9]\.[0-9]\.[0-9]" | cut -c1)
ifeq ($(SDCC_VERSION), 3)
AS = sdas8051
LD = sdld
$(error sdcc 3.x not supported - use sdcc 2.9x or cc1111)
else
AS = asx8051
LD = aslink
endif

AS = asx8051
LD = aslink

ASFLAGS  = -olsxffg
CFLAGS   = -mmcs51 --model-$(MEMMODEL) # --Werror
ifeq ($(SDCC_VERSION), 3)
    CFLAGS   += --disable-warning 197
endif
CFLAGS  += --opt-code-size
# CFLAGS += --no-xinit-opt # Do not initialize XSEG
CFLAGS  += -I../include -I./lcd/$(TYPE)

CFLAGS  += $(COMPILE_OPTIONS)

#
# See if lcd uses contrasttables
#
ifeq ($(LCD_CONTROLLER), LCD_CONTROLLER_CUSTOM)
  HAS_CTBL:=$(shell grep -e "^_custom_contrasttbl::" lcd/$(TYPE)/lcdinit.s)
  ifdef HAS_CTBL
    CFLAGS += -DLCD_HAS_CONTRASTTABLE
  endif
endif

ifeq ($(LCD_CONTROLLER), LCD_CONTROLLER_CUSTOM)
    LCD_OBJS = lcd/$(TYPE)/lcdblit.rel lcd/$(TYPE)/lcdinit.rel lcd/$(TYPE)/lcdsetor.rel
endif

DEVEL_OBJS = ovldata.rel lcd_devel.rel menu.rel printex.rel blit.rel debug.rel
DISP_OBJS = ovlstr.rel
DISP_OBJS += xmenu.rel xmenuhook.rel
DISP_OBJS += xmenu0_main.rel xmenu1_splash.rel xmenu2_backlight.rel xmenu2handler.rel
DISP_OBJS += xmenu3_info.rel xmenu4_setup.rel xmenu5_usbser.rel xmenu5handler.rel
DISP_OBJS += xmenu6_testscr.rel xmenu7_contrast.rel xmenu7handler.rel
DISP_OBJS += splash.rel usbdesc.rel initconf.rel blitchar.rel blitimg.rel


# Feel free to add stuff, but note: Order matters!
OBJS = main.rel  # ihx file will be named after first element
OBJS += bankswitch.rel
# OBJS += boothdr.rel
OBJS += bootstrap.rel lcd.rel lcdadj.rel lcdhelper.rel
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

LIBS = dpf.lib

CFLAGS += -DDPFMODEL=$(TYPE)
CFLAGS += -DDEFAULT_SPLASH=$(DEFAULT_SPLASH)

ifdef BUILD_DEVEL
  CFLAGS += -DBUILD_DEVEL
else
  ifdef DEBUG_DEV
    CFLAGS += -DDEBUG_DEV
  endif
endif

ifdef NO_BATTERY_CHECK
  CFLAGS += -DNO_BATTERY_CHECK
endif

ifdef $(DEVEL_)
  CFLAGS += -DBUILD_ID='$(TYPE)-$(VERSION)devel'
else
  CFLAGS += -DBUILD_ID='$(TYPE)-$(VERSION)'
endif

-include local.mk

HEADERS += config.inc userconfig.h

all: $(HEADERS) current.bin

include rules.mk

# This actually compiles the whole habash. Also creates a .bin for the
# full firmware.
fw_$(DEVEL_)$(TYPE).ihx: main.ihx
	python compile.py $< $@ $(FW_ADDR) $(LOAD_ADDR) \
		$(JMPTBL_OFFS) $(CODE_OFFS) $(RESX) $(RESY) $(SPLASH_FILE) > output.log

current.ihx: fw_$(DEVEL_)$(TYPE).ihx 
	ln -sf $< $@

fw_$(DEVEL_)$(TYPE).bin: fw_$(DEVEL_)$(TYPE).ihx
	@touch $@ # Hack to update current.bin link

current.bin: fw_$(DEVEL_)$(TYPE).bin
	ln -sf $< $@

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

main.ihx: $(OBJS) $(LIBS) $(BOOTSTRAP_LNK) Makefile Make.config
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
	# End:
	@echo -e >> $(TMPLNK)
	@echo "Linking.."
	@$(LD) -f $(TMPLNK) -n > /dev/null

#font4x8.bin: font4x8.pnm
#	python chartbl.py font4x8


%.ihx: %.rel
	$(CC) -o $@ $<

RES128 = -Wl-gscreen_resx=128 -Wl-gscreen_resy=128

##HEXFILES += $(wildcard fw_*.ihx)

##HEXDEST = hexfiles

##hexfiles:
##	mkdir hexfiles

##install: all hexfiles
##	cp $(HEXFILES) hexfiles

headers: ax206.h ax206.inc ili9163.h ili9320.h otm3225.h otm3225.inc 

distclean: fwclean clean
	rm -f output.log
	rm -f tmp.lnk

fwclean:
	rm -f $(OBJS)
	rm -f fw_*.ihx
	rm -f fw_*.bin

libclean::

clean: libclean
	rm -f fwload *.a *.d
	rm -fr *.ihx *.rel *.o *.asm
	rm -f *.mem *.sym *.map *.lst *.rst
	rm -fr lcd/**/*.sym lcd/**/*.lst lcd/**/*.rst lcd/**/*.rel
	rm -f *.raw
	rm -f current.bin
	rm -f userconfig.h config.inc

Debug/dpf.so: $(wildcard $(DPFLIB)/*.c) $(PYDPF)/py_device.c
	$(MAKE) -C $(DPFLIB) install
	$(MAKE) -C $(PYDPF) install

# Put DPF into bootloader mode by pressing RESET while holding
# the MENU button. It will then register as device with VID:PID = 1908:3318
# (see lsusb)

download: Debug/dpf.so 
#	sudo libhid-detach-device 1908:3318
	cd ../tools ; sudo ./hiddetach ; sudo python restore.py ../src/current.bin -f



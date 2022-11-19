# Makefile for DPF firmware analysis


CFLAGS = -g

ASFLAGS = -plosgff

ifndef DUMP
	DUMP = -d
endif

VERSION = 0.01develop

AS = asx8051

INFILES = $(wildcard code*.in) $(wildcard dump*.in) $(wildcard data*.in)

ASMFILES = $(INFILES:%.in=%.asm)
BINFILES = $(INFILES:%.in=%.bin)

all: $(ASMFILES)

show:
	echo $(ASMFILES)

all.asm: $(ASMFILES)
	@echo concatenating all ASM files into $@
	@cat $(ASMFILES) > $@

%.rel : %.asm
	$(AS) $(ASFLAGS) $<

# Fixup format:
%.asm: %.d52
	@echo Generating $@
	@sed 's/\.equ/=/g;s/^\s*end/; END/g' $< > $@

test: main.o
	$(CC) -o $@ $<

clean: cleanimages
	rm -f test *.ctl *.asm dump*.bin code*.bin *.d52 *.lst *.rel *.sym

common.in:
	ln -s ../common.in .

dump%.ctl: dump%.in common.in
	@cat $< common.in >$@

code_%.ctl: code_%.in common.in
	@cat $< common.in >$@

data_%.ctl: data_%.in
	@cp $< $@

%.d52: %.bin %.ctl
	@d52 -p -b $(DUMP) -n $< # >/dev/null

cleanimages:
	rm -fr images/*

RAWFILES = $(wildcard *.raw)

IMAGES = $(RAWFILES:%.raw=images/%.png)

images/%.png: %.raw
	convert -size 128x128 -depth 1 gray:$< $@

images: $(IMAGES)

dump:
	PWD=`pwd`; BASE=`basename $$PWD`; \
	echo $$BASE; \
	python ../dump.py $$BASE full_image.bin

.PHONY: dump

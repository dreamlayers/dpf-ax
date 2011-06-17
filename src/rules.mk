.SUFFIXES: .rel .ihx

%.asm: %.d52
	@echo Generating $@
	@sed 's/\.equ/=/g;s/^\s*end/; END/g;s/\.org/; \.org/g' $< > $@

%.rel: %.s
	$(AS) $(ASFLAGS) $<

%.rel: %.c
	$(CC) -M $(CFLAGS) $< > $(<:.c=.d)
	$(CC) $(CFLAGS) -c $<


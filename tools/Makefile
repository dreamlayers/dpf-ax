DPFLIB = ../dpflib
PYDPF = ../python

default: Debug/dpf.so

Debug/dpf.so: $(wildcard $(DPFLIB)/*.c) $(PYDPF)/py_dpf.c
	$(MAKE) -C $(DPFLIB) install
	$(MAKE) -C $(PYDPF) install

distclean: clean
	rm -rf identify.out
	rm -f *.pyc
	rm -f fulldump*.bin

clean:

OBJS = dpflib.o rawusb.o scsi.o bootload.o

ifndef PLATFORM
PLATFORM=$(shell "uname")
endif

ifeq (Linux, $(PLATFORM))
LIBS=-lusb
else
LIBS=-lusb -lsgutils2
endif

CFLAGS = -Wall -fPIC -I../include -g
# CFLAGS += -DDEBUG

ifndef DESTDIR
	DESTDIR = ..
endif

all: libdpf.a fwload bload

fwload: fwload.o libdpf.a
	$(CC) -o $@ fwload.o -L. -ldpf $(LIBS)

bload.o: bootload.c
	$(CC) -c -o $@ $(CFLAGS) -DTEST $<

bload: bload.o
	$(CC) -o $@ $< -L. -ldpf $(LIBS)

testusb: testusb.o libdpf.a
	$(CC) -o $@ testusb.o -L. -ldpf $(LIBS)

libdpf.a: $(OBJS)
	ar ruv $@ $(OBJS)

clean:
	-rm -f fwload libdpf.a *.o
	-rm -f $(DESTDIR)/fwload $(DESTDIR)/libdpf.a
	-rm -f bload testusb

distclean: clean

install: all
	cp fwload libdpf.a $(DESTDIR)

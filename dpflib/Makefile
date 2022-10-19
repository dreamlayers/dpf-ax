OBJS = dpflib.o rawusb.o scsi.o bootload.o

CFLAGS  := -Wall -fPIC -I../include -g $(shell pkg-config --cflags libusb-1.0)
LDFLAGS := -ldpf $(shell pkg-config --libs libusb-1.0)
# CFLAGS += -DDEBUG

ifndef DESTDIR
	DESTDIR = ..
endif

all: libdpf.a fwload bload

fwload: fwload.o libdpf.a
	$(CC) -o $@ fwload.o -L. $(LDFLAGS)

bload.o: bootload.c
	$(CC) -c -o $@ $(CFLAGS) -DTEST $<

bload: bload.o
	$(CC) -o $@ $< -L. $(LDFLAGS)

testusb: testusb.o libdpf.a
	$(CC) -o $@ testusb.o -L. $(LDFLAGS)

libdpf.a: $(OBJS)
	ar ruv $@ $(OBJS)

clean:
	-rm -f fwload libdpf.a *.o
	-rm -f $(DESTDIR)/fwload $(DESTDIR)/libdpf.a
	-rm -f bload testusb

distclean: clean

install: all
	cp fwload libdpf.a $(DESTDIR)

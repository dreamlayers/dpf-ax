OBJS = dpflib.o rawusb.o

CFLAGS = -Wall -fPIC -I../include -g

ifndef DESTDIR
	DESTDIR = ..
endif

all: libdpf.a fwload

fwload: fwload.o libdpf.a
	$(CC) -o $@ fwload.o -L. -ldpf -lusb

testusb: testusb.o libdpf.a
	$(CC) -o $@ testusb.o -L. -ldpf -lusb

libdpf.a: $(OBJS)
	ar ruv $@ $(OBJS)

clean:
	-rm -f fwload libdpf.a *.o
	-rm -f $(DESTDIR)/fwload $(DESTDIR)/libdpf.a

install: all
	cp fwload libdpf.a $(DESTDIR)

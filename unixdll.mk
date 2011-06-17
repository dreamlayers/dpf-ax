#
# Common makefile to create a unix .so
#
# $Id: unixdll.mk 292 2010-01-07 19:35:21Z strubi $
#

OPTIMIZATIONS = -finline-functions #-O2

CXXFLAGS = $(INCLUDES)  $(OPTIMIZATIONS)
CXXFLAGS += $(DLLFLAGS) 
CXXFLAGS += -Wall -MD

CFLAGS = $(INCLUDES)  $(OPTIMIZATIONS) 
CFLAGS += $(DLLFLAGS) 
CFLAGS += -Wall -MD
CFLAGS += -fPIC

ifdef RELEASE
	CONFIG = Release
else
	CONFIG = Debug
endif

ifndef RELEASE
	CFLAGS += $(DEBUGOPTS)
endif

OBJDIR = $(CONFIG)

ifndef PREFIX
	PREFIX = .
endif

DLLCOMMONOBJS = $(DLLCOMMONSRCS:%.c=$(OBJDIR)/%.o)
DLLCPPOBJS = $(DLLCPPSRCS:%.cpp=$(OBJDIR)/%.o)
DLLOBJS = $(DLLCSRCS:%.c=$(OBJDIR)/%.o) $(DLLCOMMONOBJS)
DLLOBJS += $(DLLCPPOBJS)

ifndef TOPDIR
	TOPDIR = ..
endif

ifndef DLLINSTALLDIR
	DLLINSTALLDIR = $(TOPDIR)/$(CONFIG)
endif

#
# Please don't change this: 
ifndef DLLTYPE
	DLLTYPE = $(LIBNAME)
endif

DLL = $(OBJDIR)/lib$(LIBNAME).so
STATICLIB = $(OBJDIR)/lib$(LIBNAME).a
DLLEXT = .so

ifndef INSTALLBASE
	INSTALLNAME = $(DLLTYPE)
else
	INSTALLNAME = $(LIBNAME)
endif

############################################################################
# RULES
#

$(OBJDIR)/%.o : %.cpp
	$(CXX) -o $@  -c $< $(CXXFLAGS) 

$(OBJDIR)/%.o : %.c
	$(CC) -o $@  -c $< $(CFLAGS) 

$(OBJDIR)/%.o: $(PREFIX)/%.c
	$(CC) -o $@ -c $< $(CFLAGS) 

$(OBJDIR)/%.o: $(COMMONSRCDIR)/%.c
	$(CC) -o $@ -c $< $(CFLAGS) 

dll: $(DLL)
	
$(DLL): dirs $(DLLOBJS)
	$(CC) -o $(DLL) -shared $(DLLOBJS) $(DLLDEPS)


# Make DLL directory, if it does not exist
$(OBJDIR):
	[ -e $(OBJDIR) ] || mkdir $(OBJDIR)

dirs: $(OBJDIR)

$(STATICLIB): dirs $(DLLOBJS)
	$(AR) ruv $(STATICLIB) $(DLLOBJS)
	
libclean:
	rm -f $(DLL) $(STATICLIB)

installdll:: $(DLL)
	[ -e $(DLLINSTALLDIR) ] || mkdir $(DLLINSTALLDIR)
	cp $(DLL) $(DLLINSTALLDIR)/lib$(INSTALLNAME).so

clean::
	rm -f $(DLLOBJS)
	rm -fr Debug/ Release/
	rm -f *.d
	rm -fr $(TOPDIR)/Debug
	rm -fr $(TOPDIR)/Release

#
# File: Makefile
# Date: 27-Nov-2019
# By  : Kevin Esteb
#
# Makefile to build the jam libraries.
#
MAJOR=1
MINOR=0
PATCH=0
VERSION="$(MAJOR).$(MINOR).$(PATCH)"
#
CC = gcc
LINK = gcc
LIBR = ar rs
LIBS = ../../libs/
WORKING = $(shell pwd)
STATIC = libqwk.a
CFLAGS = -DVERSION=\"$(VERSION)\" -Wall -c -I.. -I../../../files -I../../../errors -I../../../tracer -I../../../include -I../../../cclibs -I../../../objects -I../../../gpl
LOPTS = -o 
SOPTS = -shared -o 
SFLAGS = -L$(WORKING) -Wl,-rpath=$(WORKING) -fPIC 
#
OLBS = $(STATIC) ../../../libs/libtracer.a ../../../libs/liberrors.a ../../../libs/libfiles.a ../../../libs/libobjects.a ../../../libs/libcclibs.a ../../../libs/libgpl.a 
#
INCS = qwk.h 
#
OBJS = qwk.o 
#
all: $(OBJS)
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f $(LIBS)libqwk.a
	rm -f $(LIBS)libqwk.so.*
	rm -f qwk.3
#
qwk.o: qwk.c $(INCS)
	$(CC) $(CFLAGS) qwk.c
	$(LIBR) $(LIBS)libqwk.a qwk.o
#
man:
	pod2man -c " " -r "qwk(3)" -s 3 qwk.pod qwk.3
#
shared:
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) qwk.o qwk.c
	$(LINK) $(SOPTS) $(LIBS)libqwk.so.$(VERSION) qwk.o
#
# eof
#

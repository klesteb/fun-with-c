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
STATIC = libjam.a
CFLAGS = -DVERSION=\"$(VERSION)\" -Wall -c -I.. -I../../../files -I../../../errors -I../../../tracer -I../../../include -I../../../cclibs -I../../../objects -I../../../gpl
LOPTS = -o 
SOPTS = -shared -o 
SFLAGS = -L$(WORKING) -Wl,-rpath=$(WORKING) -fPIC 
#
OLBS = $(STATIC) ../../../libs/libtracer.a ../../../libs/liberrors.a ../../../libs/libfiles.a ../../../libs/libobjects.a ../../../libs/libcclibs.a ../../../libs/libgpl.a 
#
INCS = jam.h 
#
OBJS = jam.o 
#
all: $(OBJS)
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f $(LIBS)libjam.a
	rm -f $(LIBS)libjam.so.*
	rm -f jam.3
#
jam.o: jam.c $(INCS)
	$(CC) $(CFLAGS) jam.c
	$(LIBR) $(LIBS)libjam.a jam.o
#
man:
	pod2man -c " " -r "jam(3)" -s 3 jam.pod jam.3
#
shared:
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) jam.o jam.c
	$(LINK) $(SOPTS) $(LIBS)libjam.so.$(VERSION) jam.o
#
# eof
#

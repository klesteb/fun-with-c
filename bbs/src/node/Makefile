#
# File: Makefile
# Date: 04-Jan-2021
# By  : Kevin Esteb
#
# Makefile to build the node libraries.
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
STATIC = libnode.a
CFLAGS = -DVERSION=\"$(VERSION)\" -Wall -c -I.. -I../../../rms -I../../../files -I../../../errors -I../../../tracer -I../../../include -I../../../cclibs -I../../../objects -I../../../gpl
LOPTS = -o 
SOPTS = -shared -o 
SFLAGS = -L$(WORKING) -Wl,-rpath=$(WORKING) -fPIC 
#
OLBS = $(STATIC) ../../../libs/libtracer.a ../../../libs/liberrors.a ../../../libs/librms.a../../../libs/libfiles.a  ../../../libs/libobjects.a ../../../libs/libcclibs.a ../../../libs/libgpl.a 
#
INCS = node.h 
#
OBJS = node.o 
#
all: $(OBJS)
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f $(LIBS)libnode.a
	rm -f $(LIBS)libnode.so.*
	rm -f node.3
#
node.o: node.c $(INCS)
	$(CC) $(CFLAGS) node.c
	$(LIBR) $(LIBS)libnode.a node.o
#
man:
	pod2man -c " " -r "node(3)" -s 3 node.pod node.3
#
shared:
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) node.o node.c
	$(LINK) $(SOPTS) $(LIBS)libnode.so.$(VERSION) node.o
#
# eof
#

#
# File: Makefile
# Date: 27-Nov-2019
# By  : Kevin Esteb
#
# Makefile to build the room libraries.
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
STATIC = libroom.a
CFLAGS = -DVERSION=\"$(VERSION)\" -Wall -c -I.. -I../../../files -I../../../errors -I../../../tracer -I../../../include -I../../../cclibs -I../../../objects -I../../../gpl
LOPTS = -o 
SOPTS = -shared -o 
SFLAGS = -L$(WORKING) -Wl,-rpath=$(WORKING) -fPIC 
#
OLBS = $(STATIC) ../../../libs/libtracer.a ../../../libs/liberrors.a ../../../libs/libfiles.a ../../../libs/libobjects.a ../../../libs/libcclibs.a ../../../libs/libgpl.a 
#
INCS = room.h 
#
OBJS = room.o 
#
all: $(OBJS)
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f $(LIBS)libroom.a
	rm -f $(LIBS)libroom.so.*
	rm -f room.3
#
room.o: room.c $(INCS)
	$(CC) $(CFLAGS) room.c
	$(LIBR) $(LIBS)libroom.a room.o
#
man:
	pod2man -c " " -r "room(3)" -s 3 room.pod room.3
#
shared:
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) room.o room.c
	$(LINK) $(SOPTS) $(LIBS)libroom.so.$(VERSION) room.o
#
# eof
#

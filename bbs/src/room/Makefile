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
INCS = room.h handler.h msgs.h finds.h
#
OBJS = room.o handler.o msgs.o finds.o
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
	rm -f msgs.3
	rm -f finds.3
	rm -f handler.3
#
room.o: room.c $(INCS)
	$(CC) $(CFLAGS) room.c
	$(LIBR) $(LIBS)libroom.a room.o
#
handler.o: handler.c $(INCS)
	$(CC) $(CFLAGS) handler.c
	$(LIBR) $(LIBS)libroom.a handler.o
#
msgs.o: msgs.c $(INCS)
	$(CC) $(CFLAGS) msgs.c
	$(LIBR) $(LIBS)libroom.a msgs.o
#
finds.o: finds.c $(INCS)
	$(CC) $(CFLAGS) finds.c
	$(LIBR) $(LIBS)libroom.a finds.o
#
man:
	pod2man -c " " -r "room(3)" -s 3 room.pod room.3
	pod2man -c " " -r "msgs(3)" -s 3 msgs.pod msgs.3
	pod2man -c " " -r "finds(3)" -s 3 finds.pod finds.3
	pod2man -c " " -r "handler(3)" -s 3 handler.pod handler.3
#
shared:
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) room.o room.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) msgs.o msgs.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) finds.o finds.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) handler.o handler.c
	$(LINK) $(SOPTS) $(LIBS)libroom.so.$(VERSION) room.o msgs.o finds.o handler.o
#
# eof
#

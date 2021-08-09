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
LIBS = ../libs/
WORKING = $(shell pwd)
STATIC = libroom.a
INCLUDE = -I ../../../
CFLAGS = -DVERSION=\"$(VERSION)\" -Wall -c $(INCLUDE)
LOPTS = -o 
SOPTS = -shared -o 
SFLAGS = -L$(WORKING) -Wl,-rpath=$(WORKING) -fPIC 
#
OLBS = $(STATIC) ../../../libs/libtracer.a ../../../libs/liberrors.a ../../../libs/librms.a ../../../libs/libfiles.a ../../../libs/libobjects.a ../../../libs/libcclibs.a ../../../libs/libgpl.a 
#
INCS = room.h handler.h msgs.h 
#
OBJS = room.o handler.o msgs.o 
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
	rm -f *.3
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
#room_status.o: room_status.c $(INCS)
#	$(CC) $(CFLAGS) room_status.c
#	$(LIBR) $(LIBS)libroom.a room_status.o
#
man:
	pod2man -c " " -r "room(3)" -s 3 room.pod room.3
	pod2man -c " " -r "msgs(3)" -s 3 msgs.pod msgs.3
	pod2man -c " " -r "handler(3)" -s 3 handler.pod handler.3
#	pod2man -c " " -r "room status(3)" -s 3 room_status.pod room_status.3
#
shared:
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) room.o room.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) msgs.o msgs.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) handler.o handler.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) room_status.o rom_status.c
	$(LINK) $(SOPTS) $(LIBS)libroom.so.$(VERSION) room.o msgs.o handler.o room_status.o
#
# eof
#

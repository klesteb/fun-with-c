#
# File: Makefile
# Date: 28-Jan-2021
# By  : Kevin Esteb
#
# Makefile to build the finds libraries.
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
STATIC = libfinds.a
INCLUDES = -I ../../../
CFLAGS = -DVERSION=\"$(VERSION)\" -Wall -c $(INCLUDES)
LOPTS = -o 
SOPTS = -shared -o 
SFLAGS = -L$(WORKING) -Wl,-rpath=$(WORKING) -fPIC 
#
OLBS = $(STATIC) ../../../libs/libtracer.a ../../../libs/liberrors.a ../../../libs/librms.a ../../../libs/libfiles.a ../../../libs/libobjects.a ../../../libs/libcclibs.a ../../../libs/libgpl.a 
#
INCS = finds.h ../room/room.h ../user/user.h ../node/node.h ../jam/jam.h
#
OBJS = rooms.o users.o nodes.o msgs.o
#
all: $(OBJS)
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f $(LIBS)libfinds.a
	rm -f $(LIBS)libfinds.so.*
	rm -f *.3
#
rooms.o: rooms.c $(INCS)
	$(CC) $(CFLAGS) rooms.c
	$(LIBR) $(LIBS)libfinds.a rooms.o
#
users.o: users.c $(INCS)
	$(CC) $(CFLAGS) users.c
	$(LIBR) $(LIBS)libfinds.a users.o
#
nodes.o: nodes.c $(INCS)
	$(CC) $(CFLAGS) nodes.c
	$(LIBR) $(LIBS)libfinds.a nodes.o
#
msgs.o: msgs.c $(INCS)
	$(CC) $(CFLAGS) msgs.c
	$(LIBR) $(LIBS)libfinds.a msgs.o
#
man:
	pod2man -c " " -r "finds(3)" -s 3 finds.pod finds.3
#
shared:
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) rooms.o rooms.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) users.o users.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) nodes.o nodes.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) msgs.o msgs.c
	$(LINK) $(SOPTS) $(LIBS)libfinds.so.$(VERSION) rooms.o users.o nodes.o msgs.o
#
# eof
#

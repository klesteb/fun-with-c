#
# File: Makefile
# Date: 11-Aug-2021
# By  : Kevin Esteb
#
# Makefile to build the door libraries.
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
STATIC = libdoor.a
INCLUDE = -I ../../../
CFLAGS = -DVERSION=\"$(VERSION)\" -Wall -c $(INCLUDE)
LOPTS = -o 
SOPTS = -shared -o 
SFLAGS = -L$(WORKING) -Wl,-rpath=$(WORKING) -fPIC 
#
OLBS = $(STATIC) ../../../libs/librms.a ../../../libs/libtracer.a ../../../libs/liberrors.a ../../../libs/libfiles.a ../../../libs/libobjects.a ../../../libs/libcclibs.a ../../../libs/libgpl.a 
#
INCS = door.h 
#
OBJS = door.o 
#
all: $(OBJS)
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f $(LIBS)libdoor.a
	rm -f $(LIBS)libdoor.so.*
	rm -f *.3
#
door.o: door.c $(INCS)
	$(CC) $(CFLAGS) door.c
	$(LIBR) $(LIBS)libdoor.a door.o
#
man:
	pod2man -c " " -r "door(3)" -s 3 door.pod door.3
#
shared:
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) door.o door.c
	$(LINK) $(SOPTS) $(LIBS)libdoor.so.$(VERSION) door.o
#
# eof
#

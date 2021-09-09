#
# File: Makefile
# Date: 08-Apr-2021
# By  : Kevin Esteb
#
# Makefile to build the rms libraries.
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
STATIC = librms.a
INCLUDE = -I ../
CFLAGS = -DVERSION=\"$(VERSION)\" -Wall -c $(INCLUDE)
LOPTS = -o 
SOPTS = -shared -o 
SFLAGS = -L$(WORKING) -Wl,-rpath=$(WORKING) -fPIC 
#
OLBS = $(STATIC) ../libs/libtracer.a ../libs/liberrors.a ../libs/libfiles.a ../libs/libobjects.a ../libs/libcclibs.a ../libs/libgpl.a 
#
INCS = rms.h index.h
#
OBJS = rms.o index.o
#
all: $(OBJS)
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f $(LIBS)librms.a
	rm -f $(LIBS)librms.so.*
	rm -f *.3
#
rms.o: rms.c $(INCS)
	$(CC) $(CFLAGS) rms.c
	$(LIBR) $(LIBS)librms.a rms.o
#
index.o: index.c $(INCS)
	$(CC) $(CFLAGS) index.c
	$(LIBR) $(LIBS)librms.a index.o
#
man:
	pod2man -c " " -r "rms(3)" -s 3 rms.pod rms.3
	pod2man -c " " -r "index(3)" -s 3 index.pod index.3
#
shared:
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) rms.o rms.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) index.o index.c
	$(LINK) $(SOPTS) $(LIBS)librms.so.$(VERSION) $(OBJS)
#
# eof
#

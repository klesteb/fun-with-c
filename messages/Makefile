#
# File: Makefile
# Date: 21-Nov-2019
# By  : Kevin Esteb
#
# Makefile to build messages.
#
MAJOR=1
MINOR=0
PATCH=0
VERSION="$(MAJOR).$(MINOR).$(PATCH)"
#
CC = gcc
LINK = gcc
LIBR = ar rs
WORKING = $(shell pwd)
STATIC = ../libs/libmessage.a
SHARED = libmessage.so.$(VERSION)
CFLAGS = -DVERSION=\"$(VERSION)\" -Wall -c -I../errors -I../tracer -I../include -I../cclibs -I../objects -I../gpl
LOPTS = -o 
SOPTS = -shared -o 
SFLAGS = -L$(WORKING) -Wl,-rpath=$(WORKING) -fPIC 
#
OLBS = $(STATIC) ../libs/libobjects.a ../libs/libgpl.a ../libs/libcclibs.a 
#
INCS = message.h
#
OBJS = message.o
#
all: $(OBJS) test1
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f $(STATIC)
	rm -f $(SHARED)
	rm -f message.3
	rm -f test1
#
#
test1: test1.o
	$(CC) $(CFLAGS) test1.c
	$(LINK) $(LOPTS) test1 test1.o $(OLBS)
#
message.o: message.c $(INCS)
	$(CC) $(CFLAGS) message.c
	$(LIBR) $(STATIC) message.o
#
man:
	pod2man -c " " -r "message(3)" -s 3 message.pod message.3
#
shared:
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) message.o message.c
	$(LINK) $(SOPTS) $(SHARED) message.o
#
# eof
#

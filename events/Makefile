#
# File: Makefile
# Date: 25-Jan-2021
# By  : Kevin Esteb
#
# Makefile to build events.
#
MAJOR=0
MINOR=1
PATCH=0
VERSION="$(MAJOR).$(MINOR).$(PATCH)"
#
CC = gcc
LINK = gcc
LIBR = ar rs
WORKING = $(shell pwd)
STATIC = ../libs/libevents.a
SHARED = libevents.so.$(VERSION)
CFLAGS = -DVERSION=\"$(VERSION)\" -Wall -c -I../errors -I../tracer -I../include -I../cclibs -I../objects -I../gpl
LOPTS = -o 
SOPTS = -shared -o 
SFLAGS = -L$(WORKING) -Wl,-rpath=$(WORKING) -fPIC 
#
OLBS = $(STATIC) ../libs/libobjects.a ../libs/libgpl.a ../libs/libcclibs.a 
#
INCS = events.h
#
OBJS = events.o
#
all: $(OBJS) test1 test2 test3 test4 test5 test6
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f $(STATIC)
	rm -f $(SHARED)
	rm -f event.3
	rm -f test1
	rm -f test2
	rm -f test3
	rm -f test4
	rm -f test5
	rm -f test6
#
test1: test1.o
	$(CC) $(CFLAGS) test1.c
	$(LINK) $(LOPTS) test1 test1.o $(OLBS)
#
test2: test2.o
	$(CC) $(CFLAGS) test2.c
	$(LINK) $(LOPTS) test2 test2.o $(OLBS)
#
test3: test3.o
	$(CC) $(CFLAGS) test3.c
	$(LINK) $(LOPTS) test3 test3.o $(OLBS)
#
test4: test4.o
	$(CC) $(CFLAGS) test4.c
	$(LINK) $(LOPTS) test4 test4.o $(OLBS)
#
test5: test5.o
	$(CC) $(CFLAGS) test5.c
	$(LINK) $(LOPTS) test5 test5.o $(OLBS)
#
test6: test6.o
	$(CC) $(CFLAGS) test6.c
	$(LINK) $(LOPTS) test6 test6.o $(OLBS)
#
events.o: events.c $(INCS)
	$(CC) $(CFLAGS) events.c
	$(LIBR) $(STATIC) events.o
#
man:
	pod2man -c " " -r "events(3)" -s 3 events.pod events.3
#
shared:
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) events.o events.c
	$(LINK) $(SOPTS) $(SHARED) events.o
#
# eof
#

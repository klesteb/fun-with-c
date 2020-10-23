#
# File: Makefile
# Date: 02-Oct-2020
# By  : Kevin Esteb
#
# Makefile to build files.
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
STATIC = ../libs/libfiles.a
SHARED = libfiles.so.$(VERSION)
CFLAGS = -DVERSION=\"$(VERSION)\" -Wall -c -I../errors -I../tracer -I../include -I../cclibs -I../objects -I../gpl
LOPTS = -o 
SOPTS = -shared -o 
SFLAGS = -L$(WORKING) -Wl,-rpath=$(WORKING) -fPIC 
#
OLBS = files.o ../libs/libobjects.a ../libs/libgpl.a ../libs/libcclibs.a 
#
INCS = files.h
#
OBJS = files.o
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
	rm -f files.3
	rm -f test1
	rm -f test2
	rm -f test3
	rm -f test4
	rm -f test5
	rm -f test6
#
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
files.o: files.c $(INCS)
	$(CC) $(CFLAGS) files.c
	$(LIBR) $(STATIC) files.o
#
man:
	pod2man -c " " -r "files(3)" -s 3 files.pod files.3
#
shared:
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) files.o files.c
	$(LINK) $(SOPTS) $(SHARED) files.o
#
# eof
#

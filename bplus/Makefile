#
# File: Makefile
# Date: 27-Nov-2019
# By  : Kevin Esteb
#
# Makefile to build bplus.
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
STATIC = ../libs/libbplus.a
SHARED = ../libs/libbplus.so.$(VERSION)
INCLUDE = -I ../
CFLAGS = -DVERSION=\"$(VERSION)\" -Wall -c $(INCLUDE)
LOPTS = -o 
SOPTS = -shared -o 
SFLAGS = -L$(WORKING) -Wl,-rpath=$(WORKING) -fPIC 
#
OLBS = ../libs/libcclibs.a $(STATIC) 
#
INCS = bplus.h
#
OBJS = bplus.o 
#
all: $(OBJS) listtree names reindex vtest
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f $(STATIC)
	rm -f $(SHARED)
	rm -f *.3
	rm -f listtree names reindex vtest
#
listtree: listtree.o
	$(CC) $(CFLAGS) listtree.c
	$(LINK) $(LOPTS) listtree listtree.o $(OLBS)
#
names: names.o
	$(CC) $(CFLAGS) names.c
	$(LINK) $(LOPTS) names names.o $(OLBS)
#
reindex: reindex.o
	$(CC) $(CFLAGS) reindex.c
	$(LINK) $(LOPTS) reindex reindex.o $(OLBS)
#
vtest: vtest.o
	$(CC) $(CFLAGS) vtest.c
	$(LINK) $(LOPTS) vtest vtest.o $(OLBS)
#
bplus.o: bplus.c $(INCS)
	$(CC) $(CFLAGS) bplus.c
	$(LIBR) $(STATIC) bplus.o
#
man:
	pod2man -c " " -r "bplus(3)" -s 3 bplus.pod bplus.3
#
shared:
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) bplus.o bplus.c
	$(LINK) $(SOPTS) $(SHARED) $(OBJS)
#
# eof
#

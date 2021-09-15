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
WORKING = $(shell pwd)
STATIC = ../libs/librms.a
SHARED = librms.so.$(VERSION)
INCLUDE = -I ../
CFLAGS = -DVERSION=\"$(VERSION)\" -Wall -c $(INCLUDE)
LOPTS = -o 
SOPTS = -shared -o 
SFLAGS = -L$(WORKING) -Wl,-rpath=$(WORKING) -fPIC 
#
OLBS = $(STATIC) ../libs/libtracer.a ../libs/liberrors.a ../libs/libobjects.a ../libs/libcclibs.a ../libs/libgpl.a 
#
INCS = rms.h btree.h files.h
#
OBJS = rms.o btree.o files.o 
#
all: $(OBJS) test1 test2 test3 test4 test5 test6 test7 test8
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f $(STATIC)
	rm -f $(SHARED)
	rm -f *.3
	rm -f test1
	rm -f test2
	rm -f test3
	rm -f test4
	rm -f test5
	rm -f test6
	rm -f test7
	rm -f test8
#
rms.o: rms.c $(INCS)
	$(CC) $(CFLAGS) rms.c
	$(LIBR) $(STATIC) rms.o
#
btree.o: btree.c $(INCS)
	$(CC) $(CFLAGS) btree.c
	$(LIBR) $(STATIC) btree.o
#
files.o: files.c $(INCS)
	$(CC) $(CFLAGS) files.c
	$(LIBR) $(STATIC) files.o
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
test7: test7.o
	$(CC) $(CFLAGS) test7.c
	$(LINK) $(LOPTS) test7 test7.o $(OLBS)
#
test8: test8.o
	$(CC) $(CFLAGS) test8.c
	$(LINK) $(LOPTS) test8 test8.o $(OLBS)
#
man:
	pod2man -c " " -r "rms(3)" -s 3 rms.pod rms.3
	pod2man -c " " -r "btree(3)" -s 3 btree.pod btree.3
	pod2man -c " " -r "files(3)" -s 3 files.pod files.3
#
shared:
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) rms.o rms.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) btree.o btree.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) files.o files.c
	$(LINK) $(SOPTS) $(SHARED) $(OBJS)
#
# eof
#

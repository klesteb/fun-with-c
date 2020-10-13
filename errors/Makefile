#
# File: Makefile
# Date: 09-Sep-2019
# By  : Kevin Esteb
#
# Makefile to build errors.
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
STATIC = ../libs/liberrors.a
SHARED = liberrors.so.$(VERSION)
CFLAGS = -DVERSION=\"$(VERSION)\" -Wall -c -I../errors -I../tracer -I../include -I../cclibs -I../objects -I../gpl
LOPTS = -o 
SOPTS = -shared -o 
SFLAGS = -L$(WORKING) -Wl,-rpath=$(WORKING) -fPIC 
#
OLBS = $(STATIC) ../libs/libobjects.a ../libs/libgpl.a ../libs/libcclibs.a 
#
INCS = errors.h ../include/error_codes.h
#
OBJS = errors.o 
#
all: $(OBJS) test1 test2
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f $(STATIC)
	rm -f $(SHARED)
	rm -f template.3
	rm -f test1
	rm -f test2
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
errors.o: errors.c $(INCS)
	$(CC) $(CFLAGS) errors.c
	$(LIBR) $(STATIC) errors.o
#
errnoname.o: errnoname.c $(INCS)
	$(CC) $(CFLAGS) errnoname.c
	$(LIBR) $(STATIC) errnoname.o
#
man:
	pod2man -c " " -r "errors(3)" -s 3 errors.pod errors.3
#
shared:
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) errors.o errors.c
	$(LINK) $(SOPTS) $(SHARED) errors.o
#
# eof
#

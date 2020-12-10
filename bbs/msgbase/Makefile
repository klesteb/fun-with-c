#
# File: Makefile
# Date: 27-Nov-2019
# By  : Kevin Esteb
#
# Makefile to build the msgbase libraries.
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
STATIC = libjam.a libqwk.a
CFLAGS = -DVERSION=\"$(VERSION)\" -Wall -c -I../../files -I../../errors -I../../tracer -I../../include -I../../cclibs -I../../objects -I../../gpl
LOPTS = -o 
SOPTS = -shared -o 
SFLAGS = -L$(WORKING) -Wl,-rpath=$(WORKING) -fPIC 
#
OLBS = $(STATIC) ../../libs/libtracer.a ../../libs/liberrors.a ../../libs/libfiles.a ../../libs/libobjects.a ../../libs/libcclibs.a ../../libs/libgpl.a 
#
INCS = jam.h qwk.h
#
OBJS = jam.o qwk.o
#
all: $(OBJS) test1 test2 test3 test4 test5 test6 test7 test8 test9 \
             test10 test11 test12 test13 test14 test15
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f libjam.a
	rm -f libjam.so.*
	rm -f libqwk.a
	rm -f libqwk.so.*
	rm -f jam.3
	rm -f qwk.3
	rm -f test1
	rm -f test2
	rm -f test3
	rm -f test4
	rm -f test5
	rm -f test6
	rm -f test7
	rm -f test8
	rm -f test9
	rm -f test10
	rm -f test11
	rm -f test12
	rm -f test13
	rm -f test14
	rm -f test15
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
test9: test9.o
	$(CC) $(CFLAGS) test9.c
	$(LINK) $(LOPTS) test9 test9.o $(OLBS)
#
test10: test10.o
	$(CC) $(CFLAGS) test10.c
	$(LINK) $(LOPTS) test10 test10.o $(OLBS)
#
test11: test11.o
	$(CC) $(CFLAGS) test11.c
	$(LINK) $(LOPTS) test11 test11.o $(OLBS)
#
test12: test12.o
	$(CC) $(CFLAGS) test12.c
	$(LINK) $(LOPTS) test12 test12.o $(OLBS)
#
test13: test13.o
	$(CC) $(CFLAGS) test13.c
	$(LINK) $(LOPTS) test13 test13.o $(OLBS)
#
test14: test14.o
	$(CC) $(CFLAGS) test14.c
	$(LINK) $(LOPTS) test14 test14.o $(OLBS)
#
test15: test15.o
	$(CC) $(CFLAGS) test15.c
	$(LINK) $(LOPTS) test15 test15.o $(OLBS)
#
#
jam.o: jam.c $(INCS)
	$(CC) $(CFLAGS) jam.c
	$(LIBR) libjam.a jam.o
#
qwk.o: qwk.c $(INCS)
	$(CC) $(CFLAGS) qwk.c
	$(LIBR) libqwk.a qwk.o
#
man:
	pod2man -c " " -r "jam(3)" -s 3 jam.pod jam.3
	pod2man -c " " -r "qwk(3)" -s 3 qwk.pod qwk.3
#
shared:
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) jam.o jam.c
	$(LINK) $(SOPTS) libjam.so.$(VERSION) jam.o
#
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) qwk.o qwk.c
	$(LINK) $(SOPTS) libqwk.so.$(VERSION) qwk.o
#
# eof
#

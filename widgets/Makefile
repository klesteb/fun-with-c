#
# File: Makefile
# Date: 05-Feb-2021
# By  : Kevin Esteb
#
# Makefile to build widgets.
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
STATIC = libwidgets.a
SHARED = libwidgets.so.$(VERSION)
CFLAGS = -DVERSION=\"$(VERSION)\" -Wall -c -I../errors -I../tracer -I../include -I../cclibs -I../objects -I../gpl
LOPTS = -o 
SOPTS = -shared -o 
SFLAGS = -L$(WORKING) -Wl,-rpath=$(WORKING) -fPIC 
#
OLBS = $(STATIC) ../libs/libobjects.a ../libs/libgpl.a ../libs/libcclibs.a 
#
INCS = widget.h colors.h window.h component.h
#
OBJS = widget.o colors.o window.o component.o
#
all: $(OBJS) test1 test2 test3 test4 test5
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f $(STATIC)
	rm -f $(SHARED)
	rm -f widgets.3
	rm -f test1
	rm -f test2
	rm -f test3
	rm -f test4
	rm -f test5
#
test1: test1.o
	$(CC) $(CFLAGS) test1.c
	$(LINK) $(LOPTS) test1 test1.o $(OLBS) -lncurses
#
test2: test2.o
	$(CC) $(CFLAGS) test2.c
	$(LINK) $(LOPTS) test2 test2.o $(OLBS) -lncurses
#
test3: test3.o
	$(CC) $(CFLAGS) test3.c
	$(LINK) $(LOPTS) test3 test3.o $(OLBS) -lncurses
#
test4: test4.o
	$(CC) $(CFLAGS) test4.c
	$(LINK) $(LOPTS) test4 test4.o $(OLBS) -lncurses
#
test5: test5.o
	$(CC) $(CFLAGS) test5.c
	$(LINK) $(LOPTS) test5 test5.o $(OLBS) -lncurses
#
colors.o: colors.c $(INCS)
	$(CC) $(CFLAGS) colors.c
	$(LIBR) $(STATIC) colors.o
#
widget.o: widget.c $(INCS)
	$(CC) $(CFLAGS) widget.c
	$(LIBR) $(STATIC) widget.o
#
window.o: window.c $(INCS)
	$(CC) $(CFLAGS) window.c
	$(LIBR) $(STATIC) window.o
#
container.o: container.c $(INCS)
	$(CC) $(CFLAGS) container.c
	$(LIBR) $(STATIC) container.o
#
component.o: component.c $(INCS)
	$(CC) $(CFLAGS) component.c
	$(LIBR) $(STATIC) component.o
#
man:
	pod2man -c " " -r "widget(3)" -s 3 widget.pod widget.3
#
shared:
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) widget.o widget.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) colors.o colors.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) window.o window.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) component.o component.c
	$(LINK) $(SOPTS) $(SHARED) $(OBJS)
#
# eof
#

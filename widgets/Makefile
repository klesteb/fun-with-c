#
# File: Makefile
# Date: 05-Feb-2021
# By  : Kevin Esteb
#
# Makefile to build widgets.
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
STATIC = libwidgets.a
SHARED = libwidgets.so.$(VERSION)
CFLAGS = -DVERSION=\"$(VERSION)\" -Wall -c -I. -I../errors -I../tracer -I../include -I../cclibs -I../objects -I../gpl
LOPTS = -o 
SOPTS = -shared -o 
SFLAGS = -L$(WORKING) -Wl,-rpath=$(WORKING) -fPIC 
#
OLBS = $(STATIC) ../libs/libobjects.a ../libs/libgpl.a ../libs/libcclibs.a 
#
INCS = widget.h colors.h window.h component.h components/label.h \
       components/hline.h components/vline.h components/text.h \
       components/prompt.h components/textarea.h
#
OBJS = widget.o colors.o window.o component.o components/label.o \
       components/hline.o components/vline.o components/text.o \
       components/prompt.o components/textarea.o
#
all: $(OBJS) test1 test2 test3 test4 test5 test6 test7 test8 test9 test10
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f components/*.o
	rm -f components/*~
	rm -f $(STATIC)
	rm -f $(SHARED)
	rm -f widgets.3
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
test6: test6.o
	$(CC) $(CFLAGS) test6.c
	$(LINK) $(LOPTS) test6 test6.o $(OLBS) -lncurses
#
test7: test7.o
	$(CC) $(CFLAGS) test7.c
	$(LINK) $(LOPTS) test7 test7.o $(OLBS) -lncurses
#
test8: test8.o
	$(CC) $(CFLAGS) test8.c
	$(LINK) $(LOPTS) test8 test8.o $(OLBS) -lncurses
#
test9: test9.o
	$(CC) $(CFLAGS) test9.c
	$(LINK) $(LOPTS) test9 test9.o $(OLBS) -lncurses
#
test10: test10.o
	$(CC) $(CFLAGS) test10.c
	$(LINK) $(LOPTS) test10 test10.o $(OLBS) -lncurses
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
components/label.o: components/label.c $(INCS) 
	$(CC) $(CFLAGS) $(LOPTS) components/label.o components/label.c 
	$(LIBR) $(STATIC) components/label.o
#
components/hline.o: components/hline.c $(INCS) 
	$(CC) $(CFLAGS) $(LOPTS) components/hline.o components/hline.c 
	$(LIBR) $(STATIC) components/hline.o
#
components/vline.o: components/vline.c $(INCS) 
	$(CC) $(CFLAGS) $(LOPTS) components/vline.o components/vline.c 
	$(LIBR) $(STATIC) components/vline.o
#
components/text.o: components/text.c $(INCS) 
	$(CC) $(CFLAGS) $(LOPTS) components/text.o components/text.c 
	$(LIBR) $(STATIC) components/text.o
#
components/prompt.o: components/prompt.c $(INCS) 
	$(CC) $(CFLAGS) $(LOPTS) components/prompt.o components/prompt.c 
	$(LIBR) $(STATIC) components/prompt.o
#
components/textarea.o: components/textarea.c $(INCS) 
	$(CC) $(CFLAGS) $(LOPTS) components/textarea.o components/textarea.c 
	$(LIBR) $(STATIC) components/textarea.o
#
man:
	pod2man -c " " -r "widget(3)" -s 3 widget.pod widget.3
#
shared:
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) widget.o widget.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) colors.o colors.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) window.o window.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) component.o component.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) components/label.o components/label.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) components/hline.o components/hline.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) components/vline.o components/vline.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) components/text.o components/text.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) components/prompt.o components/prompt.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) components/textarea.o components/textarea.c
	$(LINK) $(SOPTS) $(SHARED) $(OBJS)
#
# eof
#

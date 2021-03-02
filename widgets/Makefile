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
CFLAGS = -DVERSION=\"$(VERSION)\" -Wall -c -I. -I../events -I../errors -I../tracer -I../include -I../cclibs -I../objects -I../gpl
LOPTS = -o 
SOPTS = -shared -o 
SFLAGS = -L$(WORKING) -Wl,-rpath=$(WORKING) -fPIC 
#
OLBS = $(STATIC) ../libs/libtracer.a ../libs/libevent.a ../libs/liberrors.a ../libs/libobjects.a ../libs/libgpl.a ../libs/libcclibs.a 
#
INCS = widget.h colors.h window.h component.h components/label.h \
       components/hline.h components/vline.h components/text.h \
       components/prompt.h components/textarea.h components/input.h \
       windows/menus/menus.h windows/menus/bar.h windows/menus/box.h \
       windows/menus/list.h workbench.h
#
OBJS = widget.o colors.o window.o component.o components/label.o \
       components/hline.o components/vline.o components/text.o \
       components/prompt.o components/textarea.o components/input.o \
       windows/menus/menus.o windows/menus/bar.o windows/menus/box.o \
       windows/menus/list.o workbench.o
#
all: $(OBJS) test1 test2 test3 test4 test5 test6 test7 test8 test9 \
             test10 test11 test12 test13
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f components/*.o
	rm -f components/*~
	rm -f windows/menus/*.o
	rm -f windows/menus/*~
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
	rm -f test11
	rm -f test12
	rm -f test13
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
test11: test11.o
	$(CC) $(CFLAGS) test11.c
	$(LINK) $(LOPTS) test11 test11.o $(OLBS) -lncurses -lmenu
#
test12: test12.o
	$(CC) $(CFLAGS) test12.c
	$(LINK) $(LOPTS) test12 test12.o $(OLBS) -lncurses -lmenu
#
test13: test13.o
	$(CC) $(CFLAGS) test13.c
	$(LINK) $(LOPTS) test13 test13.o $(OLBS) -lpanel  -lncurses -lmenu
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
workbench.o: workbench.c $(INCS)
	$(CC) $(CFLAGS) workbench.c
	$(LIBR) $(STATIC) workbench.o
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
components/input.o: components/input.c $(INCS) 
	$(CC) $(CFLAGS) $(LOPTS) components/input.o components/input.c 
	$(LIBR) $(STATIC) components/input.o
#
windows/menus/menus.o: windows/menus/menus.c $(INCS) 
	$(CC) $(CFLAGS) $(LOPTS) windows/menus/menus.o windows/menus/menus.c 
	$(LIBR) $(STATIC) windows/menus/menus.o
#
windows/menus/box.o: windows/menus/box.c $(INCS) 
	$(CC) $(CFLAGS) $(LOPTS) windows/menus/box.o windows/menus/box.c 
	$(LIBR) $(STATIC) windows/menus/box.o
#
windows/menus/bar.o: windows/menus/bar.c $(INCS) 
	$(CC) $(CFLAGS) $(LOPTS) windows/menus/bar.o windows/menus/bar.c 
	$(LIBR) $(STATIC) windows/menus/bar.o
#
windows/menus/list.o: windows/menus/list.c $(INCS) 
	$(CC) $(CFLAGS) $(LOPTS) windows/menus/list.o windows/menus/list.c 
	$(LIBR) $(STATIC) windows/menus/list.o
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
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) components/input.o components/input.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) menus/menus.o menus/menus.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) menus/bar.o menus/bar.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) menus/box.o menus/box.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) menus/list.o menus/list.c
	$(LINK) $(SOPTS) $(SHARED) $(OBJS)
#
# eof
#

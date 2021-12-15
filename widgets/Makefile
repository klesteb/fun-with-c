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
STATIC = ../libs/libwidgets.a
SHARED = ../libs/libwidgets.so.$(VERSION)
INCLUDE = -I ../
CFLAGS = -DVERSION=\"$(VERSION)\" -Wall -c $(INCLUDE)
LOPTS = -o 
SOPTS = -shared -o 
SFLAGS = -L$(WORKING) -Wl,-rpath=$(WORKING) -fPIC 
#
OLBS = $(STATIC) ../libs/libtracer.a ../libs/libevent.a ../libs/liberrors.a ../libs/libobjects.a ../libs/libgpl.a ../libs/libcclibs.a 
#
INCS = keys.h widget.h colors.h window.h component.h components/label.h \
       components/hline.h components/vline.h components/text.h \
       components/prompt.h components/textarea.h components/input.h \
       components/bell.h components/menus/menus.h components/menus/bar.h \
       components/menus/box.h components/menus/list.h components/more.h \
       components/yesno.h components/forms/forms.h \
       workbench.h windows/alert.h windows/status.h windows/bar_menu.h \
       windows/box_menu.h windows/list_menu.h windows/base_window.h \
       windows/query_window.h
#
OBJS = widget.o colors.o window.o component.o components/label.o \
       components/hline.o components/vline.o components/text.o \
       components/prompt.o components/textarea.o components/input.o \
       components/bell.o components/menus/menus.o components/menus/bar.o \
       components/menus/box.o components/menus/list.o components/more.o \
       components/yesno.o components/forms/form_field.o \
       components/forms/form_header.o components/forms/form_label.o \
       components/forms/form_seperator.o components/forms/form_textares.o \
       workbench.o windows/alert.o windows/status.o windows/bar_menu.o \
       windows/box_menu.o windows/list_menu.o windows/base_window.o \
       windows/query_window.o
#
all: $(OBJS) test1 test2 test3 test4 test5 test6 test7 test8 test9 \
             test10 test11 test12 test13 test14 test15 test16 test17 \
             test18 test19 test20
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f components/*.o
	rm -f components/*~
	rm -f components/menus/*.o
	rm -f components/menus/*~
	rm -f components/forms/*.o
	rm -f components/forms/*~
	rm -f windows/*.o
	rm -f windows/*~
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
	rm -f test14
	rm -f test15
	rm -f test16
	rm -f test17
	rm -f test18
	rm -f test19
	rm -f test20
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
	$(LINK) $(LOPTS) test11 test11.o $(OLBS) -lmenu -lncurses 
#
test12: test12.o
	$(CC) $(CFLAGS) test12.c
	$(LINK) $(LOPTS) test12 test12.o $(OLBS) -lmenu -lncurses 
#
test13: test13.o
	$(CC) $(CFLAGS) test13.c
	$(LINK) $(LOPTS) test13 test13.o $(OLBS) -lpanel -lmenu -lncurses 
#
test14: test14.o
	$(CC) $(CFLAGS) test14.c
	$(LINK) $(LOPTS) test14 test14.o $(OLBS) -lpanel -lmenu -lncurses 
#
test15: test15.o
	$(CC) $(CFLAGS) test15.c
	$(LINK) $(LOPTS) test15 test15.o $(OLBS) -lpanel -lmenu -lncurses 
#
test16: test16.o
	$(CC) $(CFLAGS) test16.c
	$(LINK) $(LOPTS) test16 test16.o $(OLBS) -lpanel -lmenu -lncurses 
#
test17: test17.o
	$(CC) $(CFLAGS) test17.c
	$(LINK) $(LOPTS) test17 test17.o $(OLBS) -lpanel -lmenu -lncurses 
#
test18: test18.o
	$(CC) $(CFLAGS) test18.c
	$(LINK) $(LOPTS) test18 test18.o $(OLBS) -lpanel -lmenu -lncurses 
#
test19: test19.o
	$(CC) $(CFLAGS) test19.c
	$(LINK) $(LOPTS) test19 test19.o $(OLBS) -lpanel -lmenu -lncurses 
#
test20: test20.o
	$(CC) $(CFLAGS) test20.c
	$(LINK) $(LOPTS) test20 test20.o $(OLBS) -lpanel -lmenu -lncurses 
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
components/bell.o: components/bell.c $(INCS) 
	$(CC) $(CFLAGS) $(LOPTS) components/bell.o components/bell.c 
	$(LIBR) $(STATIC) components/bell.o
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
components/more.o: components/more.c $(INCS) 
	$(CC) $(CFLAGS) $(LOPTS) components/more.o components/more.c 
	$(LIBR) $(STATIC) components/more.o
#
components/yesno.o: components/yesno.c $(INCS) 
	$(CC) $(CFLAGS) $(LOPTS) components/yesno.o components/yesno.c 
	$(LIBR) $(STATIC) components/yesno.o
#
components/forms/form_field.o: components/forms/form_field.c $(INCS)
	$(CC) $(CFLAGS) $(LFLAGS) components/forms/form_field.o components/forms/form_field.c
	$(LIBR) $(STATIC) components/forms/form_field.o
#
components/forms/form_label.o: components/forms/form_label.c $(INCS)
	$(CC) $(CFLAGS) $(LFLAGS) components/forms/form_label.o components/forms/form_label.c
	$(LIBR) $(STATIC) components/forms/form_label.o
#
components/forms/form_header.o: components/forms/form_header.c $(INCS)
	$(CC) $(CFLAGS) $(LFLAGS) components/forms/form_header.o components/forms/form_header.c
	$(LIBR) $(STATIC) components/forms/form_header.o
#
components/forms/form_textarea.o: components/forms/form_textarea.c $(INCS)
	$(CC) $(CFLAGS) $(LFLAGS) components/forms/form_textarea.o components/forms/form_textarea.c
	$(LIBR) $(STATIC) components/forms/form_textarea.o
#
components/forms/form_seperator.o: components/forms/form_seperator.c $(INCS)
	$(CC) $(CFLAGS) $(LFLAGS) components/forms/form_seperator.o components/forms/form_seperator.c
	$(LIBR) $(STATIC) components/forms/form_seperator.o
#
components/menus/menus.o: components/menus/menus.c $(INCS) 
	$(CC) $(CFLAGS) $(LOPTS) components/menus/menus.o components/menus/menus.c 
	$(LIBR) $(STATIC) components/menus/menus.o
#
components/menus/box.o: components/menus/box.c $(INCS) 
	$(CC) $(CFLAGS) $(LOPTS) components/menus/box.o components/menus/box.c 
	$(LIBR) $(STATIC) components/menus/box.o
#
components/menus/bar.o: components/menus/bar.c $(INCS) 
	$(CC) $(CFLAGS) $(LOPTS) components/menus/bar.o components/menus/bar.c 
	$(LIBR) $(STATIC) components/menus/bar.o
#
components/menus/list.o: components/menus/list.c $(INCS) 
	$(CC) $(CFLAGS) $(LOPTS) components/menus/list.o components/menus/list.c 
	$(LIBR) $(STATIC) components/menus/list.o
#
windows/alert.o: windows/alert.c $(INCS) 
	$(CC) $(CFLAGS) $(LOPTS) windows/alert.o windows/alert.c 
	$(LIBR) $(STATIC) windows/alert.o
#
windows/status.o: windows/status.c $(INCS) 
	$(CC) $(CFLAGS) $(LOPTS) windows/status.o windows/status.c 
	$(LIBR) $(STATIC) windows/status.o
#
windows/bar_menu.o: windows/bar_menu.c $(INCS) 
	$(CC) $(CFLAGS) $(LOPTS) windows/bar_menu.o windows/bar_menu.c 
	$(LIBR) $(STATIC) windows/bar_menu.o
#
windows/box_menu.o: windows/box_menu.c $(INCS) 
	$(CC) $(CFLAGS) $(LOPTS) windows/box_menu.o windows/box_menu.c 
	$(LIBR) $(STATIC) windows/box_menu.o
#
windows/list_menu.o: windows/list_menu.c $(INCS) 
	$(CC) $(CFLAGS) $(LOPTS) windows/list_menu.o windows/list_menu.c 
	$(LIBR) $(STATIC) windows/list_menu.o
#
windows/base_window.o: windows/base_window.c $(INCS) 
	$(CC) $(CFLAGS) $(LOPTS) windows/base_window.o windows/base_window.c 
	$(LIBR) $(STATIC) windows/base_window.o
#
windows/query_window.o: windows/query_window.c $(INCS) 
	$(CC) $(CFLAGS) $(LOPTS) windows/query_window.o windows/query_window.c 
	$(LIBR) $(STATIC) windows/query_window.o
#
man:
	pod2man -c " " -r "widget(3)" -s 3 widget.pod widget.3
#
shared:
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) widget.o widget.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) colors.o colors.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) window.o window.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) workbench.o workbench.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) component.o component.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) components/label.o components/label.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) components/hline.o components/hline.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) components/vline.o components/vline.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) components/text.o components/text.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) components/prompt.o components/prompt.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) components/textarea.o components/textarea.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) components/input.o components/input.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) components/bell.o components/bell.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) components/menus/menus.o components/menus/menus.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) components/menus/bar.o components/menus/bar.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) components/menus/box.o components/menus/box.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) components/menus/list.o components/menus/list.c
#
# eof
#

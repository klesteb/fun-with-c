#
# File: Makefile
# Date: 27-Jun-2019
# By  : Kevin Esteb
#
# Makefile to build libwindows.
#
CC = gcc
CFLAGS = -Wall -c -I. -I../include -I../cclibs -I../objects
OLBS = ../libs/libwindows.a ../libs/libobjects.a ../cclibs/libcclibs.a -lncurses -lpanel -lform -lmenu
LINK = gcc
LFLAGS = -o test
LIBS = ../libs/libwindows.a
LIBR = ar -r
#
INCS = container.h colors.h events.h component.h workbench.h window.h \
       containers/horizontal.h containers/vertical.h \
       containers/scrollable.h containers/form.h \
       components/label.h components/hline.h components/vline.h \
       components/text.h components/textarea.h components/forms/forms.h
#
OBJS = container.o colors.o component.o workbench.o window.o \
       containers/horizontal.o containers/vertical.o \
       containers/scrollable.o containers/form.o \
       components/label.o components/hline.o components/vline.o \
       components/text.o components/textarea.o \
       components/forms/form_field.o components/forms/form_label.o \
       components/forms/form_textarea.o components/forms/form_header.o \
       components/forms/form_seperator.o
#
all: $(OBJS) test1 test2 test3 test4 test5 test6 test7 test8 test9 \
     test10 test11 test12 test13 test14 test15 test16 test17 test18
#
libs: $(OBJS)
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f containers/*.o
	rm -f containers/*~
	rm -f components/*.o
	rm -f components/*~
	rm -f components/forms/*.o
	rm -f components/forms/*~
	rm -f windows/*.o
	rm -f windows/*~
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
	rm -f $(LIBS)
#
test1: test1.o
	$(CC) $(CFLAGS) test1.c
	$(LINK) -o test1 test1.o  $(OLBS)
#
test2: test2.o
	$(CC) $(CFLAGS) test2.c
	$(LINK) -o test2 test2.o  $(OLBS)
#
test3: test3.o
	$(CC) $(CFLAGS) test3.c
	$(LINK) -o test3 test3.o  $(OLBS)
#
test4: test4.o
	$(CC) $(CFLAGS) test4.c
	$(LINK) -o test4 test4.o  $(OLBS)
#
test5: test5.o
	$(CC) $(CFLAGS) test5.c
	$(LINK) -o test5 test5.o  $(OLBS)
#
test6: test6.o
	$(CC) $(CFLAGS) test6.c
	$(LINK) -o test6 test6.o  $(OLBS)
#
test7: test7.o
	$(CC) $(CFLAGS) test7.c
	$(LINK) -o test7 test7.o  $(OLBS)
#
test8: test8.o
	$(CC) $(CFLAGS) test8.c
	$(LINK) -o test8 test8.o  $(OLBS)
#
test9: test9.o
	$(CC) $(CFLAGS) test9.c
	$(LINK) -o test9 test9.o  $(OLBS)
#
test10: test10.o
	$(CC) $(CFLAGS) test10.c
	$(LINK) -o test10 test10.o  $(OLBS)
#
test11: test11.o
	$(CC) $(CFLAGS) test11.c
	$(LINK) -o test11 test11.o  $(OLBS)
#
test12: test12.o
	$(CC) $(CFLAGS) test12.c
	$(LINK) -o test12 test12.o  $(OLBS)
#
test13: test13.o
	$(CC) $(CFLAGS) test13.c
	$(LINK) -o test13 test13.o  $(OLBS)
#
test14: test14.o
	$(CC) $(CFLAGS) test14.c
	$(LINK) -o test14 test14.o  $(OLBS)
#
test15: test15.o
	$(CC) $(CFLAGS) test15.c
	$(LINK) -o test15 test15.o  $(OLBS)
#
test16: test16.o
	$(CC) $(CFLAGS) test16.c
	$(LINK) -o test16 test16.o  $(OLBS)
#
test17: test17.o
	$(CC) $(CFLAGS) test17.c
	$(LINK) -o test17 test17.o  $(OLBS)
#
test18: test18.o
	$(CC) $(CFLAGS) test18.c
	$(LINK) -o test18 test18.o  $(OLBS)
#
container.o: container.c $(INCS)
	$(CC) $(CFLAGS) container.c
	$(LIBR) $(LIBS) container.o
#
colors.o: colors.c $(INCS)
	$(CC) $(CFLAGS) colors.c
	$(LIBR) $(LIBS) colors.o
#
component.o: component.c $(INCS)
	$(CC) $(CFLAGS) component.c
	$(LIBR) $(LIBS) component.o
#
window.o: window.c $(INCS)
	$(CC) $(CFLAGS) window.c
	$(LIBR) $(LIBS) window.o
#
workbench.o: workbench.c $(INCS)
	$(CC) $(CFLAGS) workbench.c
	$(LIBR) $(LIBS) workbench.o
#
containers/horizontal.o: containers/horizontal.c $(INCS)
	$(CC) $(CFLAGS) -o containers/horizontal.o containers/horizontal.c
	$(LIBR) $(LIBS) containers/horizontal.o
#
containers/vertical.o: containers/vertical.c $(INCS)
	$(CC) $(CFLAGS) -o containers/vertical.o containers/vertical.c 
	$(LIBR) $(LIBS) containers/vertical.o
#
containers/scrollable.o: containers/scrollable.c $(INCS)
	$(CC) $(CFLAGS) -o containers/scrollable.o containers/scrollable.c 
	$(LIBR) $(LIBS) containers/scrollable.o
#
containers/form.o: containers/form.c $(INCS)
	$(CC) $(CFLAGS) -o containers/form.o containers/form.c 
	$(LIBR) $(LIBS) containers/form.o
#
components/label.o: components/label.c $(INCS)
	$(CC) $(CFLAGS) -o components/label.o components/label.c
	$(LIBR) $(LIBS) components/label.o
#
components/hline.o: components/hline.c $(INCS)
	$(CC) $(CFLAGS) -o components/hline.o components/hline.c
	$(LIBR) $(LIBS) components/hline.o
#
components/vline.o: components/vline.c $(INCS)
	$(CC) $(CFLAGS) -o components/vline.o components/vline.c
	$(LIBR) $(LIBS) components/vline.o
#
components/text.o: components/text.c $(INCS)
	$(CC) $(CFLAGS) -o components/text.o components/text.c
	$(LIBR) $(LIBS) components/text.o
#
components/textarea.o: components/textarea.c $(INCS)
	$(CC) $(CFLAGS) -o components/textarea.o components/textarea.c
	$(LIBR) $(LIBS) components/textarea.o
#
#
components/forms/form_field.o: components/forms/form_field.c $(INCS)
	$(CC) $(CFLAGS) -o components/forms/form_field.o components/forms/form_field.c
	$(LIBR) $(LIBS) components/forms/form_field.o
#
components/forms/form_label.o: components/forms/form_label.c $(INCS)
	$(CC) $(CFLAGS) -o components/forms/form_label.o components/forms/form_label.c
	$(LIBR) $(LIBS) components/forms/form_label.o
#
components/forms/form_header.o: components/forms/form_header.c $(INCS)
	$(CC) $(CFLAGS) -o components/forms/form_header.o components/forms/form_header.c
	$(LIBR) $(LIBS) components/forms/form_header.o
#
components/forms/form_textarea.o: components/forms/form_textarea.c $(INCS)
	$(CC) $(CFLAGS) -o components/forms/form_textarea.o components/forms/form_textarea.c
	$(LIBR) $(LIBS) components/forms/form_textarea.o
#
components/forms/form_seperator.o: components/forms/form_seperator.c $(INCS)
	$(CC) $(CFLAGS) -o components/forms/form_seperator.o components/forms/form_seperator.c
	$(LIBR) $(LIBS) components/forms/form_seperator.o
#
# eof
#

#
# File: Makefile
# Date: 27-Nov-2019
# By  : Kevin Esteb
#
# Makefile to build template.
#
MAJOR=0
MINOR=1
PATCH=0
VERSION="$(MAJOR).$(MINOR).$(PATCH)"
#
CC = gcc
LIBR = ar -rs
LIBS = libtemplate.a
CFLAGS = -DVERSION=\"$(VERSION)\" -Wall -c -I../errors -I../tracer -I../include -I../cclibs -I../objects -I../gpl
LINK = gcc
LFLAGS = -o 
#
OLBS = $(LIBS) ../libs/libobjects.a ../libs/libgpl.a ../libs/libcclibs.a 
#
INCS = template.h
#
OBJS = template.o
#
all: $(OBJS) test1 test2
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f $(LIBS)
	rm -f test1
	rm -f test2
	rm -f template.man
#
man:
	pod2man -c " " -r "template(3)" -s 3 template.pod template.man
#
test1: test1.o
	$(CC) $(CFLAGS) test1.c
	$(LINK) $(LFLAGS) test1 test1.o $(OLBS)
#
test2: test2.o
	$(CC) $(CFLAGS) test2.c
	$(LINK) $(LFLAGS) test2 test2.o $(OLBS)
#
template.o: template.c $(INCS)
	$(CC) $(CFLAGS) template.c
	$(LIBR) $(LIBS) template.o
#
# eof
#

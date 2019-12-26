#
# File: Makefile
# Date: 27-Nov-2019
# By  : Kevin Esteb
#
# Makefile to build template.
#
CC = gcc
HOME = $(shell pwd)
CFLAGS = -Wall -c -I../include -I../cclibs -I../objects
SHARED = -shared  -o
OLBS = ../libs/libobjects.a ../cclibs/libcclibs.a 
LINK = gcc -L$(HOME) -Wl,-rpath=$(HOME)
LFLAGS = -o test
LIBR = ar -r
LIBS = libtemplate.a
#
INCS = template.h template_priv.h
#
OBJS = template.o template_priv.o libtemplate.so
#
all: $(OBJS) test1
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f $(LIBS)
	rm -f libtemplate.so
	rm -f test1
#
#
test1: test1.o
	$(CC) $(CFLAGS) test1.c
	$(LINK) -o test1 test1.o -ltemplate $(OLBS)
#
template.o: template.c $(INCS)
	$(CC) $(CFLAGS) -fPIC template.c
	$(LIBR) $(LIBS) template.o
#
template_priv.o: template_priv.c $(INCS)
	$(CC) $(CFLAGS) -fPIC template_priv.c
	$(LIBR) $(LIBS) template_priv.o
#
libtemplate.so: template.o template_priv.o
	$(CC) $(SHARED) libtemplate.so template.o template_priv.o
#
# eof
#

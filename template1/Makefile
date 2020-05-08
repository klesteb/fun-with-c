#
# File: Makefile
# Date: 27-Nov-2019
# By  : Kevin Esteb
#
# Makefile to build template.
#
CC = gcc
LINK = gcc 
LIBR = ar rs
WORKING = $(shell pwd)
CFLAGS = -Wall -c -I../include -I../cclibs -I../objects -I../gpl
LFLAGS = -o 
SFLAGS = -L$(WORKING) -Wl,-rpath=$(WORKING) -o
SHARED = -shared -o libtemplate.so
STATIC = libtemplate.a
OLBS = ../libs/libobjects.a ../libs/libgpl.a ../libs/libcclibs.a 
#
INCS = template.h template_priv.h
#
OBJS = libtemplate.a libtemplate.so
#
all: $(OBJS) test1 test2
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f libtemplate.a
	rm -f libtemplate.so
	rm -f test1
	rm -f test2
#
#
test1: test1.o
	$(CC) $(CFLAGS) test1.c
	$(LINK) $(SFLAGS) test1 test1.o -ltemplate $(OLBS)
#
test2: test1.o
	$(CC) $(CFLAGS) test1.c
	$(LINK) $(LFLAGS) test2 test1.o $(STATIC) $(OLBS)
#
template.o: template.c $(INCS)
	$(CC) $(CFLAGS) -fPIC template.c
#
template_priv.o: template_priv.c $(INCS)
	$(CC) $(CFLAGS) -fPIC template_priv.c
#
libtemplate.a: template.o template_priv.o
	$(LIBR) $(STATIC) template.o
	$(LIBR) $(STATIC) template_priv.o
#
libtemplate.so: template.o template_priv.o
	$(CC) $(SHARED) template.o template_priv.o
#
# eof
#

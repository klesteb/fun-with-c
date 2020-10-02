#
# File: Makefile
# Date: 02-Oct-2020
# By  : Kevin Esteb
#
# Makefile to build files.
#
CC = gcc
CFLAGS = -Wall -c -I../errors -I../tracer -I../include -I../cclibs -I../objects -I../gpl
OLBS = files.o ../libs/libobjects.a ../libs/libgpl.a ../libs/libcclibs.a 
LINK = gcc
LFLAGS = -o 
LIBR = ar -rs
LIBS = ../libs/libfiles.a
#
INCS = files.h
#
OBJS = files.o
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
#
#
test1: test1.o
	$(CC) $(CFLAGS) test1.c
	$(LINK) $(LFLAGS) test1 test1.o $(OLBS)
#
test2: test2.o
	$(CC) $(CFLAGS) test2.c
	$(LINK) $(LFLAGS) test2 test2.o $(OLBS)
#
files.o: files.c $(INCS)
	$(CC) $(CFLAGS) files.c
	$(LIBR) $(LIBS) files.o
#
# eof
#

#
# File: Makefile
# Date: 07-Jun-2019
# By  : Kevin Esteb
#
# Makefile to build objects.
#
CC = gcc
CFLAGS = -Wall -c -I../include -I../cclibs
OLBS = ../libs/libobjects.a ../cclibs/libcclibs.a
LINK = gcc
LFLAGS = -o 
LIBS = ../libs/libobjects.a
LIBR = ar -r
#
INCS = object.h
#
OBJS = object.o
#
all: $(OBJS)
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f $(LIBS)
#
object.o: object.c $(INCS)
	$(CC) $(CFLAGS) object.c
	$(LIBR) $(LIBS) object.o
#
# eof
#

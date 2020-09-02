#
# File: Makefile
# Date: 02-Sep-2020
# By  : Kevin Esteb
#
# Makefile to build trace.
#
CC = gcc
CFLAGS = -Wall -c -I../include -I../cclibs -I../objects -I../gpl -I../errors
OLBS = tracer.o ../libs/liberrors.a ../libs/libobjects.a ../libs/libgpl.a ../libs/libcclibs.a  
LINK = gcc
LFLAGS = -o 
LIBR = ar -rs
LIBS = ../libs/libtracer.a
#
INCS = tracer.h
#
OBJS = tracer.o
#
all: $(OBJS) test1 test2
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f test1
	rm -f test2
	rm -f $(LIBS)
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
tracer.o: tracer.c $(INCS)
	$(CC) $(CFLAGS) tracer.c
	$(LIBR) $(LIBS) tracer.o
#
# eof
#

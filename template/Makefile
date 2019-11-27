#
# File: Makefile
# Date: 27-Nov-2019
# By  : Kevin Esteb
#
# Makefile to build template.
#
CC = gcc
CFLAGS = -Wall -c -I../include -I../cclibs -I../objects
OLBS = template.o ../libs/libobjects.a ../cclibs/libcclibs.a 
LINK = gcc
LFLAGS = -o test
LIBR = ar -r
#
INCS = template.h
#
OBJS = template.o
#
all: $(OBJS) test1
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f test1
#
#
test1: test1.o
	$(CC) $(CFLAGS) test1.c
	$(LINK) -o test1 test1.o  $(OLBS)
#
template.o: template.c $(INCS)
	$(CC) $(CFLAGS) template.c
#
# eof
#

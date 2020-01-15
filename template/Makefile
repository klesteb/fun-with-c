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
LFLAGS = -o 
LIBR = ar -r
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
template.o: template.c $(INCS)
	$(CC) $(CFLAGS) template.c
#
# eof
#

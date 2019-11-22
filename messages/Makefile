#
# File: Makefile
# Date: 21-Nov-2019
# By  : Kevin Esteb
#
# Makefile to build messages.
#
CC = gcc
CFLAGS = -Wall -c -I../include -I../cclibs -I../objects
OLBS = message.o ../libs/libobjects.a ../cclibs/libcclibs.a 
LINK = gcc
LFLAGS = -o test
LIBR = ar -r
LIBS = ../libs/libmessages.a
#
INCS = message.h
#
OBJS = message.o
#
all: $(OBJS) test1
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f test1
	rm -f $(LIBS)
#
#
test1: test1.o
	$(CC) $(CFLAGS) test1.c
	$(LINK) -o test1 test1.o  $(OLBS)
#
message.o: message.c $(INCS)
	$(CC) $(CFLAGS) message.c
	$(LIBR) $(LIBS) message.o
#
# eof
#

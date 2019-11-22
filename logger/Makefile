#
# File: Makefile
# Date: 07-Jun-2019
# By  : Kevin Esteb
#
# Makefile to build factory.
#
CC = gcc
CFLAGS = -Wall -c -I../include -I../cclibs -I../objects
OLBS = ../libs/liblogger.a ../libs/libobjects.a ../cclibs/libcclibs.a -llog4c
LINK = gcc
LFLAGS = -o test
LIBR = ar -r
LIBS = ../libs/liblogger.a
#
INCS = logger.h log4c_extensions.h
#
OBJS = logger.o log4c_extensions.o
#
all: $(OBJS) test1 test2 test3 test4
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f test1
	rm -f test2
	rm -f test3
	rm -f test4
	rm -f ../libs/liblogger.a
#
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
logger.o: logger.c $(INCS)
	$(CC) $(CFLAGS) logger.c
	$(LIBR) $(LIBS) logger.o
#
log4c_extensions.o: log4c_extensions.c $(INCS)
	$(CC) $(CFLAGS) log4c_extensions.c
	$(LIBR) $(LIBS) log4c_extensions.o
#
# eof
#

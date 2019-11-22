#
# File: Makefile
# Date: 09-Sep-2019
# By  : Kevin Esteb
#
# Makefile to build error_code.
#
CC = gcc
CFLAGS = -Wall -c -I../include -I../cclibs -I../objects
OLBS = ../libs/liberrors.a ../libs/libobjects.a ../cclibs/libcclibs.a 
LINK = gcc
LFLAGS = -o test
LIBS = ../libs/liberrors.a
LIBR = ar -r
#
INCS = errors.h ../include/error_codes.h
#
OBJS = errors.o 
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
	$(LINK) -o test1 test1.o $(OLBS)
#
test2: test2.o
	$(CC) $(CFLAGS) test2.c
	$(LINK) -o test2 test2.o $(OLBS)
#
errors.o: errors.c $(INCS)
	$(CC) $(CFLAGS) errors.c
	$(LIBR) $(LIBS) errors.o
#
errnoname.o: errnoname.c $(INCS)
	$(CC) $(CFLAGS) errnoname.c
	$(LIBR) $(LIBS) errnoname.o
#
# eof
#

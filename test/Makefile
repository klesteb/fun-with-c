#
# File: Makefile
# Date: 14-Nov-2019
# By  : Kevin Esteb
#
# Makefile to build testing .
#
CC = gcc
CFLAGS = -Wall -c -I../include -I../cclibs -I../objects -I../logger -I../errors -I../messages
OLBS = ../libs/libmessages.a ../libs/liberrors.a ../libs/liblogger.a ../libs/libobjects.a ../cclibs/libcclibs.a -llog4c
LINK = gcc
LFLAGS = -o test
LIBR = ar -r
#
#INCS = spooler.h
#
#OBJS = spooler.o
#
all: $(OBJS) test test2 test3 test4 test5 test6 test7
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f test
	rm -f test2
	rm -f test3
	rm -f test4
	rm -f test5
	rm -f test6
	rm -f test7
#
test: test.o
	$(CC) $(CFLAGS) test.c
	$(LINK) -o test test.o  $(OLBS)
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
test5: test5.o
	$(CC) $(CFLAGS) test5.c
	$(LINK) -o test5 test5.o  $(OLBS)
#
test6: test6.o
	$(CC) $(CFLAGS) test6.c
	$(LINK) -o test6 test6.o  $(OLBS)
#
test7: test7.o
	$(CC) $(CFLAGS) test7.c
	$(LINK) -o test7 test7.o  $(OLBS)
#
# eof
#

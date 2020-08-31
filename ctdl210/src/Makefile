#
# File: Makefile
# Date: 22-May-2020
# By  : Kevin Esteb
#
# Makefile to build ctdl210.
#
CC = gcc
CFLAGS = -Wall -c -g 
OLBS = libctdl.a 
LINK = gcc
LFLAGS = -o 
LIBR = ar -rs
STATIC = libctdl.a
#
INCS = ctdl.h protos.h common.h common.c
#
OBJS = log.o misc.o modem.o msga.o msgb.o rooma.o roomb.o
#
all: $(OBJS) citadel confg test1 test2 test3 test4 test5
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f libctdl.a
	rm -f confg
	rm -f citadel
	rm -f test1
	rm -f test2
	rm -f test3
	rm -f test4
	rm -f test5
#
#
test1: test1.c $(INCS)
	$(CC) $(CFLAGS) test1.c
	$(LINK) $(LFLAGS) test1 test1.o $(OLBS)
#
test2: test2.c $(INCS)
	$(CC) $(CFLAGS) test2.c
	$(LINK) $(LFLAGS) test2 test2.o $(OLBS)
#
test3: test3.c $(INCS)
	$(CC) $(CFLAGS) test3.c
	$(LINK) $(LFLAGS) test3 test3.o $(OLBS)
#
test4: test4.c $(INCS)
	$(CC) $(CFLAGS) test4.c
	$(LINK) $(LFLAGS) test4 test4.o $(OLBS)
#
test5: test5.c $(INCS)
	$(CC) $(CFLAGS) test5.c
	$(LINK) $(LFLAGS) test5 test5.o $(OLBS)
#
citadel: ctdl.c $(INCS)
	$(CC) $(CFLAGS) ctdl.c
	$(LINK) $(LFLAGS) citadel ctdl.o $(OLBS)
#
confg: confg.c $(INCS) 
	$(CC) $(CFLAGS) confg.c
	$(LINK) $(LFLAGS) confg confg.o $(OLBS)
#
log.o: log.c $(INCS)
	$(CC) $(CFLAGS) log.c
	$(LIBR) $(STATIC) log.o
#
misc.o: misc.c $(INCS)
	$(CC) $(CFLAGS) misc.c
	$(LIBR) $(STATIC) misc.o
#
modem.o: modem.c $(INCS)
	$(CC) $(CFLAGS) modem.c
	$(LIBR) $(STATIC) modem.o
#
msga.o: msga.c $(INCS)
	$(CC) $(CFLAGS) msga.c
	$(LIBR) $(STATIC) msga.o
#
msgb.o: msgb.c $(INCS)
	$(CC) $(CFLAGS) msgb.c
	$(LIBR) $(STATIC) msgb.o
#
rooma.o: rooma.c $(INCS)
	$(CC) $(CFLAGS) rooma.c
	$(LIBR) $(STATIC) rooma.o
#
roomb.o: roomb.c $(INCS)
	$(CC) $(CFLAGS) roomb.c
	$(LIBR) $(STATIC) roomb.o
#
# eof
#

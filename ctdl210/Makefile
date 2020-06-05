#
# File: Makefile
# Date: 22-May-2020
# By  : Kevin Esteb
#
# Makefile to build ctdl210.
#
CC = gcc
CFLAGS = -Wall -c 
OLBS = 210ctdl.a 
LINK = gcc
LFLAGS = -o 
LIBR = ar -rs
STATIC = 210ctdl.a
#
INCS = 210ctdl.h 210protos.h 210common.h 210common.c
#
OBJS = 210log.o 210misc.o 210modem.o 210msg.o 210rooma.o 210roomb.o
#
all: $(OBJS) 210ctdl 210confg test1 test2 test3 test4 test5
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f libctdl.a
	rm -f 210confg
	rm -f 210ctdl
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
210ctdl: 210ctdl.c $(INCS)
	$(CC) $(CFLAGS) 210ctdl.c
	$(LINK) $(LFLAGS) 210ctdl 210ctdl.o $(OLBS)
#
210confg: 210confg.c $(INCS) 
	$(CC) $(CFLAGS) 210confg.c
	$(LINK) $(LFLAGS) 210confg 210confg.o $(OLBS)
#
210log.o: 210log.c $(INCS)
	$(CC) $(CFLAGS) 210log.c
	$(LIBR) $(STATIC) 210log.o
#
210misc.o: 210misc.c $(INCS)
	$(CC) $(CFLAGS) 210misc.c
	$(LIBR) $(STATIC) 210misc.o
#
210modem.o: 210modem.c $(INCS)
	$(CC) $(CFLAGS) 210modem.c
	$(LIBR) $(STATIC) 210modem.o
#
210msg.o: 210msg.c $(INCS)
	$(CC) $(CFLAGS) 210msg.c
	$(LIBR) $(STATIC) 210msg.o
#
210rooma.o: 210rooma.c $(INCS)
	$(CC) $(CFLAGS) 210rooma.c
	$(LIBR) $(STATIC) 210rooma.o
#
210roomb.o: 210roomb.c $(INCS)
	$(CC) $(CFLAGS) 210roomb.c
	$(LIBR) $(STATIC) 210roomb.o
#
# eof
#

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
INCS = 210ctdl.h 210protos.h
#
OBJS = 210log.o 210misc.o 210modem.o 210msg.o 210rooma.o 210roomb.o
#
all: $(OBJS) 210ctdl 210confg
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f 210config
	rm -f 210ctdl
#
#
210ctdl: 210ctdl.o
	$(CC) $(CFLAGS) 210ctdl.c
	$(LINK) $(LFLAGS) 210ctdl 210ctdl.o $(OLBS)
#
210confg: 210confg.o
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

#
# File: Makefile
# Date: 14-Nov-2019
# By  : Kevin Esteb
#
# Makefile to build testing .
#
CC = gcc
CFLAGS = -Wall -c -I../
OLBS = ../libs/libtracer.a ../libs/libmessage.a ../libs/liberrors.a ../libs/liblogger.a ../libs/libobjects.a ../libs/libgpl.a ../libs/libcclibs.a -llog4c
LINK = gcc
LFLAGS = -o 
LIBR = ar -r
#
#INCS = 
#
#OBJS = 
#
all: $(OBJS) test test2 test3 test4 test5 test6 test7 test8 test9 \
     test10 test11
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
	rm -f test8
	rm -f test9
	rm -f test10
	rm -f test11
#
test: test.o
	$(CC) $(CFLAGS) test.c
	$(LINK) $(LFLAGS) test test.o  $(OLBS)
#
test2: test2.o
	$(CC) $(CFLAGS) test2.c
	$(LINK) $(LFLAGS) test2 test2.o  $(OLBS)
#
test3: test3.o
	$(CC) $(CFLAGS) test3.c
	$(LINK) $(LFLAGS) test3 test3.o  $(OLBS)
#
test4: test4.o
	$(CC) $(CFLAGS) test4.c
	$(LINK) $(LFLAGS) test4 test4.o  $(OLBS)
#
test5: test5.o
	$(CC) $(CFLAGS) test5.c
	$(LINK) $(LFLAGS) test5 test5.o  $(OLBS)
#
test6: test6.o
	$(CC) $(CFLAGS) test6.c
	$(LINK) $(LFLAGS) test6 test6.o  $(OLBS)
#
test7: test7.o
	$(CC) $(CFLAGS) test7.c
	$(LINK) $(LFLAGS) test7 test7.o  $(OLBS)
#
test8: test8.o
	$(CC) $(CFLAGS) test8.c
	$(LINK) $(LFLAGS) test8 test8.o  $(OLBS)
#
test9: test9.o
	$(CC) $(CFLAGS) test9.c
	$(LINK) $(LFLAGS) test9 test9.o  $(OLBS)
#
test10: test10.o
	$(CC) $(CFLAGS) test10.c
	$(LINK) $(LFLAGS) test10 test10.o  $(OLBS)
#
test11: test11.o
	$(CC) $(CFLAGS) test11.c
	$(LINK) $(LFLAGS) test11 test11.o  $(OLBS)
#
# eof
#

#
# File: Makefile
# Date: 05-Jan-2021
# By  : Kevin Esteb
#
# Makefile to build the tests.
#
#
CC = gcc
LINK = gcc
LIBS = ../../libs/
OLIBS = ../../../libs/
INCLUDE = -I.. -I../jam -I../qwk -I../room -I../node
STATIC = $(LIBS)libroom.a $(LIBS)libjam.a $(LIBS)libqwk.a  \
         $(LIBS)libnode.a $(OLIBS)libtracer.a $(OLIBS)liberrors.a \
         $(OLIBS)libfiles.a $(OLIBS)libobjects.a $(OLIBS)libcclibs.a \
         $(OLIBS)libgpl.a
CFLAGS = -Wall -c $(INCLUDE) -I../../../files -I../../../errors -I../../../tracer -I../../../include -I../../../cclibs -I../../../objects -I../../../gpl
LFLAGS =
LOPTS = -o 
#
OLBS = $(STATIC)
#
TARGETS = test1 test2 test3 test4 test5 test6 test7 test8 test9 test10 \
          test11 test12 test13 test14 test15 test16 test17 test18 test19 test20 \
          test21
#
all: $(TARGETS)
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f $(TARGETS)
#
test1: test1.o
	$(CC) $(CFLAGS) test1.c
	$(LINK) $(LOPTS) test1 test1.o $(OLBS)
#
test2: test2.o
	$(CC) $(CFLAGS) test2.c
	$(LINK) $(LOPTS) test2 test2.o $(OLBS)
#
test3: test3.o
	$(CC) $(CFLAGS) test3.c
	$(LINK) $(LOPTS) test3 test3.o $(OLBS)
#
test4: test4.o
	$(CC) $(CFLAGS) test4.c
	$(LINK) $(LOPTS) test4 test4.o $(OLBS)
#
test5: test5.o
	$(CC) $(CFLAGS) test5.c
	$(LINK) $(LOPTS) test5 test5.o $(OLBS)
#
test6: test6.o
	$(CC) $(CFLAGS) test6.c
	$(LINK) $(LOPTS) test6 test6.o $(OLBS)
#
test7: test7.o
	$(CC) $(CFLAGS) test7.c
	$(LINK) $(LOPTS) test7 test7.o $(OLBS)
#
test8: test8.o
	$(CC) $(CFLAGS) test8.c
	$(LINK) $(LOPTS) test8 test8.o $(OLBS)
#
test9: test9.o
	$(CC) $(CFLAGS) test9.c
	$(LINK) $(LOPTS) test9 test9.o $(OLBS)
#
test10: test10.o
	$(CC) $(CFLAGS) test10.c
	$(LINK) $(LOPTS) test10 test10.o $(OLBS)
#
test11: test11.o
	$(CC) $(CFLAGS) test11.c
	$(LINK) $(LOPTS) test11 test11.o $(OLBS)
#
test12: test12.o
	$(CC) $(CFLAGS) test12.c
	$(LINK) $(LOPTS) test12 test12.o $(OLBS)
#
test13: test13.o
	$(CC) $(CFLAGS) test13.c
	$(LINK) $(LOPTS) test13 test13.o $(OLBS)
#
test14: test14.o
	$(CC) $(CFLAGS) test14.c
	$(LINK) $(LOPTS) test14 test14.o $(OLBS)
#
test15: test15.o
	$(CC) $(CFLAGS) test15.c
	$(LINK) $(LOPTS) test15 test15.o $(OLBS)
#
test16: test16.o
	$(CC) $(CFLAGS) test16.c
	$(LINK) $(LOPTS) test16 test16.o $(OLBS)
#
test17: test17.o
	$(CC) $(CFLAGS) test17.c
	$(LINK) $(LOPTS) test17 test17.o $(OLBS)
#
test18: test18.o
	$(CC) $(CFLAGS) test18.c
	$(LINK) $(LOPTS) test18 test18.o $(OLBS)
#
test19: test19.o
	$(CC) $(CFLAGS) test19.c
	$(LINK) $(LOPTS) test19 test19.o $(OLBS)
#
test20: test20.o
	$(CC) $(CFLAGS) test20.c
	$(LINK) $(LOPTS) test20 test20.o $(OLBS)
#
test21: test21.o
	$(CC) $(CFLAGS) test21.c
	$(LINK) $(LOPTS) test21 test21.o $(OLBS)
#
# eof
#

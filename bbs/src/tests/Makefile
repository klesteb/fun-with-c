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
INCLUDE = -I.. -I../jam -I../qwk -I../room -I../node -I../user -I../finds
STATIC = $(LIBS)libuser.a $(LIBS)libroom.a $(LIBS)libjam.a $(LIBS)libqwk.a  \
         $(LIBS)libnode.a $(LIBS)libfinds.a $(OLIBS)librms.a $(OLIBS)libtracer.a $(OLIBS)liberrors.a \
         $(OLIBS)libfiles.a $(OLIBS)libobjects.a $(OLIBS)libcclibs.a \
         $(OLIBS)libgpl.a
CFLAGS = -Wall -c $(INCLUDE) -I../../../rms -I../../../files -I../../../errors -I../../../tracer -I../../../include -I../../../cclibs -I../../../objects -I../../../gpl
LFLAGS =
LOPTS = -o 
#
OLBS = $(STATIC)
#
TARGETS = jam-test1 jam-test2 jam-test3 jam-test4 jam-test5 jam-test6 \
          jam-test7 jam-test8 qwk-test1 qwk-test2 qwk-test3 qwk-test4 \
          qwk-test5 room-test1 room-test2 room-test3 room-test4 room-test5 \
          room-test6 room-test7 test1 node-test1 node-test2 node-test3 \
          user-test1 user-test2
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
jam-test1: jam-test1.o
	$(CC) $(CFLAGS) jam-test1.c
	$(LINK) $(LOPTS) jam-test1 jam-test1.o $(OLBS)
#
jam-test2: jam-test2.o
	$(CC) $(CFLAGS) jam-test2.c
	$(LINK) $(LOPTS) jam-test2 jam-test2.o $(OLBS)
#
jam-test3: jam-test3.o
	$(CC) $(CFLAGS) jam-test3.c
	$(LINK) $(LOPTS) jam-test3 jam-test3.o $(OLBS)
#
jam-test4: jam-test4.o
	$(CC) $(CFLAGS) jam-test4.c
	$(LINK) $(LOPTS) jam-test4 jam-test4.o $(OLBS)
#
jam-test5: jam-test5.o
	$(CC) $(CFLAGS) jam-test5.c
	$(LINK) $(LOPTS) jam-test5 jam-test5.o $(OLBS)
#
jam-test6: jam-test6.o
	$(CC) $(CFLAGS) jam-test6.c
	$(LINK) $(LOPTS) jam-test6 jam-test6.o $(OLBS)
#
jam-test7: jam-test7.o
	$(CC) $(CFLAGS) jam-test7.c
	$(LINK) $(LOPTS) jam-test7 jam-test7.o $(OLBS)
#
jam-test8: jam-test8.o
	$(CC) $(CFLAGS) jam-test8.c
	$(LINK) $(LOPTS) jam-test8 jam-test8.o $(OLBS)
#
jam-test9: jam-test9.o
	$(CC) $(CFLAGS) jam-test9.c
	$(LINK) $(LOPTS) jam-test9 jam-test9.o $(OLBS)
#
qwk-test1: qwk-test1.o
	$(CC) $(CFLAGS) qwk-test1.c
	$(LINK) $(LOPTS) qwk-test1 qwk-test1.o $(OLBS)
#
qwk-test2: qwk-test2.o
	$(CC) $(CFLAGS) qwk-test2.c
	$(LINK) $(LOPTS) qwk-test2 qwk-test2.o $(OLBS)
#
qwk-test3: qwk-test3.o
	$(CC) $(CFLAGS) qwk-test3.c
	$(LINK) $(LOPTS) qwk-test3 qwk-test3.o $(OLBS)
#
qwk-test4: qwk-test4.o
	$(CC) $(CFLAGS) qwk-test4.c
	$(LINK) $(LOPTS) qwk-test4 qwk-test4.o $(OLBS)
#
qwk-test5: qwk-test5.o
	$(CC) $(CFLAGS) qwk-test5.c
	$(LINK) $(LOPTS) qwk-test5 qwk-test5.o $(OLBS)
#
room-test1: room-test1.o
	$(CC) $(CFLAGS) room-test1.c
	$(LINK) $(LOPTS) room-test1 room-test1.o $(OLBS)
#
room-test2: room-test2.o
	$(CC) $(CFLAGS) room-test2.c
	$(LINK) $(LOPTS) room-test2 room-test2.o $(OLBS)
#
room-test3: room-test3.o
	$(CC) $(CFLAGS) room-test3.c
	$(LINK) $(LOPTS) room-test3 room-test3.o $(OLBS)
#
room-test4: room-test4.o
	$(CC) $(CFLAGS) room-test4.c
	$(LINK) $(LOPTS) room-test4 room-test4.o $(OLBS)
#
room-test5: room-test5.o
	$(CC) $(CFLAGS) room-test5.c
	$(LINK) $(LOPTS) room-test5 room-test5.o $(OLBS)
#
room-test6: room-test6.o
	$(CC) $(CFLAGS) room-test6.c
	$(LINK) $(LOPTS) room-test6 room-test6.o $(OLBS)
#
room-test7: room-test7.o
	$(CC) $(CFLAGS) room-test7.c
	$(LINK) $(LOPTS) room-test7 room-test7.o $(OLBS)
#
node-test1: node-test1.o
	$(CC) $(CFLAGS) node-test1.c
	$(LINK) $(LOPTS) node-test1 node-test1.o $(OLBS)
#
node-test2: node-test2.o
	$(CC) $(CFLAGS) node-test2.c
	$(LINK) $(LOPTS) node-test2 node-test2.o $(OLBS)
#
node-test3: node-test3.o
	$(CC) $(CFLAGS) node-test3.c
	$(LINK) $(LOPTS) node-test3 node-test3.o $(OLBS)
#
user-test1: user-test1.o
	$(CC) $(CFLAGS) user-test1.c
	$(LINK) $(LOPTS) user-test1 user-test1.o $(OLBS)
#
user-test2: user-test2.o
	$(CC) $(CFLAGS) user-test2.c
	$(LINK) $(LOPTS) user-test2 user-test2.o $(OLBS)
#
# eof
#

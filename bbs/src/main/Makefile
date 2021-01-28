#
# File: Makefile
# Date: 25-Jan-2021
# By  : Kevin Esteb
#
# Makefile to build the bbs.
#
#
MAJOR=1
MINOR=0
PATCH=0
VERSION="$(MAJOR).$(MINOR).$(PATCH)"
#
CC = gcc
LINK = gcc
LIBS = ../../libs/
OLIBS = ../../../libs/
INCLUDE = -I.. -I../jam -I../qwk -I../room -I../node -I../user 
STATIC = $(LIBS)libuser.a $(LIBS)libroom.a $(LIBS)libjam.a $(LIBS)libqwk.a  \
         $(LIBS)libnode.a $(OLIBS)libevents.a $(OLIBS)libtracer.a \
         $(OLIBS)liberrors.a $(OLIBS)libfiles.a $(OLIBS)libobjects.a \
         $(OLIBS)libcclibs.a $(OLIBS)libgpl.a
CFLAGS = -Wall -DVERSION=\"$(VERSION)\" -c $(INCLUDE) -I../../../events -I../../../files -I../../../errors -I../../../tracer -I../../../include -I../../../cclibs -I../../../objects -I../../../gpl
LFLAGS =
LOPTS = -o 
#
OLBS = $(STATIC)
#
TARGETS = xa-bbs
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
xa-bbs: bbs_main.o bbs_run.o bbs_logoff.o tty.o
	$(LINK) $(LOPTS) xa-bbs bbs_main.o bbs_run.o bbs_logoff.o tty.o $(OLBS)
#
tty.o.o: tty.c
	$(CC) $(CFLAGS) tty.c
#
bbs_main.o: bbs_main.c
	$(CC) $(CFLAGS) bbs_main.c
#
bbs_run.o: bbs_run.c
	$(CC) $(CFLAGS) bbs_run.c
#
bbs_logoff.o: bbs_logoff.c
	$(CC) $(CFLAGS) bbs_logoff.c
#
# eof
#

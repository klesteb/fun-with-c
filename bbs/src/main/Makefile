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
INCLUDE = -I.. -I../jam -I../qwk -I../room -I../node -I../user -I../finds
STATIC = $(LIBS)libuser.a $(LIBS)libroom.a $(LIBS)libjam.a $(LIBS)libqwk.a  \
         $(LIBS)libnode.a $(LIBS)libfinds.a $(OLIBS)libevent.a \
         $(OLIBS)libtracer.a $(OLIBS)liberrors.a $(OLIBS)libfiles.a \
         $(OLIBS)libwindows.a $(OLIBS)libobjects.a $(OLIBS)libcclibs.a \
         $(OLIBS)libgpl.a -lncurses -lpanel
CFLAGS = -Wall -DVERSION=\"$(VERSION)\" -c $(INCLUDE) -I../../../windows -I../../../events -I../../../files -I../../../errors -I../../../tracer -I../../../include -I../../../cclibs -I../../../objects -I../../../gpl
LFLAGS =
LOPTS = -o 
MODULES = bbs_main.o bbs_run.o bbs_logoff.o bbs_workbench.o bbs_access.o \
          bbs_ui.o bbs_ipc.o
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
xa-bbs: $(MODULES)
	$(LINK) $(LOPTS) xa-bbs $(MODULES) $(OLBS)
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
bbs_access.o: bbs_access.c
	$(CC) $(CFLAGS) bbs_access.c
#
bbs_workbench.o: bbs_workbench.c
	$(CC) $(CFLAGS) bbs_workbench.c
#
bbs_ui.o: bbs_ui.c
	$(CC) $(CFLAGS) bbs_ui.c
#
bbs_ipc.o: bbs_ipc.c
	$(CC) $(CFLAGS) bbs_ipc.c
#
# eof
#

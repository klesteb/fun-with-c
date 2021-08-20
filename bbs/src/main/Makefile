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
LIBS = ../libs/
OLIBS = ../../../libs/
INCLUDE = -I../../../
STATIC = $(LIBS)libuser.a $(LIBS)libroom.a $(LIBS)libjam.a $(LIBS)libqwk.a  \
         $(LIBS)libnode.a $(LIBS)libfinds.a $(LIBS)libdoor.a \
         $(OLIBS)libevent.a $(OLIBS)librms.a $(OLIBS)libtracer.a \
         $(OLIBS)liberrors.a $(OLIBS)libfiles.a $(OLIBS)libwidgets.a \
         $(OLIBS)libobjects.a $(OLIBS)libcclibs.a $(OLIBS)libgpl.a \
         -lpanel -lmenu -lncurses
CFLAGS = -Wall -DVERSION=\"$(VERSION)\" -c $(INCLUDE) 
LFLAGS =
LOPTS = -o 
INCS = bbs_common.h bbs_includes.h bbs_config.h bbs_error_codes.h \
       bbs_errors.h  bbs_protos.h
MODULES = bbs_main.o bbs_run.o bbs_access.o bbs_ipc.o bbs_logout.o \
          bbs_rooms.o bbs_ui.o bbs_msgs.o bbs_utils.o bbs_doors.o
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
bbs_main.o: bbs_main.c $(INCS)
	$(CC) $(CFLAGS) bbs_main.c
#
bbs_run.o: bbs_run.c $(INCS)
	$(CC) $(CFLAGS) bbs_run.c
#
bbs_access.o: bbs_access.c $(INCS)
	$(CC) $(CFLAGS) bbs_access.c
#
bbs_ui.o: bbs_ui.c $(INCS)
	$(CC) $(CFLAGS) bbs_ui.c
# 
bbs_ipc.o: bbs_ipc.c $(INCS)
	$(CC) $(CFLAGS) bbs_ipc.c
#
bbs_rooms.o: bbs_rooms.c $(INCS)
	$(CC) $(CFLAGS) bbs_rooms.c
#
bbs_msgs.o: bbs_msgs.c $(INCS)
	$(CC) $(CFLAGS) bbs_msgs.c
#
bbs_utils.o: bbs_utils.c $(INCS)
	$(CC) $(CFLAGS) bbs_utils.c
#
bbs_logout.o: bbs_logout.c $(INCS)
	$(CC) $(CFLAGS) bbs_logout.c
#
bbs_doors.o: bbs_doors.c $(INCS)
	$(CC) $(CFLAGS) bbs_doors.c
#
# eof
#

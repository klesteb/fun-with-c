#
# File: Makefile
# Date: 05-Jan-2021
# By  : Kevin Esteb
#
# Makefile to build the utils.
#
#
CC = gcc
LINK = gcc
LIBS = ../libs/
OLIBS = ../../../libs/
INCLUDE = -I ../../../
STATIC = $(LIBS)libroom.a $(LIBS)libdoor.a $(LIBS)libfinds.a \
         $(LIBS)libjam.a $(LIBS)libqwk.a $(LIBS)libnode.a \
         $(OLIBS)libtracer.a $(OLIBS)liberrors.a \
         $(OLIBS)librms.a $(OLIBS)libfiles.a $(OLIBS)libobjects.a \
         $(OLIBS)libcclibs.a $(OLIBS)libgpl.a
CFLAGS = -Wall -c $(INCLUDE) 
LFLAGS =
LOPTS = -o 
#
OLBS = $(STATIC)
#
TARGETS = qwk-dump makekey qwk-load lcnames room-dump room-create
#
all: $(TARGETS)
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f $(TARGETS)
	rm -f *.3
#
qwk-dump: qwk-dump.o
	$(CC) $(CFLAGS) qwk-dump.c
	$(LINK) $(LOPTS) qwk-dump qwk-dump.o $(OLBS)
#
qwk-load: qwk-load.o
	$(CC) $(CFLAGS) qwk-load.c
	$(LINK) $(LOPTS) qwk-load qwk-load.o ../main/bbs_access.o ../main/bbs_utils.o $(OLBS) 
#
makekey: makekey.o
	$(CC) $(CFLAGS) makekey.c
	$(LINK) $(LOPTS) makekey makekey.o $(OLBS)
#
lcnames: lcnames.o
	$(CC) $(CFLAGS) lcnames.c
	$(LINK) $(LOPTS) lcnames lcnames.o $(OLBS)
#
room-dump: room-dump.o
	$(CC) $(CFLAGS) room-dump.c
	$(LINK) $(LOPTS) room-dump room-dump.o ../main/bbs_access.o ../main/bbs_utils.o $(OLBS)
#
room-create: room-create.o
	$(CC) $(CFLAGS) room-create.c
	$(LINK) $(LOPTS) room-create room-create.o ../main/bbs_access.o ../main/bbs_utils.o $(OLBS) 
#
man:
	pod2man -c " " -r "lcnames(3)" -s 3 lcnames.pod lcnames.3
	pod2man -c " " -r "makekey(3)" -s 3 makekey.pod makekey.3
	pod2man -c " " -r "qwk-dump(3)" -s 3 qwk-dump.pod qwk-dump.3
	pod2man -c " " -r "qwk-load(3)" -s 3 qwk-load.pod qwk-load.3
	pod2man -c " " -r "room-create(3)" -s 3 room-create.pod room-create.3
	pod2man -c " " -r "room-dump(3)" -s 3 room-dump.pod room-dump.3
#
# eof
#

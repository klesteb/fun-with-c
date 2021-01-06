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
TARGETS = qwk-dump
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
qwk-dump: qwk-dump.o
	$(CC) $(CFLAGS) qwk-dump.c
	$(LINK) $(LOPTS) qwk-dump qwk-dump.o $(OLBS)
#
# eof
#

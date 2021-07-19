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
INCLUDE = -I.. -I../main -I../finds -I../user -I../jam -I../qwk -I../room -I../node
STATIC = $(LIBS)libroom.a $(LIBS)libfinds.a $(LIBS)libjam.a $(LIBS)libqwk.a  \
         $(LIBS)libnode.a $(OLIBS)libtracer.a $(OLIBS)liberrors.a \
         $(LIBS)librms.a $(OLIBS)libfiles.a $(OLIBS)libobjects.a \
         $(OLIBS)libcclibs.a $(OLIBS)libgpl.a
CFLAGS = -Wall -c $(INCLUDE) -I../../../rms -I../../../files -I../../../errors -I../../../tracer -I../../../include -I../../../cclibs -I../../../objects -I../../../gpl
LFLAGS =
LOPTS = -o 
#
OLBS = $(STATIC)
#
TARGETS = qwk-dump makekey qwk-load lcnames
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
qwk-load: qwk-load.o
	$(CC) $(CFLAGS) qwk-load.c
	$(LINK) $(LOPTS) qwk-load qwk-load.o $(OLBS)
#
makekey: makekey.o
	$(CC) $(CFLAGS) makekey.c
	$(LINK) $(LOPTS) makekey makekey.o $(OLBS)
#
lcname: lcname.o
	$(CC) $(CFLAGS) lcname.c
	$(LINK) $(LOPTS) lcname lcname.o $(OLBS)
#
# eof
#

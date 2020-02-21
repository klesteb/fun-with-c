#
# File: Makefile
# Date: 11-Feb-2020
# By  : Kevin Esteb
#
# Makefile to build libpjl.a.
#
CC = gcc 
CFLAGS = -Wall -c -I.. -I../misc -I../../gpl
OLBS = libpjl.a ../../libs/libcclibs.a ../../libs/libgpl.a -lpcre
LINK = gcc
LFLAGS = -o
LIBS = libpjl.a 
LIBR = ar -r
#
INCS = ../pjl_util.h pjl_priv.h
#
OBJS = pjl_close.o pjl_core.o pjl_dump_config.o pjl_dump_ustatus.o \
       pjl_dump_variables.o pjl_echo.o pjl_eoj.o pjl_inquire.o \
       pjl_jobname.o pjl_language.o pjl_open.o pjl_rdymsg.o \
       pjl_ustatus.o pjl_ustatusoff.o pjl_set_debug.o pjl_load_config.o \
       pjl_load_variables.o pjl_load_ustatus.o pjl_stop.o pjl_start.o \
       pjl_load_model.o pjl_get_model.o
#
all: $(OBJS) test1 test2 test3 test4 test5 test6 test7
#
debug:
#CFLAGS = -ggdb -Wall -c -I.. -I../misc
#LFLAGS = -ggdb -o 
#
clean:
	rm -f *.o
	rm -f *~
	rm -f libpjl.a
	rm -f test1
	rm -f test2
	rm -f test3
	rm -f test4
	rm -f test5
	rm -f test6
	rm -f test7
#
test1: test1.c $(INCS)
	$(CC) $(CFLAGS) test1.c
	$(LINK) $(LFLAGS) test1 test1.o $(OLBS)
#
test2: test2.c $(INCS)
	$(CC) $(CFLAGS) test2.c
	$(LINK) $(LFLAGS) test2 test2.o $(OLBS)
#
test3: test3.c $(INCS)
	$(CC) $(CFLAGS) test3.c
	$(LINK) $(LFLAGS) test3 test3.o $(OLBS)
#
test4: test4.c $(INCS)
	$(CC) $(CFLAGS) test4.c
	$(LINK) $(LFLAGS) test4 test4.o $(OLBS)
#
test5: test5.c $(INCS)
	$(CC) $(CFLAGS) test5.c
	$(LINK) $(LFLAGS) test5 test5.o $(OLBS)
#
test6: test6.c $(INCS)
	$(CC) $(CFLAGS) test6.c
	$(LINK) $(LFLAGS) test6 test6.o $(OLBS)
#
test7: test7.c $(INCS)
	$(CC) $(CFLAGS) test7.c
	$(LINK) $(LFLAGS) test7 test7.o $(OLBS)
#
pjl_close.o: pjl_close.c $(INCS)
	$(CC) $(CFLAGS) pjl_close.c
	$(LIBR) $(LIBS) pjl_close.o
#
pjl_core.o: pjl_core.c $(INCS)
	$(CC) $(CFLAGS) pjl_core.c
	$(LIBR) $(LIBS) pjl_core.o
#
pjl_dump_config.o: pjl_dump_config.c $(INCS)
	$(CC) $(CFLAGS) pjl_dump_config.c
	$(LIBR) $(LIBS) pjl_dump_config.o
#
pjl_dump_ustatus.o: pjl_dump_ustatus.c $(INCS)
	$(CC) $(CFLAGS) pjl_dump_ustatus.c
	$(LIBR) $(LIBS) pjl_dump_ustatus.o
#
pjl_dump_variables.o: pjl_dump_variables.c $(INCS)
	$(CC) $(CFLAGS) pjl_dump_variables.c
	$(LIBR) $(LIBS) pjl_dump_variables.o
#
pjl_echo.o: pjl_echo.c $(INCS)
	$(CC) $(CFLAGS) pjl_echo.c
	$(LIBR) $(LIBS) pjl_echo.o
#
pjl_eoj.o: pjl_eoj.c $(INCS)
	$(CC) $(CFLAGS) pjl_eoj.c
	$(LIBR) $(LIBS) pjl_eoj.o
#
pjl_inquire.o: pjl_inquire.c $(INCS)
	$(CC) $(CFLAGS) pjl_inquire.c
	$(LIBR) $(LIBS) pjl_inquire.o
#
pjl_jobname.o: pjl_jobname.c $(INCS)
	$(CC) $(CFLAGS) pjl_jobname.c
	$(LIBR) $(LIBS) pjl_jobname.o
#
pjl_language.o: pjl_language.c $(INCS)
	$(CC) $(CFLAGS) pjl_language.c
	$(LIBR) $(LIBS) pjl_language.o
#
pjl_open.o: pjl_open.c $(INCS)
	$(CC) $(CFLAGS) pjl_open.c
	$(LIBR) $(LIBS) pjl_open.o
#
pjl_rdymsg.o: pjl_rdymsg.c $(INCS)
	$(CC) $(CFLAGS) pjl_rdymsg.c
	$(LIBR) $(LIBS) pjl_rdymsg.o
#
pjl_ustatus.o: pjl_ustatus.c $(INCS)
	$(CC) $(CFLAGS) pjl_ustatus.c
	$(LIBR) $(LIBS) pjl_ustatus.o
#
pjl_ustatusoff.o: pjl_ustatusoff.c $(INCS)
	$(CC) $(CFLAGS) pjl_ustatusoff.c
	$(LIBR) $(LIBS) pjl_ustatusoff.o
#
pjl_set_debug.o: pjl_set_debug.c $(INCS)
	$(CC) $(CFLAGS) pjl_set_debug.c
	$(LIBR) $(LIBS) pjl_set_debug.o
#
pjl_load_config.o: pjl_load_config.c $(INCS)
	$(CC) $(CFLAGS) pjl_load_config.c
	$(LIBR) $(LIBS) pjl_load_config.o
#
pjl_load_variables.o: pjl_load_variables.c $(INCS)
	$(CC) $(CFLAGS) pjl_load_variables.c
	$(LIBR) $(LIBS) pjl_load_variables.o
#
pjl_load_ustatus.o: pjl_load_ustatus.c $(INCS)
	$(CC) $(CFLAGS) pjl_load_ustatus.c
	$(LIBR) $(LIBS) pjl_load_ustatus.o
#
pjl_start.o: pjl_start.c $(INCS)
	$(CC) $(CFLAGS) pjl_start.c
	$(LIBR) $(LIBS) pjl_start.o
#
pjl_stop.o: pjl_stop.c $(INCS)
	$(CC) $(CFLAGS) pjl_stop.c
	$(LIBR) $(LIBS) pjl_stop.o
#
pjl_load_model.o: pjl_load_model.c $(INCS)
	$(CC) $(CFLAGS) pjl_load_model.c
	$(LIBR) $(LIBS) pjl_load_model.o
#
pjl_get_model.o: pjl_get_model.c $(INCS)
	$(CC) $(CFLAGS) pjl_get_model.c
	$(LIBR) $(LIBS) pjl_get_model.o
#
# eof
#

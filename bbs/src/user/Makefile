#
# File: Makefile
# Date: 08-Jan-2021
# By  : Kevin Esteb
#
# Makefile to build the user libraries.
#
MAJOR=1
MINOR=0
PATCH=0
VERSION="$(MAJOR).$(MINOR).$(PATCH)"
#
CC = gcc
LINK = gcc
LIBR = ar rs
LIBS = ../libs/
WORKING = $(shell pwd)
STATIC = libuser.a
INCLUDE = -I ../../../
CFLAGS = -DVERSION=\"$(VERSION)\" -Wall -c $(INCLUDE)
LOPTS = -o 
SOPTS = -shared -o 
SFLAGS = -L$(WORKING) -Wl,-rpath=$(WORKING) -fPIC 
#
OLBS = $(STATIC) ../../../libs/librms.a ../../../libs/libtracer.a ../../../libs/liberrors.a ../../../libs/libfiles.a ../../../libs/libobjects.a ../../../libs/libcclibs.a ../../../libs/libgpl.a 
#
INCS = user.h profile.h
#
OBJS = user.o profile.o
#
all: $(OBJS)
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f $(LIBS)libuser.a
	rm -f $(LIBS)libuser.so.*
	rm -f *.3
#
user.o: user.c $(INCS)
	$(CC) $(CFLAGS) user.c
	$(LIBR) $(LIBS)libuser.a user.o
#
profile.o: profile.c $(INCS)
	$(CC) $(CFLAGS) profile.c
	$(LIBR) $(LIBS)libuser.a profile.o
#
man:
	pod2man -c " " -r "user(3)" -s 3 user.pod user.3
	pod2man -c " " -r "profile(3)" -s 3 profile.pod profile.3
#
shared:
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) user.o user.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) profile.o profile.c
	$(LINK) $(SOPTS) $(LIBS)libuser.so.$(VERSION) user.o profile.o
#
# eof
#

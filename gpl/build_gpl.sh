#!/bin/sh
#
# File: build_gpl.com
# Date: 05-Feb-2020
# By  : Kevin Esteb
#
# This procedure will rebuild libgpl.a.
#
#
 ar -c ../libs/libgpl.a
#
 cd crlf
 echo "Making the CRLF routines..."
 make -f Makefile.linux clean
 make -f Makefile.linux
#
 cd ../drs
 echo "Making the DRS routines..."
 make -f Makefile.linux clean
 make -f Makefile.linux
#
 cd ../fnm
 echo "Making the FNM routines..."
 make -f Makefile.linux clean
 make -f Makefile.linux
#
 cd ../get
 echo "Making the GET routines..."
 make -f Makefile.linux clean
 make -f Makefile.linux
#
 cd ../hash
 echo "Making the HASH routines..."
 make -f Makefile.linux clean
 make -f Makefile.linux
#
 cd ../lfn
 echo "Making the LFN routines..."
 make -f Makefile.linux clean
 make -f Makefile.linux
#
 cd ../meo
 echo "Making the MEO routines..."
 make -f Makefile.linux clean
 make -f Makefile.linux
#
 cd ../misc
 echo "Making the MISC routines..."
 make -f Makefile.linux clean
 make -f Makefile.linux
#
 cd ../net
 echo "Making the NET routines..."
 make -f Makefile.linux clean
 make -f Makefile.linux
#
 cd ../nix
 echo "Making the NIX routines..."
 make -f Makefile.linux clean
 make -f Makefile.linux
#
 cd ../opt
 echo "Making the OPT routines..."
 make -f Makefile.linux clean
 make -f Makefile.linux
#
 cd ../port
 echo "Making the PORT routines..."
 make -f Makefile.linux clean
 make -f Makefile.linux
#
 cd ../rex
 echo "Making the REX routines..."
 make -f Makefile.linux clean
 make -f Makefile.linux
#
 cd ../skt
 echo "Making the SKT routines..."
 make -f Makefile.linux clean
 make -f Makefile.linux
#
 cd ../str
 echo "Making the STR routines..."
 make -f Makefile.linux clean
 make -f Makefile.linux
#
 cd ../tcp
 echo "Making the TCP routines..."
 make -f Makefile.linux clean
 make -f Makefile.linux
#
 cd ../tv
 echo "Making the TV routines..."
 make -f Makefile.linux clean
 make -f Makefile.linux
#
 cd ../xqt
 echo "Making the XQT routines..."
 make -f Makefile.linux clean
 make -f Makefile.linux
#
 cd ..
 ranlib ../libs/libgpl.a
#

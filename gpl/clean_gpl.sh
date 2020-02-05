#!/bin/sh
#
# File: clean_gpl.com
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
#
 cd ../drs
 echo "Making the DRS routines..."
 make -f Makefile.linux clean
#
 cd ../fnm
 echo "Making the FNM routines..."
 make -f Makefile.linux clean
#
 cd ../get
 echo "Making the GET routines..."
 make -f Makefile.linux clean
#
 cd ../hash
 echo "Making the HASH routines..."
 make -f Makefile.linux clean
#
 cd ../lfn
 echo "Making the LFN routines..."
 make -f Makefile.linux clean
#
 cd ../meo
 echo "Making the MEO routines..."
 make -f Makefile.linux clean
#
 cd ../net
 echo "Making the NET routines..."
 make -f Makefile.linux clean
#
 cd ../nix
 echo "Making the NIX routines..."
 make -f Makefile.linux clean
#
 cd ../opt
 echo "Making the OPT routines..."
 make -f Makefile.linux clean
#
 cd ../port
 echo "Making the PORT routines..."
 make -f Makefile.linux clean
#
 cd ../rex
 echo "Making the REX routines..."
 make -f Makefile.linux clean
#
 cd ../skt
 echo "Making the SKT routines..."
 make -f Makefile.linux clean
#
 cd ../str
 echo "Making the STR routines..."
 make -f Makefile.linux clean
#
 cd ../tcp
 echo "Making the TCP routines..."
 make -f Makefile.linux clean
#
 cd ../tv
 echo "Making the TV routines..."
 make -f Makefile.linux clean
#
 cd ../xqt
 echo "Making the XQT routines..."
 make -f Makefile.linux clean
#
 cd ..
 ranlib ../libs/libgpl.a
#

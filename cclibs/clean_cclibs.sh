#!/bin/sh
#
# File: clean_cclibs.com
# Date: 05-Feb-2020
# By  : Kevin Esteb
#
# This procedure will clean libcclibs.a.
#
#
 ar -c ../libs/libcclibs.a
#
 cd cfg
 echo "Making the CFG routines..."
 make -f Makefile.linux clean
#
 cd ../cmd
 echo "Making the CMD routines..."
 make -f Makefile.linux clean
#
 cd ../lpd
 echo "Making the LPD routines..."
 make -f Makefile.linux clean
#
 cd ../misc
 echo "Making the MISC routines..."
 make -f Makefile.linux clean
#
 cd ../que
 echo "Making the QUE routines..."
 make -f Makefile.linux clean
#
 cd ..
 ranlib ../libs/libcclibs.a
#

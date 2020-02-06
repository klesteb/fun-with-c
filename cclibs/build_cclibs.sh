#!/bin/sh
#
# File: build_cclibs.com
# Date: 24-May-2002
# By  : Kevin Esteb
#
# This procedure will rebuild cclibs.olb.
#
#
 ar -c libcclibs.a
#
 cd cfg
 echo "Making the CFG routines..."
 make -f Makefile.linux clean
 make -f Makefile.linux
#
 cd ../cmd
 echo "Making the CMD routines..."
 make -f Makefile.linux clean
 make -f Makefile.linux
#
 cd ../lpd
 echo "Making the LPD routines..."
 make -f Makefile.linux clean
 make -f Makefile.linux
#
 cd ../misc
 echo "Making the MISC routines..."
 make -f Makefile.linux clean
 make -f Makefile.linux
#
 cd ../que
 echo "Making the QUE routines..."
 make -f Makefile.linux clean
 make -f Makefile.linux
#
 cd ..
 ranlib libcclibs.a
#

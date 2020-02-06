$!
$! File: build_cclibs.com
$! Date: 24-May-2002
$! By  : Kevin Esteb
$!
$! This procedure will rebuild cclibs.olb.
$!
$ echo := write sys$output
$!
$ libr/create cclibs.olb
$!
$ set def [.cfg]
$ echo "Making the CFG routines..."
$ make/force all
$ make clean
$!
$ set def [-.cmd]
$ echo "Making the CMD routines..."
$ make/force all
$ make clean
$!
$ set def [-.lpd]
$ echo "Making the LPD routines..."
$ make/force all
$ make clean
$!
$ set def [-.misc]
$ echo "Making the MISC routines..."
$ make/force all
$ make clean
$!
$ set def [-.que]
$ echo "Making the QUE routines..."
$ make/force all
$ make clean
$!
$ set def [-]
$!
$ exit

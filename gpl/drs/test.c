
/*******************************************************************************

    Program to test the DRS_UTIL() functions.

    Under UNIX,
        compile and link as follows:
            % cc -g -DTEST drs_util.c -I<... includes ...> <libraries ...>
        and run with the following command line:
            % a.out <wildcard_file_spec>

    Under VxWorks,
        compile and link as follows:
            % cc -g -c -DTEST -DVXWORKS drs_util.c -I<... includes ...> \
                       -o testDRS.o
            % ld -r testDRS.o <libraries ...> -o testDRS.vx.o
        load as follows:
            -> ld <testDRS.vx.o
        and run with the following command line:
            -> testDRS.vx.o "<wildcard_file_spec>"

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <errno.h>

#include "vperror.h"
#include "drs_util.h"

#ifdef VXWORKS

testDRS (fs)
    char  *fs ;
{    /* Local variables. */

#else

main (argc, argv)
    int  argc ;
    char  *argv[] ;
{    /* Local variables. */
    char  *fs = argv[1] ;

#endif

    char  *s ;
    DirectoryScan scan;


    drs_util_debug = 1;
    vperror_print = 1;

    if (drs_create(fs, &scan)) exit(errno);

    s = drs_first(scan);
    while (s != NULL) {
        printf ("%s\n", s);
        s = drs_next(scan);
    }

    drs_destroy(scan);

}


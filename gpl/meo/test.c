
/*******************************************************************************

    Program to test the MEO_UTIL() functions.

    Under UNIX,
        compile and link as follows:
            % cc -g -DTEST meo_util.c -I<... includes ...> <libraries ...>
        and run with the following command line:
            % a.out <wildcard_file_spec>

    Under VxWorks,
        compile and link as follows:
            % cc -g -c -DTEST -DVXWORKS meo_util.c -I<... includes ...> \
                       -o test_drs.o
            % ld -r test_drs.o <libraries ...> -o test_drs.vx.o
        load as follows:
            -> ld <test_drs.vx.o
        and run with the following command line:
            -> test_drs.vx.o "<wildcard_file_spec>"

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "vperror.h"
#include "meo_util.h"

#ifdef VXWORKS

test_meo (fileName)
    char  *fileName ;
{    /* Local variables. */

#else

main (argc, argv)
    int  argc ;
    char  *argv[] ;
{    /* Local variables. */
    char  *fileName = argv[1] ;

#endif

    unsigned  char  buffer[256] ;
    long  i, numBytes ;
    void  *newBuffer ;




    meo_util_debug = 1 ;
    vperror_print = 1 ;

    for (i = 0 ;  i < sizeof buffer ;  i++)
        buffer[i] = i ;

    meo_save(buffer, sizeof buffer, fileName, 0) ;

    newBuffer = NULL ;
    meo_load(fileName, 0, &newBuffer, &numBytes) ;

    printf("\nOctal Dump:\n\n") ;
    meo_dumpo(NULL, "    ", 0, newBuffer, numBytes) ;
    printf("\nDecimal Dump:\n\n") ;
    meo_dumpd(NULL, "    ", 0, newBuffer, numBytes) ;
    printf("\nHexadecimal Dump:\n\n") ;
    meo_dumpx(NULL, "    ", 0, newBuffer, numBytes) ;
    printf("\nText Dump:\n\n") ;
    meo_dumpt(NULL, "    ", 0, newBuffer, numBytes) ;

}


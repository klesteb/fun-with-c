
/*******************************************************************************

    Program to test the XQT_UTIL functions.  This test program executes a
    series of commands and writes the output from the commands to the user's
    screen.

    Under VMS,
        compile and link as follows:
            $ CC/DEFINE=TEST xqt_util.c
            $ LINK xqt_util, <libraries>
        and run with the following command line:
            % xqt_util

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "xqt_util.h"
#include "vperror.h"

main (argc, argv)
    int  argc ;
    char  *argv[] ;
{    /* Local variables. */
    char  buffer[1024] ;
    int  i, status ;
    XqtStream  stream ;
    extern  int  vperror_print ;

    static  char  *command_list[] = {
#if defined(vms)
        "SHOW DEFAULT",
        "DIRECTORY",
        "SHOW SYSTEM/SUBPROCESS",
        "WRITE SYS$OUTPUT \"$STATUS = \", $STATUS",
#else
        "pwd",
        "ls",
        "ps",
        "echo '$STATUS =' $status",
#endif
        NULL
    } ;



    vperror_print = 1 ;
    xqt_util_debug = 1 ;

    if (xqt_open(NULL, &stream)) {
        vperror("[XQT_TEST] Error opening shell execution stream.\nxqtOpen: ") ;
        exit(errno) ;
    }

    for (i = 0 ;  command_list[i] != NULL ;  i++) {
        printf("Command: \"%s\"\n", command_list[i]) ;
        if (xqt_write(stream, "%s", command_list[i])) {
            vperror("[XQT_TEST] Error writing command to shell.\nxqtWrite: ") ;
            exit(errno) ;
        }
    }

    xqt_write(stream, NULL) ;		/* End-of-input. */

    for ( ; ; ) {
        while (xqt_poll(stream) == 0)
            sleep(1) ;
        if (xqt_poll(stream) < 0)  break ;
        if (xqt_read(stream, sizeof buffer, buffer))  break ;
        if (strncmp(buffer, "$STATUS", 7) == 0)  break ;
    }

    xqt_close(stream) ;

}


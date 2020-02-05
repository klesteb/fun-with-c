
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "tcp_util.h"
#include "lfn_util.h"
#include "vperror.h"

/*******************************************************************************

    Program to test the LFN_UTIL functions.

    Under UNIX:
    Compile and link as follows:
        % cc -DTEST lfn_util.c <libraries> -o lfn_test
    First run the server with the following command line:
        % lfn_test server <name> &
    Then, run the client (preferably in a separate window):
        % lfn_test client <name>
    The client sends 16 messages to the server and the server
    reads them.

    Under VxWorks:
    Compile and link as follows:
        % cc -DTEST lfn_util.c <libraries> -o lfn_test.vx.o
    First, load and run the server with the following commands:
        -> ld <lfn_test.vx.o
        -> sp lfn_test, "server <name>"
    Then, load (to be on the safe side) and run the client:
        -> ld <lfn_test.vx.o
        -> sp lfn_test, "client <name>"
    The client sends 16 messages to the server and the server
    reads them.

*******************************************************************************/

#ifdef VXWORKS

    void  lfn_test (
    char  *commandLine)

#else

    main (argc, argv)
    int  argc ;
    char  *argv[] ;

#endif

{    /* Local variables. */
    char  *string ;
    int  i ;
    TcpEndpoint  connection, listeningPoint ;
    LfnStream  stream ;




#ifdef VXWORKS
    char  **argv ;
    int  argc ;
        /* Parse command string into an ARGC/ARGV array of arguments. */
    opt_create_argv ("lfn_test", commandLine, &argc, &argv) ;
#endif

    vperror_print = 1 ;
    tcp_util_debug = 1 ;
    lfn_util_debug = 1 ;

    if (argc < 3) {
    fprintf (stderr, "Usage:  lfn_test client|server <name>\n") ;
    exit (EINVAL) ;
    }

    if (strcmp (argv[1], "client") == 0) {      /* The Client? */

        tcp_call (argv[2], 0, 0, &connection) ;
        lfn_create (connection, "-buffer 1024 -debug", &stream) ;
        for (i = 0 ;  i < 16 ;  i++) {
            lfn_putline (stream, -1.0, "Message #%d\n", i) ;
        }

        lfn_destroy (stream) ;

    } else {                                    /* The Server. */

        tcp_listen (argv[2], 99, &listeningPoint) ;
        tcp_answer (listeningPoint, -1.0, &connection) ;
        lfn_create (connection, "-buffer 1024 -debug", &stream) ;
        for ( ; ; ) {
            if (lfn_getline (stream, -1.0, &string)) {
                vperror ("Error reading from connection on %s.\nlfn_getline: ",
                          argv[2]) ;
                break ;
            }
            printf ("[SERVER] Input: \"%s\"\n", string) ;
        }

        lfn_destroy (stream) ;

    }

}


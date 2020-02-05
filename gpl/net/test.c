

/*******************************************************************************

    Program to test the NET_UTIL functions.

    Under UNIX:
        Compile, link, and run as follows:
            % cc -DTEST net_util.c <libraries> -o net_test
            % net_test <port>

    Under VxWorks:
        Compile and link as follows:
            % cc -DTEST net_util.c <libraries> -o net_test.vx.o
        Load and run the server with the following commands:
            -> ld <net_test.vx.o
            -> sp net_test, "<port>"

    The test program is a basic FTP server that listens for clients
    at the specified network port.  Try connecting to it from within
    ftp(1):

        % ftp
        ftp> open <host> <port>
        ... enter username and password ...
        ftp> pwd
        ... see current directory ...
        ftp> ls
        ... list current directory ...
        ftp> close
        ... connection to server is closed ...
        ftp>

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "net_util.h"
#include "vperror.h"


#ifdef VXWORKS

    void  net_test (
        char  *commandLine)

#else

    main (argc, argv)
        int  argc ;
        char  *argv[] ;

#endif

{

#ifdef VXWORKS
    char  **argv ;
    int  argc ;
                /* Parse command string into an ARGC/ARGV array of arguments. */
    opt_create_argv ("net_test", commandLine, &argc, &argv) ;
#endif

    vperror_print = 1 ;

    printf ("Address of %s = %lX\n", argv[1], net_addrof (argv[1])) ;

    printf ("   Host of %s = %s\n",
            argv[1], net_hostof (strtoul (argv[1], NULL, 0))) ;

    printf ("   Port of %s = %d\n", argv[1], net_portof (argv[1], NULL)) ;

    exit (0) ;

}


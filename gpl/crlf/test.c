
/*******************************************************************************

    Program to test the CRLF_UTIL functions.

    Under UNIX:
        Compile, link, and run as follows:
            % cc -DTEST crlf_util.c <libraries> -o crlf_test
            % crlf_test
        You will probably want to redirect standard output to a file
        so that you can see the CR/LF sequences.

    Under VxWorks:
        Compile and link as follows:
            % cc -DTEST crlf_util.c <libraries> -o crlf_test.vx.o
        Load and run the test program with the following commands:
            -> ld <crlf_test.vx.o
            -> sp crlf_test

*******************************************************************************/

#ifdef VXWORKS

    void  crlf_test (
        char  *commandLine)

#else

    main (argc, argv)
        int  argc ;
        char  *argv[] ;

#endif

{    /* Local variables. */
    char  buffer[1024], lastChar ;




    printf ("CR/LFs to Newlines\n------------------\n\n") ;

    lastChar = '\0' ;

    memset (buffer, '@', sizeof buffer) ;
    strcpy (buffer, "") ;
    printf ("Before = \"%s\"\n", buffer) ;
    printf (" After = \"%s\"\n", crlf2nl (buffer, -1, &lastChar)) ;

    memset (buffer, '@', sizeof buffer) ;
    strcpy (buffer, "One line\rand another\r\n") ;
    printf ("Before = \"%s\"\n", buffer) ;
    printf (" After = \"%s\"\n", crlf2nl (buffer, -1, &lastChar)) ;

    memset (buffer, '@', sizeof buffer) ;
    strcpy (buffer, "First buffer\r\n\r") ;
    printf ("Before = \"%s\"\n", buffer) ;
    printf (" After = \"%s\"\n", crlf2nl (buffer, -1, &lastChar)) ;

    memset (buffer, '@', sizeof buffer) ;
    strcpy (buffer, "and the second buffer.\nOkay!") ;
    printf ("Before = \"%s\"\n", buffer) ;
    printf (" After = \"%s\"\n", crlf2nl (buffer, -1, &lastChar)) ;


    printf ("\nNewlines to CR/LFs\n------------------\n\n") ;

    memset (buffer, '@', sizeof buffer) ;
    strcpy (buffer, "") ;
    printf ("Before = \"%s\"\n", buffer) ;
    printf (" After = \"%s\"\n", nl2crlf (buffer, -1, sizeof buffer)) ;

    memset (buffer, '@', sizeof buffer) ;
    strcpy (buffer, "\n") ;
    printf ("Before = \"%s\"\n", buffer) ;
    printf (" After = \"%s\"\n", nl2crlf (buffer, -1, sizeof buffer)) ;

    memset (buffer, '@', sizeof buffer) ;
    strcpy (buffer, "One line\nand another\nOkay!") ;
    printf ("Before = \"%s\"\n", buffer) ;
    printf (" After = \"%s\"\n", nl2crlf (buffer, -1, sizeof buffer)) ;

}


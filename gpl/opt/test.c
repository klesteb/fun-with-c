
/*******************************************************************************

    Program to test the OPT_UTIL functions.

    Under VMS,
        compile and link as follows:
            $ CC/DEFINE=TEST/INCLUDE=?? opt_util.c
            $ LINK opt_util, <libraries>
        invoke with one of the following command lines:
            $ opt_util <optionString>

*******************************************************************************/

main (argc, argv)

    int  argc ;
    char  *argv[] ;

{    /* Local variables. */
    char  *argument, *string, **t_argv ;
    int  length, option, t_argc ;
    OptContext  context ;
    extern  int  vperror_print ;




    vperror_print = 1 ;
    opt_util_debug = 1 ;

    opt_init(argc, argv, 0, argv[1], &context) ;
    while (option = opt_get (context, &argument)) {
        printf ("(Index %d)  Option %d, Argument = %s\n",
                opt_index (context), option,
                (argument == NULL) ? "<nil>" : argument) ;
    }
    opt_reset (context, argc, argv) ;
    while (option = opt_get (context, &argument)) {
        printf ("(Index %d)  Option %d, Argument = %s\n",
                opt_index (context), option,
                (argument == NULL) ? "<nil>" : argument) ;
    }

    printf ("\nOption Indices:\n") ;
    for (option = -2 ;  ;  option++) {
        string = opt_name (context, option) ;
        if (strcmp (string, "") == 0)  break ;
        printf ("Option %d:\t\"%s\"\n", option, string) ;
    }

    printf ("\nInternal Command String:\n\n") ;

    string = "  program -ab xyz -name file -flag" ;
    printf ("Command String = %s\n", string) ;
    opt_create_argv (NULL, string, &t_argc, &t_argv) ;
    opt_delete_argv (t_argc, t_argv) ;

    string =
        "ab \"ab cd\" 'ab cd' ab\"cd\" \"ab\"'cd' \"ab'cd\" {ab\"Hello!\"cd}" ;
    printf ("\nCommand String = %s\n", string) ;
    opt_create_argv ("program", string, &t_argc, &t_argv) ;
    opt_delete_argv (t_argc, t_argv) ;

    exit (0) ;

}


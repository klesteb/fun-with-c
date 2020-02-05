
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 1996 by Charles A. Measday                                 */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include "opt_priv.h"

/*----------------------------------------------------------------------*/

/**/

int  opt_create_argv (

#    if __STDC__
        const  char  *program,
        const  char  *command,
        int  *argc,
        char  *(*argv[]))
#    else
        program, command, argc, argv)

        char  *program ;
        char  *command ;
        int  *argc ;
        char  *(*argv[]) ;
#    endif

{
/*
 * Function: opt_create_argv.c
 * Version : 1.0
 * Created : 12-Jul-1996
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function opt_create_argv() builds an ARGV array of arguments for a
 *    command line string.  For example, the command line
 *
 *        "program -a -o <object_file> -readonly -list <file>"
 *
 *    would be broken up into the following arguments:
 *
 *        ARGV[0] = "program"
 *        ARGV[1] = "-a"
 *        ARGV[2] = "-o"
 *        ARGV[3] = "<object_file>"
 *        ARGV[4] = "-readonly"
 *        ARGV[5] = "-list"
 *        ARGV[6] = "<file>"
 *
 *    opt_create_argv() is intended for the processing of internally-generated
 *    command strings. Given such a command string, a program would:
 *
 *        (1) Call opt_create_argv() to create an ARGV array of arguments.
 *        (2) Call opt_init() to initilize a scan of these arguments.
 *        (3) Call opt_get() for each option/argument.
 *        (4) Call opt_term() to terminate the scan.
 *        (5) Call opt_delete_argv() to delete the ARGV array built by
 *            opt_create_argv().
 *
 *    Invocation:
 *
 *        status = opt_create_argv(program, command, &argc, &argv);
 *
 *    where:
 *
 *        <program>           -I
 *            is the argument (usually the program name) assigned to ARGV[0];
 *            the first argument in the command string (see COMMAND) will be
 *            assigned to ARGV[1]. If PROGRAM is NULL, the first argument in
 *            COMMAND is assigned to ARGV[0].
 *
 *        <command>           - I
 *            is the command line string from which an ARGV array of arguments
 *            will be built.
 *
 *        <argc>              - O
 *            returns the number of arguments on the command line.
 *
 *        <argv>              - O
 *            returns a pointer to an ARGV array of arguments. This is simply
 *            an array of (CHAR *) pointers to argument strings.
 *
 *        <status>        - O
 *            returns the status of constructing the argument array, zero if
 *            no errors occurred and ERRNO otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

    char  *s;
    int  i, length;
    static  char  *quotes = "\"'{";

/*
 * Main part of function.
 */

    if (command == NULL)  command = "";

    /* Scan the command string and determine the number of arguments    */
    /* on the line.                                                     */

    i = (program == NULL) ? 0 : 1;
    length = 0;
    s = (char *)get_string(command, quotes, &length);

    while (length > 0) {

        i++;  
        s = (char *) get_string(s, quotes, &length);

    }

    *argc = i;

    /* Allocate an ARGV array to hold the arguments.                    */

    *argv = (char **)malloc((*argc + 1) * sizeof(char *));
    if (*argv == NULL) {

        vperror("(opt_create_argv) Error allocating ARGV array of %d elements.\nmalloc: ",
                 *argc);
        return(errno);

    }

    /* Scan the command string again, this time copying the arguments   */
    /* into the ARGV array.                                             */

    i = 0 ;
    if (program != NULL)  (*argv)[i++] = strdup(program);
    length = 0;
    s = (char *)get_string(command, quotes, &length);

    while (length > 0) {

        (*argv)[i++] = str_destring(s, length, quotes);
        s = (char *)get_string(s, quotes, &length);

    }

    (*argv)[i] = NULL;

    if (opt_util_debug) {

        printf("(opt_create_argv) %d arguments:\n", *argc);

        for (i = 0;  i < *argc;  i++) {

            printf("                      [%d] = \"%s\"\n", i, (*argv)[i]);

        }

    }

    return(0);

}


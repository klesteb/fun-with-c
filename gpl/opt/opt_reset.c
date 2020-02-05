
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

int  opt_reset (

#    if __STDC__
        OptContext  scan,
        int  argc,
        char  *argv[])
#    else
        scan, argc, argv)

        OptContext  scan ;
        int  argc ;
        char  *argv[] ;
#    endif

{
/*
 * Function: opt_reset.c
 * Version : 1.0
 * Created : 12-Jul-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function opt_reset() resets a command line scan so that the next call to
 *    opt_get() begins at the beginning of the command line.  A new command
 *    line can be specified by passing opt_reset() a new ARGC/ARGV set of
 *    arguments. This allows a program to process multiple command lines
 *    without having to create a new scan context for each command line.
 *
 *    Invocation:
 *
 *        status = opt_reset(scan, argc, argv);
 *
 *    where:
 *
 *        <scan>          - I
 *            is the command line scan context originally created by opt_init().
 *            If this argument is NULL, the default scan context is used.
 *
 *        <argc>          - I
 *            specifies the number of arguments in the new command line's ARGV
 *            array.
 *
 *        <argv>          - I
 *            is an array of (CHAR *) pointers to the new command line's
 *            arguments.  If this argument is NULL, opt_reset() simply resets
 *            the scan context structure's internal fields so that the existing
 *            ARGC/ARGV is rescanned.
 *
 *        <status>        - O
 *            returns the status of enabling or disabling the display of error
 *            messages, zero if no errors occurred and ERRNO otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

    int i;

/*
 * Main part of function.
 */

    if (scan == NULL) scan = defaultContext;

    if (scan == NULL) {

        errno = EINVAL;
        vperror("(opt_reset) Null scan context.\n");
        return(errno);

    }

    /* If a new command line was specified, then replace the existing   */
    /* one in the scan context structure.  If there are name options,   */
    /* then construct a new argument length table for the ARGV array;   */
    /* the length table is used by opt_get() when trying to match       */
    /* abbreviated name options.                                        */

    if (argv != NULL) {

        if (scan->numNames > 0) {

            if ((argc > scan->argc) && (scan->arglen != NULL)) {

                /* Force a new table allocation if there isn't enough   */
                /* room in the current table.                           */

                free(scan->arglen);
                scan->arglen = NULL;

            }

            if (scan->arglen == NULL) {         /* Allocate a new table. */

                scan->arglen = (int *)malloc(argc * sizeof (int));

                if (scan->arglen == NULL) {

                    vperror("(opt_reset) Error allocating array to hold %d argument lengths.\nmalloc: ",
                            argc);
                    return(errno);

                }

            }

            for (i = 0; i < argc; i++) {      /* Tabulate argument lengths. */

                scan->arglen[i] = strlen(argv[i]);

            }

        }

        scan->argc = argc;
        scan->argv = argv;

    }

    /* Reset the dynamic fields in the scan context structure so that   */
    /* the command line will be rescanned from the beginning.           */

    scan->endOfOptions = scan->argc + 1;
    scan->current = 1;
    scan->offset = 0;
    scan->optionType = none;

    if (opt_util_debug) {

        if (scan == defaultContext) {

            printf("(opt_reset) Resetting default scan.\n");

        } else printf("(opt_reset) Resetting scan %p.\n", scan);

    }

    return(0);

}

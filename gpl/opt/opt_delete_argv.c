
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

int  opt_delete_argv(

#    if __STDC__
        int  argc,
        char  *argv[])
#    else
        argc, argv)

        int  argc ;
        char  *argv[] ;
#    endif

{
/*
 * Function: opt_delete_argv.c
 * Version : 1.0
 * Created : 12-Jul-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function opt_delete_argv() deletes an ARGV array of arguments originally
 *    built by opt_create_argv().
 *
 *    Invocation:
 *
 *        status = opt_delete_argv(argc, argv);
 *
 *    where:
 *
 *        <argc>              - I
 *            Is the number of arguments in the ARGV array.
 *
 *        <argv>              - I
 *            Is an ARGV array of arguments built by opt_create_argv().
 *
 *        <status>            - O
 *            Returns the status of deleting the ARGV array, zero if no
 *            errors occurred and ERRNO otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

    int i;

/*
 * Main part of function.
 */

    /* Deallocate the individual argument strings.                      */

    for (i = 0; argv[i] != NULL; i++) {

        free(argv[i]);

    }

    /* Deallocate the ARGV array.                                       */

    free(argv);

    if (opt_util_debug)
        printf("(opt_delete_argv) Deleted %d arguments:\n", i);

    return(0);

}



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

int  opt_index (

#    if __STDC__
        OptContext  scan)
#    else
        scan)

        OptContext  scan ;
#    endif

{
/*
 * Function: opt_index.c
 * Version : 1.0
 * Created : 12-Jul-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function opt_index() returns the index of the current option/argument 
 *    in a command line scan. The index is just the current position in the 
 *    ARGV argument array and has a value between 1 and ARGC-1, inclusive.
 *
 *    Invocation:
 *
 *        index = opt_index(scan);
 *
 *    where:
 *
 *        <scan>              - I
 *            Is the command line scan context originally created by opt_init().
 *            If this argument is NULL, the default scan context is used.
 *
 *        <index>             - O
 *            Returns the index, 1..ARGC-1, of the current option/argument.
 *            Zero is returned if the command line scan is complete; -1 is
 *            returned in case of an error (e.g., no scan context).
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    if (scan == NULL) scan = defaultContext;
    if (scan == NULL) return(-1);

    if (scan->current >= scan->argc) {

        return(0);                    /* End of command line scan.      */

    } else if (scan->current < 1) {

        return(-1);                   /* Error.                         */

    } else {

        return(scan->current);        /* Index of current option/argument. */

    }

}


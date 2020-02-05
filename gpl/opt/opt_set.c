
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

int  opt_set (

#    if __STDC__
        OptContext  scan,
        int  newIndex)
#    else
        scan, newIndex)

        OptContext  scan ;
        int  newIndex ;
#    endif

{
/*
 * Function: opt_set.c
 * Version : 1.0
 * Created : 12-Jul-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function opt_set() changes the current position in a command line scan.
 *    The next argument on the command line examined by opt_get() will be
 *    that specified in the call to opt_set().
 *
 *    Invocation:
 *
 *        status = opt_set(scan, newIndex);
 *
 *    where:
 *
 *        <scan>              - I
 *            Is the command line scan context originally created by opt_init().
 *            If this argument is NULL, the default scan context is used.
 *
 *        <newIndex>          - I
 *            Specifies the index, 1..ARGC, in the ARGV array of the next
 *            argument to be examined by opt_get(). Note that an index of
 *            ARGC will cause opt_get() to return an end-of-scan indication.
 *
 *        <status>            - O
 *            Returns the status of changing the current scan position, zero
 *            if no errors occurred and ERRNO otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    if (scan == NULL)  scan = defaultContext;

    if (scan == NULL) {

        errno = EINVAL;
        vperror("(opt_set) Null scan context.\n");
        return(errno);

    }

    if ((newIndex < 1) || (newIndex > scan->argc)) {

        errno = EINVAL;
        vperror("(opt_set) Invalid index: %d\n", newIndex);
        return(errno);

    }

    /* Set the current position of the command line scan so that the    */
    /* new position will be examined on the next call to opt_get().     */

    if (newIndex < scan->endOfOptions)  scan->endOfOptions = scan->argc + 1;

    scan->current = newIndex;
    scan->offset = 0;
    scan->optionType = none;

    if (opt_util_debug) {

        if (scan == defaultContext) {

            printf("(opt_reset) New position of default scan: %d\n", newIndex);

        } else printf("(opt_reset) New position of scan %p: %d\n", scan, newIndex);

    }

    return(0);

}


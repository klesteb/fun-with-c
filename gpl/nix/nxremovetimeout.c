
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

#include "nix_priv.h"

/*----------------------------------------------------------------------*/

int  NxRemoveTimeOut(

#    if __STDC__
        NxAppContext  context,
        NxIntervalId  timer_ID)
#    else
        context, timer_ID)

        NxAppContext  context ;
        NxIntervalId  timer_ID ;
#    endif

{
/*
 * Function: NxRemoveTimeOut.c
 * Version : 1.0
 * Created : 27-Apr-1997
 * Author  : Alex Measday
 *
 * Description
 *
 *    Function NxRemoveTimeout "unregisters" a timeout timer previously
 *    registered with the NIX dispatcher.  NxRemoveTimeOut() is modeled
 *    after the X Toolkit function, "XtRemoveTimeOut()".
 *
 *    Invocation:
 *
 *        status = NxRemoveTimeOut(context, timer_ID);
 *
 *    where:
 *
 *        <context>           - I
 *            Is the application context returned by NxCreateContext().  If
 *            this argument is NULL, the default application context is used.
 *            This should be the same context used in the call to NxAddTimeOut()
 *            that registered the timer.
 *
 *        <timer_ID>          - I
 *            Is the ID assigned to the timeout timer by NxAddTimeOut().
 *
 *        <status>            - O
 *            Returns the status of "unregistering" the timer, zero if
 *            no errors occurred and ERRNO otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

    NxAppContext  app;
    NxTimer  prev, tot;

/*
 * Main part of function.
 */

    /* Use the desired application context.                             */

    if (context == NULL) {

        if ((default_context == NULL) && NxCreateContext(NULL)) {

            vperror("(NxRemoveTimeOut) Error creating default application context.\nNxCreateContext: ");
            return(errno);

        }

        context = default_context;

    }

    app = context;

    /* Locate the target timer in the list of registered timers.        */

    for (prev = NULL, tot = app->timeout_list; tot != NULL; tot = tot->next) {

        if (tot == timer_ID) break;
        prev = tot;

    }

    if (tot == NULL) {

        errno = EINVAL;
        vperror("(NxRemoveTimeOut) Timer ID %p not found.\n", timer_ID);
        return(errno);

    }

    if (app->debug)
        printf("(NxRemoveTimeOut) %g-second timeout unregistered.\n",
                tot->interval);

    /* Unlink the I/O timer structure from the list and deallocate it.  */

    if (prev == NULL) {            /* Beginning of list? */

        app->timeout_list = tot->next;

    } else {                /* Middle or end of list. */

        prev->next = tot->next;

    }

    free(tot);

    return(0);

}



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

int  NxRemoveWorkProc(

#    if __STDC__
        NxAppContext  context,
        NxWorkProcId  workproc_ID)
#    else
        context, workproc_ID)

        NxAppContext  context ;
        NxWorkProcId  workproc_ID ;
#    endif

{
/*
 * Function: NxRemoveWorkProc.c
 * Version : 1.0
 * Created : 27-Apr-1997
 * Author  : Alex Measday
 *
 * Description
 *
 *    Function NxRemoveWorkProc "unregisters" a work procedure previously
 *    registered with the NIX dispatcher.  NxRemoveWorkProc() is modeled
 *    after the X Toolkit function, "XtRemoveWorkProc()".
 *
 *    Invocation:
 *
 *        status = NxRemoveWorkProc(context, workproc_ID);
 *
 *    where:
 *
 *        <context>           - I
 *            Is the application context returned by NxCreateContext().  If
 *            this argument is NULL, the default application context is used.
 *            This should be the same context used in the call to NxAddWorkProc()
 *            that registered the work procedure.
 *
 *        <workproc_ID>       - I
 *            Is the ID assigned to the work procedure by NxAddWorkProc().
 *
 *        <status>            - O
 *            Returns the status of "unregistering" the work procedure, zero
 *            if no errors occurred and ERRNO otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

    NxAppContext  app ;
    NxBackgroundTask  bat, prev ;

/*
 * Main part of function.
 */

    /* Use the desired application context.                             */

    if (context == NULL) {

        if ((default_context == NULL) && NxCreateContext(NULL)) {

            vperror("(NxRemoveWorkProc) Error creating default application context.\nNxCreateContext: ");
            return(errno);

        }

        context = default_context;

    }

    app = context;

    /* Locate the target procedure in the queue of registered work      */
    /* procedures.                                                      */

    for (prev = NULL, bat = app->workproc_queue; bat != NULL; bat = bat->next) {

        if (bat == workproc_ID) break;
        prev = bat;

    }

    if (bat == NULL) {

        errno = EINVAL;
        vperror("(NxRemoveWorkProc) Workproc ID %p not found.\n", workproc_ID);
        return(errno);

    }

    if (app->debug)
        printf("(NxRemoveWorkProc) Workproc %p (Data %p) unregistered.\n",
               bat->workproc, bat->client_data);

    /* Unlink the background task structure from the queue and          */
    /* deallocate it.                                                   */

    if (prev == NULL) {            /* Beginning of queue? */

        app->workproc_queue = bat->next;

    } else {                /* Middle or end of queue. */

        prev->next = bat->next;

    }

    free(bat);

    return(0);

}


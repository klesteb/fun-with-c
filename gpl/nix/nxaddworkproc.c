
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

NxWorkProcId  NxAddWorkProc(

#    if __STDC__
        NxAppContext  context,
        NxWorkProc  workprocF,
        void  *client_data)
#    else
        context, workprocF, client_data)

        NxAppContext  context ;
        NxWorkProc  workprocF ;
        void  *client_data ;
#    endif

{
/*
 * Function: NxAddWorkProc.c
 * Version : 1.0
 * Created : 27-Apr-1997
 * Author  : Alex Measday
 *
 * Description
 *
 *    Function NxAddWorkProc registers a work procedure with the NIX
 *    dispatcher.  When no other event sources are active (e.g., I/O
 *    ready or timer expired), the NIX dispatcher will remove the next
 *    work procedure from the queue and call it; the work procedure
 *    effectively executes in "background" mode.  Work procedures are
 *    responsible for returning control to the NIX dispatcher in a
 *    timely fashion and for re-registering themselves if need be.
 *    NxAddWorkProc() is modeled after the X Toolkit function,
 *    "XtAppAddWorkProc()".
 *
 *    Invocation:
 *
 *        workproc_ID = NxAddWorkProc(context, callback, client_data);
 *
 *    where:
 *
 *        <context>           - I
 *            Is the application context returned by NxCreateContext().  If
 *            this argument is NULL, the default application context is used.
 *
 *        <callback>          - I
 *            Is the function ("work procedure") that is to be called when the
 *            NIX dispatcher has nothing else to do.  The callback function
 *            should be declared as follows:
 *
 *                int  callback_function(NxAppContext context,
 *                                       NxWorkProcId workproc_ID,
 *                                       void *client_data);
 *
 *            where "context" is the application context used in this call to
 *            NxAddWorkProc(); "workproc_ID" is the value returned by this call
 *            to NxAddWorkProc(); "client_data" is the argument that was passed
 *            in to NxAddWorkProc().  The return value of the callback function
 *            is ignored by the NIX dispatcher.
 *
 *        <client_data>       - I
 *            Is an arbitrary data value, cast as a (VOID *) pointer, that will
 *            be passed to the callback function when it is invoked.
 *
 *        <workproc_ID>       - O
 *            Returns an ID for the registered work procedure; NULL is returned
 *            if an error occurs.  This ID is used by NxRemoveWorkProc() to
 *            identify a work procedure being "unregistered".
 *
 * Modification History
 *
 * Variables Used
 */

    NxAppContext  app;
    NxBackgroundTask  bat, rear;

/*
 * Main part of function.
 */

    /* Use the desired application context.                             */

    if (context == NULL) {

        if ((default_context == NULL) && NxCreateContext(NULL)) {

            vperror("(NxAddWorkProc) Error creating default application context.\nNxCreateContext: ");
            return(NULL);

        }

        context = default_context;

    }

    app = context;

    /* Allocate a background task structure for the work procedure.     */

    if ((bat = (NxBackgroundTask)malloc(sizeof(_NxBackgroundTask))) == NULL) {

        vperror("(NxAddWorkProc) Error allocating background task structure.\nmalloc: ");
        return(NULL);

    }

    bat->next = NULL;
    bat->workproc = workprocF;
    bat->client_data = client_data;

    /* Add the work procedure to the queue of registered work procedures.*/

    for (rear = app->workproc_queue; rear != NULL; rear = rear->next) {

        if (rear->next == NULL) break;

    }

    if (rear == NULL) {            /* Add to empty queue? */

        app->workproc_queue = bat;

    } else {                /* Append to non-empty queue. */

        rear->next = bat;

    }

    if (app->debug)
        printf("(NxAddWorkProc) Workproc: %p, Data: %p\n", workprocF, client_data);

    return(bat);

}


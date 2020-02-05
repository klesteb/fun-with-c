
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

NxIntervalId  NxAddTimeOut(

#    if __STDC__
        NxAppContext  context,
        double  interval,
        NxTimerCallback  callbackF,
        void  *client_data)
#    else
        context, interval, callbackF, client_data)

        NxAppContext  context ;
        double  interval ;
        NxTimerCallback  callbackF ;
        void  *client_data ;
#    endif

{
/*
 * Function: NxAddTimeOut.c
 * Version : 1.0
 * Created : 27-Apr-1997
 * Author  : Alex Measday
 *
 * Description
 *
 *    Function NxAddTimeOut registers a timeout timer with the NIX dispatcher;
 *    when the timer expires, a caller-specified "callback" function is 
 *    invoked to respond to the timeout.  At a minimum, the specified time 
 *    interval will elapse before the callback function is called; there is 
 *    no guarantee on how soon after the timer fires that the callback will 
 *    be called.  The timer is automatically deleted after the timeout 
 *    interval expires, so periodic timers must be explicitly re-registered.  
 *    NxAddTimeOut() is modeled after the X Toolkit function, 
 *    "XtAppAddTimeOut()".
 *
 *    Invocation:
 *
 *        timer_ID = NxAddTimeOut(context, interval, callback, client_data);
 *
 *    where:
 *
 *        <context>           - I
 *            Is the application context returned by NxCreateContext().  If
 *            this argument is NULL, the default application context is used.
 *
 *        <interval>          - I
 *            Specifies the timeout interval in seconds.  (This is a real
 *            number, so fractions of a second can be specified.)
 *
 *        <callback>          - I
 *            Is the function that is to be called when the timeout interval
 *            expires.  The callback function should be declared as follows:
 *
 *                int  callback_function(NxAppContext context,
 *                                       NxIntervalId timer_ID,
 *                                       void *client_data);
 *
 *            where "context" is the application context used in this call to
 *            NxAddTimeOut(); "timer_ID" is the value returned by this call to
 *            NxAddTimeOut(); "client_data" is the argument that was passed in
 *            to NxAddTimeOut().  The return value of the callback function is
 *            ignored by the NIX dispatcher.
 *
 *        <client_data>       - I
 *            Is an arbitrary data value, cast as a (VOID *) pointer, that will
 *            be passed to the callback function when it is invoked.
 *
 *        <timer_ID>          - O
 *            Returns an ID for the registered timer; NULL is returned if
 *            an error occurs.  This handle is used by NxRemoveTimeOut() to
 *            identify a timer being "unregistered".
 *
 * Modification History
 *
 * Variables Used
 */

    NxAppContext  app;
    NxTimer  next, prev, tot;

/*
 * Main part of function.
 */

    /* Use the desired application context.                             */

    if (context == NULL) {

        if ((default_context == NULL) && NxCreateContext(NULL)) {

            vperror("(NxAddTimeOut) Error creating default application context.\nNxCreateContext: ");
            return(NULL);

        }

        context = default_context;

    }

    app = context;

    /* Allocate a timeout structure for the timer.                      */

    if ((tot = (NxTimer)malloc(sizeof(_NxTimer))) == NULL) {

        vperror("(NxAddTimeOut) Error allocating timeout structure.\nmalloc: ");
        return(NULL);

    }

    tot->interval = interval;

    /* Expiration time = current time-of-day + timeout interval.        */

    tot->expiration = tv_add(tv_tod(), tv_createf(interval));
    tot->callback = callbackF;
    tot->client_data = client_data;

    /* Add the timer to the list of registered timers. The list is      */
    /* sorted by expiration time.                                       */

    prev = NULL;
    next = app->timeout_list;

    while (next != NULL) {

        if (tv_compare(tot->expiration, next->expiration) < 0) break;
        prev = next;
        next = next->next;

    }

    if (prev == NULL) {             /* Beginning of list?               */

        tot->next = app->timeout_list;
        app->timeout_list = tot;

    } else {                        /* Middle or end of list.           */

        tot->next = next;
        prev->next = tot;

    }

    if (app->debug) {

        printf("(NxAddTimeOut) %g-second timeout; expiration time: %s\n",
                interval, tv_show(tot->expiration, 0, NULL));

    }

    return(tot);

}


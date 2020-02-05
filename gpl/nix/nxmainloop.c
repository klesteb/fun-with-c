
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

int nix_util_debug = 0;          /* Global debug switch (1/0 = yes/no). */
NxAppContext default_context = NULL;

/*----------------------------------------------------------------------*/

int  NxMainLoop(

#    if __STDC__
        NxAppContext  context)
#    else
        context)

        NxAppContext  context ;
#    endif

{
/*
 * Function: NxMainLoop.c
 * Version : 1.0
 * Created : 27-Apr-1997
 * Author  : Alex Measday
 *
 * Description
 *
 *    Function NxMainLoop monitors I/O events and, when they occur, invokes the
 *    event handlers ("callback" functions) defined for the events.  The types
 *    of events handled by NxMainLoop include I/O events registered by calls to
 *    NxAddInput() and timeout events registered by calls to NxAddTimeout().
 *    NxMainLoop() is modeled after - you guessed it - the X Toolkit function,
 *    "XtAppMainLoop()".
 *
 *    Once called, NxMainLoop() never returns.  Well, almost never.  
 *    NxMainLoop() only returns if:
 *
 *    - there are no more I/O events to monitor, no timeouts to be timed,
 *      and no work procedures to be executed, or if
 *
 *    - an error occurs while polling an I/O source.  This might occur if a
 *       closed I/O source is not removed from the NIX dispatcher's I/O source
 *       list.
 *
 *    In either case, NxMainLoop() returns an error indication to the calling
 *    program.
 *
 *    Invocation:
 *
 *        status = NxMainLoop(context);
 *
 *    where:
 *
 *        <context>           - I
 *            Is the application context returned by NxCreateContext().  If
 *            this argument is NULL, the default application context is used.
 *            This should be the same context used to register the I/O sources,
 *            timers, and work procedures.
 *
 *        <status>            - O
 *            Returns the status (ERRNO) of monitoring the registered events.
 *            NxMainLoop() only returns in the event of an unrecoverable
 *            error or if there are no more I/O sources or timers to monitor
 *            and no work procedures to execute.
 *
 * Modification History
 *
 * Variables Used
 */

    NxAppContext  app;
    fd_set  except_mask, except_mask_save;
    fd_set  read_mask, read_mask_save;
    fd_set  write_mask, write_mask_save;
#ifdef VMS
    float  f_timeout;
#endif
    int  numActive;
    NxBackgroundTask  bat;
    NxIOSource  ios;
    NxTimer  tot;
    struct  timeval  timeout;

/*
 * Main part of function.
 */

    /* Use the desired application context.                             */

    if (context == NULL) {

        if ((default_context == NULL) && NxCreateContext(NULL)) {

            vperror("(NxMainLoop) Error creating default application context.\nNxCreateContext: ");
            return(errno);

        }

        context = default_context;

    }

    app = context;

    /*------------------------------------------------------------------*/
    /* Loop forever, "listening" for and responding to I/O events and   */
    /* timeouts. When a monitored I/O event is detected, invoke the     */
    /* callback function bound by NxAddInput() to the source of the     */
    /* event.  When a timeout interval expires, invoke the callback     */
    /* function bound by NxAddTimeOut() to the timer.                   */
    /*------------------------------------------------------------------*/

    for (;;) {

        /* Construct the SELECT(2) masks for the I/O sources being      */
        /* monitored.                                                   */

        FD_ZERO(&read_mask_save);
        FD_ZERO(&write_mask_save);
        FD_ZERO(&except_mask_save);
        numActive = 0;

        for (ios = app->IO_source_list; ios != NULL; ios = ios->next) {

            if (ios->condition & NxInputReadMask) {

                FD_SET(ios->source, &read_mask_save);
                numActive++;

            }

            if (ios->condition & NxInputWriteMask) {

                FD_SET(ios->source, &write_mask_save);
                numActive++;

            }

            if (ios->condition & NxInputExceptMask) {

                FD_SET(ios->source, &except_mask_save);
                numActive++;

            }

        }

        if ((numActive == 0) && (app->timeout_list == NULL) &&
            (app->workproc_queue == NULL)) {

            errno = EINVAL;
            vperror("(NxMainLoop) No I/O sources or timeouts to monitor.\n");
            return(errno);

        }

        /* Wait for an I/O event to occur or for the timeout interval   */
        /* to expire.                                                   */

        for (;;) {

            read_mask = read_mask_save;
            write_mask = write_mask_save;
            except_mask = except_mask_save;

            if (app->workproc_queue != NULL)     {    /* Idle tasks to run? */

                if (numActive > 0) {

                    timeout.tv_sec = timeout.tv_usec = 0;
                    numActive = select(FD_SETSIZE, &read_mask, &write_mask,
                                       &except_mask, &timeout);

                }

            } else if (app->timeout_list == NULL) {    /* Wait forever. */

                numActive = select(FD_SETSIZE, &read_mask, &write_mask,
                                   &except_mask, NULL);

            } else {        /* Wait for I/O or until timeout expires. */

                timeout = tv_subtract((app->timeout_list)->expiration,
                                      tv_tod());
#ifdef VMS

                if (numActive > 0) {

                    numActive = select(FD_SETSIZE, &read_mask, &write_mask,
                                       &except_mask, &timeout);
                } else {

                    /* VMS doesn't allow SELECT(2)ing when no bits are  */
                    /* set in the masks, so LIB$WAIT() is used for      */
                    /* timeouts.                                        */

                    f_timeout = (float)timeout.tv_sec +
                                (timeout.tv_usec / 1000000.0);
                    LIB$WAIT(&f_timeout);
                    numActive = 0;

                }

#else
                numActive = select(FD_SETSIZE, &read_mask, &write_mask,
                                   &except_mask, &timeout);
#endif

            }

            if (numActive >= 0) break;
            if (errno == EINTR) continue;    /* Retry on signal interrupt. */
            vperror("(NxMainLoop) Error monitoring I/O sources.\nselect: ");
            return(errno);

        }

        /* Scan the SELECT(2) bit masks.  For each I/O event detected,  */
        /* invoke the callback function bound to that event and its     */
        /* source.  In case a callback modifies the list of monitored   */
        /* I/O events (e.g., unregistering a related connection), the   */
        /* callback's source is cleared in the SELECT(2) bit masks      */
        /* and the scan begins all over again.  Note that, if a single  */
        /* callback is bound to an ORed mask of events and two or more  */
        /* of the events are simultaneously detected (e.g.,             */
        /* input-available and output-ready), the callback is only      */
        /* invoked once; the callback is responsible, in this case, for */
        /* checking for both events.                                    */

        ios = app->IO_source_list;

        while (ios != NULL) {

            if (((ios->condition & NxInputReadMask) &&
                 FD_ISSET (ios->source, &read_mask)) ||
                ((ios->condition & NxInputWriteMask) &&
                 FD_ISSET (ios->source, &write_mask)) ||
                ((ios->condition & NxInputExceptMask) &&
                 FD_ISSET (ios->source, &except_mask))) {

                FD_CLR(ios->source, &read_mask);
                FD_CLR(ios->source, &write_mask);
                FD_CLR(ios->source, &except_mask);

                ios->callback((void *) app, (void *) ios,
                              ios->source, ios->client_data);
                ios = app->IO_source_list;        /* Re-scan list. */

            } else {

                ios = ios->next;            /* Next item in list. */

            }

        }

        /* If a timeout timer has fired, invoke the callback function   */
        /* bound to the timeout event.  Since the timer list is sorted  */
        /* by expiration time, only the first entry in the timer list   */
        /* needs to be examined.                                        */

        tot = app->timeout_list;

        if ((tot != NULL) && (tv_compare(tv_tod(), tot->expiration) >= 0)) {

            app->timeout_list = tot->next;
            tot->callback((void *)app, (void *)tot, tot->client_data);
            free(tot);
            continue;            /* In case the callback modified
                       the list of monitored events. */

        }

        /* If no I/O sources were active and no timers expired, then    */
        /* execute the next background task.                            */

        if (app->workproc_queue != NULL) {

            bat = app->workproc_queue;
            app->workproc_queue = bat->next;
            bat->workproc((void *) app, bat, bat->client_data);
            free(bat);

        }

    }     /* Loop forever */

}


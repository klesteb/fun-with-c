#ifdef VMS
/*******************************************************************************

Procedure:

    NxMainLoopEF ()

    Handle I/O Events using Event Flags.  (VMS Only)


Purpose:

    Function NXMAINLOOPEF, like NXMAINLOOP(), monitors I/O events and, when
    they occur, invokes the event handlers ("callback" functions) defined for
    the events.  Unlike NXMAINLOOP(), which uses the UNIX SELECT(2) mechanism
    to monitor I/O sources, NXMAINLOOPEF() uses VMS event flags to monitor the
    I/O sources.  Under UNIX, SELECT(2) works for any type of I/O source; the
    VMS version of SELECT(2), however, only works for network connections.
    One way of surmounting this limitation is to monitor event flags triggered
    by I/O activity instead of calling SELECT(2).  The event flag approach is
    used by the DECWindows implementation of "XtAddInput()" and "XtMainLoop()";
    NXMAINLOOPEF() makes this approach available to NIX-based programs.


    Invocation:

        status = NxMainLoopEF (context, timer_ef) ;

    where:

        <context>	- I
            is the application context returned by NXCREATECONTEXT().  If
            this argument is NULL, the default application context is used.
            This should be the same context used to register the I/O sources,
            timers, and work procedures.
       <timer_ef>	- I
            specifies an event flag (0-23) to be used by NXMAINLOOPEF() for
            monitoring timeout timers.  The calling routine is responsible for
            reserving the event flag, if necessary, and ensuring that the flag
            is not used elsewhere in the program.  If timers are not used (i.e.,
            NXADDTIMEOUT() is not called), then this argument is also not used.
        <status>	- O
            returns the status (ERRNO) of monitoring the registered events.
            NXMAINLOOPEF() only returns in the event of an unrecoverable
            error or if there are no more I/O sources or timers to monitor
            and no work procedures to execute.

*******************************************************************************/


int  NxMainLoopEF (

#    if __STDC__
        NxAppContext  context,
        int  timer_ef)
#    else
        context, timer_ef)

        NxAppContext  context ;
        int  timer_ef ;
#    endif

{    /* Local variables. */
    NxAppContext  app ;
    float  f_timeout ;
    int  status ;
    long  delta_time[2], event_flag_mask, operation ;
    NxBackgroundTask  bat ;
    NxIOSource  ios, next ;
    NxTimer  tot ;
    struct  timeval  timeout ;




/* Use the desired application context. */

    if (context == NULL) {
        if ((default_context == NULL) && NxCreateContext (NULL)) {
            vperror ("(NxMainLoopEF) Error creating default application context.\nNxCreateContext: ") ;
            return (errno) ;
        }
        context = default_context ;
    }
    app = context ;


/*******************************************************************************
    Loop forever, "listening" for and responding to I/O events and timeouts.
    When a monitored I/O event is detected, invoke the callback function
    bound by NXADDINPUT() to the source of the event.  When a timeout
    interval expires, invoke the callback function bound by NXADDTIMEOUT()
    to the timer.
*******************************************************************************/


    for ( ; ; ) {


/* Construct the event flag mask for the I/O sources being monitored. */

        event_flag_mask = 0 ;

        for (ios = app->IO_source_list ;  ios != NULL ;  ios = ios->next)
            event_flag_mask = event_flag_mask | (1 << ios->source) ;

        if ((event_flag_mask == 0) && (app->timeout_list == NULL) &&
            (app->workproc_queue == NULL)) {
            errno = EINVAL ;
            vperror ("(NxMainLoopEF) No I/O sources or timeouts to monitor.\n") ;
            return (errno) ;
        }


/* If the calling routine has set a timer via NXADDTIMEOUT(), then set a
   VMS timer to go off at that time.  When the VMS timer fires, it sets the
   caller-specified timer event flag. */

        if ((app->timeout_list != NULL) && (app->workproc_queue == NULL)) {

				/* Add timer event flag to event flag mask. */
            event_flag_mask = event_flag_mask | (1 << timer_ef) ;

				/* Compute time until timer expiration. */
            timeout = tv_subtract ((app->timeout_list)->expiration, tv_tod ()) ;
            f_timeout = (float) timeout.tv_sec +
                        (timeout.tv_usec / 1000000.0) ;

				/* Convert to internal VMS time format. */
            operation = LIB$K_DELTA_SECONDS_F ;
            status = LIB$CVTF_TO_INTERNAL_TIME (&operation, &f_timeout,
                                                delta_time) ;
            if (f_timeout <= 0.0) {	/* 0.0 is an invalid delta time. */
                delta_time[0] = 0 ;
                delta_time[1] = 0 ;
            } else if (!(status & STS$M_SUCCESS)) {
                errno = EVMSERR ;  vaxc$errno = status ;
                vperror ("(NxMainLoopEF) Error converting time (%d secs, %d usecs).\nLIB$CVTF_TO_INTERNAL_TIME: ",
                         timeout.tv_sec, timeout.tv_usec) ;
                return (errno) ;
            }
				/* Set up a VMS timer. */
            status = SYS$SETIMR (timer_ef, delta_time, NULL, 0, 0) ;
            if (!(status & STS$M_SUCCESS)) {
                errno = EVMSERR ;  vaxc$errno = status ;
                vperror ("(NxMainLoopEF) Error setting timer.\nSYS$SETIMR: ") ;
                return (errno) ;
            }

        }


/* Wait for an I/O source's event flag to be set (indicating I/O activity)
   or for the timeout timer's event flag to be set. */

        if (app->workproc_queue == NULL) {
            status = SYS$WFLOR (0, event_flag_mask) ;
            if (!(status & STS$M_SUCCESS)) {
                errno = EVMSERR ;  vaxc$errno = status ;
                vperror ("(NxMainLoopEF) Error monitoring event flags (0x%08.8lX).\nSYS$WFLOR: ",
                         event_flag_mask) ;
                return (errno) ;
            }
        }


/* Get the current state of the event flags being monitored. */

        status = SYS$READEF (0, &event_flag_mask) ;
        if (!(status & STS$M_SUCCESS)) {
            errno = EVMSERR ;  vaxc$errno = status ;
            vperror ("(NxMainLoopEF) Error reading event flags.\nSYS$READEF: ") ;
            return (errno) ;
        }


/* Scan the event flag mask.  For each flag set (indicating an I/O event),
   invoke the callback function bound to that event flag's source.  In case
   a callback modifies the list of monitored I/O events (e.g., unregistering
   a related connection), the callback's event flag is cleared in the mask
   and the scan begins all over again. */

        ios = app->IO_source_list ;
        while (ios != NULL) {
            if (event_flag_mask & (1 << ios->source)) {
                event_flag_mask &= ~(1 << ios->source) ;
                ios->callback ((void *) app, (void *) ios,
                               ios->source, ios->client_data) ;
                ios = app->IO_source_list ;		/* Re-scan list. */
            } else {
                ios = ios->next ;			/* Next item in list. */
            }
        }


/* If a timeout timer has fired, invoke the callback function bound to the
   timeout event.  Since the timer list is sorted by expiration time, only
   the first entry in the timer list needs to be examined. */

        tot = app->timeout_list ;
        if ((tot != NULL) && (tv_compare (tv_tod (), tot->expiration) >= 0)) {
            app->timeout_list = tot->next ;
            tot->callback ((void *) app, (void *) tot, tot->client_data) ;
            free (tot) ;
            continue ;			/* In case the callback modified
					   the list of monitored events. */
        }


/* If no I/O sources were active and no timers expired, then execute the
   next background task. */

        if (app->workproc_queue != NULL) {
            bat = app->workproc_queue ;
            app->workproc_queue = bat->next ;
            bat->workproc ((void *) app, bat, bat->client_data) ;
            free (bat) ;
        }


    }     /* Loop forever */

}
#endif /* VMS */

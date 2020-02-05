
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

NxInputId  NxAddInput(

#    if __STDC__
        NxAppContext  context,
        int  source,
        NxInputMask  condition,
        NxInputCallback  callbackF,
        void  *client_data)
#    else
        context, source, condition, callbackF, client_data)

        NxAppContext  context ;
        int  source ;
        NxInputMask  condition ;
        NxInputCallback  callbackF ;
        void  *client_data ;
#    endif

{
/*
 * Function: NxAddInput.c
 * Version : 1.0
 * Created : 27-Apr-1997
 * Author  : Alex Measday
 *
 * Description
 *
 *    Function NxAddInput registers an I/O source with the NIX dispatcher;
 *    when an I/O event is detected at the source, a caller-specified "callback"
 *    function is invoked to respond to the event (e.g., to read data from an
 *    input source).  NxAddInput() is modeled after the X Toolkit function,
 *    "XtAppAddInput()".
 *
 *    Invocation:
 *
 *        source_ID = NxAddInput(context, source, type, callback, client_data);
 *
 *    where:
 *
 *        <context>           - I
 *            is the application context returned by NxCreateContext().  If
 *            this argument is NULL, the default application context is used.
 *
 *        <source>            - I
 *            is an operating system-dependent object used to monitor the
 *            I/O source.  Under UNIX, this is simply the I/O source's file
 *            descriptor.
 *
 *        <type>              - I
 *            is an operating system-dependent entity.  Under UNIX, this
 *            argument is a bit mask specifying the type of I/O event being
 *            monitored; the value of this mask is the bit-wise ORing of
 *            one or more of the following: NxInputReadMask (input events),
 *            NxInputWriteMask (output events), and NxInputExceptMask
 *            (exceptional events).  (An "exceptional" I/O event, as I
 *            only just found out, is the detection of out-of-band input
 *            on a network connection.)
 *
 *        <callback>          - I
 *            is the function that is to be called when a monitored event is
 *            detected at the I/O source.  The callback function should be
 *            declared as follows:
 *
 *                int  callback_function(NxAppContext context,
 *                                       NxInputId source_ID,
 *                                       int source,
 *                                       void *client_data);
 *
 *            where "context" is the application context used in this call to
 *            NxAddInput(); "source_ID" is the value returned by this call to
 *            NxAddInput(); "source" and "client_data" are the arguments that
 *            were passed in to NxAddInput().  The return value of the callback
 *            function is ignored by the NIX dispatcher.
 *
 *        <client_data>       - I
 *            is an arbitrary data value, cast as a (VOID *) pointer, that will
 *            be passed to the callback function if and when it is invoked.
 *
 *        <source_ID>         - O
 *            returns an ID for the registered I/O source; NULL is returned in
 *            the event of an error.  This ID is used by NxRemoveInput() to
 *            identify an I/O source being "unregistered".
 *
 * Modification History
 *
 * Variables Used
 */

    NxAppContext  app;
    NxIOSource  ios;

/*
 * Main part of function.
 */

    /* Use the desired application context.                             */

    if (context == NULL) {

        if ((default_context == NULL) && NxCreateContext(NULL)) {

            vperror("(NxAddInput) Error creating default application context.\nNxCreateContext: ");
            return(NULL);

        }

        context = default_context;

    }

    app = context;

    /* Allocate an I/O source structure for the I/O source.             */

    if ((ios = (NxIOSource)malloc(sizeof(_NxIOSource))) == NULL) {

        vperror("(NxAddInput) Error allocating I/O source structure.\nmalloc: ") ;
        return(NULL);

    }

    ios->source = source;
    ios->condition = condition;
    ios->callback = callbackF;
    ios->client_data = client_data;

    /* Add the I/O source to the list of registered sources.            */

    ios->next = app->IO_source_list;
    app->IO_source_list = ios;

    if (app->debug)
        printf("(NxAddInput) I/O source %d registered.\n", source);

    return(ios);

}


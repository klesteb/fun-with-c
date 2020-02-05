
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

#include "port_priv.h"

/*----------------------------------------------------------------------*/

int port_create(

#    if __STDC__
        const  char  *name,
        NxAppContext  context,
        ClientCreateFunc  clientCreateF,
        void  *clientParameter,
        ListeningPort  *port)
#    else
        name, context, clientCreateF, clientParameter, port)

        char  *name ;
        NxAppContext  context ;
        ClientCreateFunc  clientCreateF ;
        void  *clientParameter ;
        ListeningPort  *port ;
#    endif

{
/*
 * Function: port_create.c
 * Version : 1.0
 * Created : 01-Dec-1997
 * Author  : Alex Measday
 *
 * Description
 *
 *    Function port_create() creates a network listening port at which a
 *    server listens for connection requests from clients.  The port's
 *    listening socket is registered with the NIX Dispatcher; when a
 *    connection request is received, the callback function assigned to
 *    the socket answers the connection request.
 *
 *    Invocation:
 *
 *        status = port_create(name, context, clientCreateF, clientParameter,
 *                             &port);
 *
 *    where:
 *
 *        <name>              - I
 *            Is the port's server name.
 *
 *        <context>           - I
 *            Is the application context in which the network connection's
 *            listening socket will be registered with the NIX Dispatcher;
 *            NULL indicates the default context.
 *
 *        <clientCreateF>     - I
 *            Is a function that will be called by port_answer() when a
 *            connection request is accepted and a new data connection has
 *            been established.  The function should be declared as follows:
 *
 *            int  createClient(TcpEndpoint connection,
 *                              NxAppContext context,
 *                              void *parameter,
 *                              void **clientObject);
 *
 *            and is responsible for creating the new client object.  The
 *            client object and status returned by the function are ignored
 *            by port_answer().  Instead, the creation function is responsible
 *            for saving a reference to the client object somewhere and the
 *            function must arrange for the connection to be destroyed when
 *            it is no longer needed.
 *
 *        <clientParameter>   - I
 *            Is an arbitrary (VOID *) parameter passed to the client creation
 *            function.
 *
 *        <port>              - O
 *            Returns a handle for the listening port.  This handle is used
 *            in calls to the other PORT functions.
 *
 *        <status>            - O
 *            Returns the status of creating the listening port, zero if
 *            no errors occurred and ERRNO otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    /* Create and initialize a listening port structure.                */

    if ((*port = (ListeningPort)malloc(sizeof(_ListeningPort))) == NULL) {

        vperror("(port_create) Error allocating a port structure for %s.\nmalloc: ",
                 name);
        return(errno);

    }

    (*port)->name = strdup(name);
    if ((*port)->name == NULL) {

        vperror("(port_Create) Error duplicating server name for %s.\nstrdup: ",
                 name);
        return(errno);

    }

    (*port)->context = context;
    (*port)->sourceID = NULL;
    (*port)->createF = clientCreateF;
    (*port)->parameter = clientParameter;
    (*port)->endpoint = NULL;

    /* Create a listening socket on which connection requests for this  */
    /* server will be received.                                         */

    if (tcp_listen(name, 99, &(*port)->endpoint)) {

        vperror("(port_create) Error creating listening socket for %s.\ntcp_listen: ",
                 name);
        return(errno);

    }

    /* Register the listening socket with the NIX Dispatcher.  When a   */
    /* connection request is received, the "answer" callback is invoked */
    /* to accept the request.                                           */

    (*port)->sourceID =
        NxAddInput(context,
                   tcp_fd((*port)->endpoint),    /* File descriptor for listening socket. */
                   NxInputReadMask,        /* Monitor input. */
                   port_answer,            /* Routine to answer connection request. */
                   (void *) *port);        /* Arbitrary data. */

    if (port_util_debug)
        printf ("(port_create) Created %s listening port.\n", name);

    return(0);

}


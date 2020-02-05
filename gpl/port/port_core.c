
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

int port_util_debug = 0;         /* Global debug switch (1/0 = yes/no). */

/*----------------------------------------------------------------------*/

int port_answer(

#    if __STDC__
        NxAppContext  context,
        NxInputId  sourceID,
        int  source,
        void  *clientData)
#    else
        context, sourceID, source, clientData)

        NxAppContext  context ;
        NxInputId  sourceID ;
        int  source ;
        void  *clientData ;
#    endif

{
/*
 * Function: port_answer.c
 * Version : 1.0
 * Created : 01-Dec-1997
 * Author  : Alex Measday
 *
 * Description
 *
 *    Function port_answer() is the NIX callback assigned to a listening
 *    socket by port_create().  When a network connection request is
 *    received on the socket, the NIX Dispatcher automatically invokes
 *    this callback function to answer the connection request.  After
 *    accepting the request, port_answer() creates a client object for
 *    the new client.
 *
 *    Invocation:
 *
 *        status = port_answer(context, sourceID, source, clientData);
 *
 *    where:
 *
 *        <context>           - I
 *            Is the application context used when the network connection's
 *            listening socket was registered with the NIX Dispatcher.
 *
 *        <sourceID>          - I
 *            Is the identifier returned by NxAddInput() when the network
 *            connection's listening socket was registered with the NIX
 *            Dispatcher.
 *
 *        <source>            - I
 *            Is the socket on which the NIX Dispatcher is listening for
 *            connection requests.
 *
 *        <clientData>        - I
 *            Is the address of the listening port structure created for
 *            the network connection by port_create().
 *
 *        <status>            - O
 *            Returns the status of answering a connection request, zero if
 *            there were no errors and ERRNO otherwise.  The status value
 *            is ignored by the NIX Dispatcher.
 *
 * Modification History
 *
 * Variables Used
 */

    ListeningPort  port;
    TcpEndpoint  connection;
    void  *client;

/*
 * Main part of function.
 */

    port = (ListeningPort)clientData;

    /* Accept the connection request from a new client.                 */

    if (tcp_answer(port->endpoint, -1.0, &connection)) {

        vperror("(port_answer) Error answering connection request for %s.\ntcp_answer: ",
                port->name);
        return(errno);

    }

    if (port_util_debug)
        printf("(port_answer) Answered connection %s, socket %d.\n",
               tcp_name(connection), tcp_fd(connection));

    /* Create a client object for the new client.                       */

    if (port->createF == NULL) {

        tcp_destroy(connection);

    } else if (port->createF(connection, context, port->parameter, &client)) {

        vperror("(port_answer) Error creating client object for %s: ",
                tcp_name(connection));
        return(errno);

    }

    return(0);

}



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

#include "tcp_priv.h"

/*----------------------------------------------------------------------*/

int  tcp_answer (

#    if  __STDC__
        TcpEndpoint  listeningPoint,
        double  timeout,
        TcpEndpoint  *dataPoint)
#    else
        listeningPoint, timeout, dataPoint)

        TcpEndpoint  listeningPoint ;
        double  timeout ;
        TcpEndpoint  *dataPoint ;
#    endif

{
/*
 * Function: tcp_answer.c
 * Version : 1.0
 * Created : 18-Feb-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    The tcp_answer() function waits for and answers a client's request for a
 *    network connection.  When a request is accepted, the system automatically
 *    creates a new socket (the "data" socket) through which the server can talk
 *    to the client.
 *
 *    Invocation:
 *
 *        status = tcp_answer(listeningPoint, timeout, &dataPoint);
 *
 *    where
 *
 *        <listeningPoint>    - I
 *            Is the listening endpoint handle returned by tcp_listen().
 *
 *        <timeout>           - I
 *            Specifies the maximum amount of time (in seconds) that the
 *            caller wishes to wait for a connection to be established.
 *            A fractional time can be specified; e.g., 2.5 seconds.
 *            A negative timeout (e.g., -1.0) causes an infinite wait;
 *            a zero timeout (0.0) causes an immediate return if no
 *            connection request is pending.
 *
 *        <dataPoint>         - O
 *            Returns a handle for the data endpoint created by the acceptance
 *            of a connection request from another net location (i.e., a 
 *            client).
 *
 *        <status>            - O
 *            Returns the status of answering a network connection request:
 *            zero if there were no errors, EWOULDBLOCK if the timeout interval
 *            expired before a connection was established, and ERRNO otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

    char  *hostName, serverName[PATH_MAX+1];
    fd_set  readMask;
    unsigned int length;
    int numActive, optval;
    struct  sockaddr  clientAddress;
    struct  timeval  deltaTime, expirationTime;

/*
 * Main part of function.
 */

    /* Wait for the next connection request from a client. If a timeout */
    /* interval was specified, then compute the expiration time of the  */
    /* interval as the current time plus the interval.                  */

    if (timeout >= 0.0)
        expirationTime = tv_add(tv_tod(), tv_createf(timeout));

    /* Wait for a connection request from a client.                     */

    for (;;) {

        if (timeout >= 0.0)
            deltaTime = tv_subtract(expirationTime, tv_tod());

        FD_ZERO(&readMask);
        FD_SET(listeningPoint->fd, &readMask);

        numActive = select(listeningPoint->fd+1, &readMask, NULL, NULL,
                           (timeout < 0.0) ? NULL : &deltaTime);

        if (numActive >= 0)  break;
        if (errno == EINTR)  continue;    /* Retry on signal interrupt. */

        vperror("(tcp_answer) Error waiting for connection request on %s.\nselect: ",
                 listeningPoint->name);
        return(errno);

    }

    if (numActive == 0) {

        errno = EWOULDBLOCK;
        vperror("(tcp_answer) Timeout while waiting for connection request on %s.\n",
                 listeningPoint->name);
        return(errno);

    }

    /* Answer the connection request. Create an endpoint structure for  */
    /* the pending connection request.                                  */

    if ((*dataPoint = (TcpEndpoint)malloc(sizeof(_TcpEndpoint))) == NULL) {

        vperror("(tcp_answer) Error allocating endpoint structure for %s.\nmalloc: ",
                 listeningPoint->name);
        return(errno);

    }

    (*dataPoint)->name = NULL;
    (*dataPoint)->type = TcpDataPoint;
    (*dataPoint)->fd = -1;

    /* Accept the connection request.                                   */

    do {                             /* Retry interrupted system calls. */

        length = sizeof clientAddress;
        (*dataPoint)->fd = accept(listeningPoint->fd,
                                  &clientAddress, &length);

    } while (((*dataPoint)->fd < 0) && (errno == EINTR));

    if ((*dataPoint)->fd < 0) {

        if (errno != EWOULDBLOCK)
            vperror("(tcp_answer) Error accepting connection request on %s.\naccept: ",
                     listeningPoint->name);

        CLEAN_UP(*dataPoint);
        return(errno);

    }

    /* Configure the socket so that the operating system periodically   */
    /* verifies that the connection is still alive by "pinging" the     */
    /* client.                                                          */

    optval = 1;                     /* Enable keep-alive transmissions. */

    if (setsockopt((*dataPoint)->fd, SOL_SOCKET, SO_KEEPALIVE,
                    (char *) &optval, sizeof optval) == -1) {

        vperror("(tcp_answer) Error enabling keep-alive mode for %s's client connection.\nsetsocketopt: ",
                 listeningPoint->name);
        CLEAN_UP(*dataPoint);
        return(errno);

    }

    /* Construct the connection's name.                                 */

    hostName = (char *)skt_peer(listeningPoint->name, (*dataPoint)->fd);
    if (hostName == NULL)  hostName = "localhost";

    sprintf(serverName, "%d",
            skt_port(listeningPoint->name, (*dataPoint)->fd));

    (*dataPoint)->name = malloc(strlen (serverName) + 1 +
                                strlen (hostName) + 1) ;
    if ((*dataPoint)->name == NULL) {

        vperror("(tcp_answer) Error duplicating server name: %s#%s\nmalloc: ",
                 serverName, hostName);
        CLEAN_UP(*dataPoint);
        return(errno);

    }

    sprintf((*dataPoint)->name, "%s#%s", serverName, hostName);

    /* Return the data connection to the caller.                        */

    if (tcp_util_debug)  
        printf("(tcp_answer) Accepted connection %s, socket %d.\n",
               (*dataPoint)->name, (*dataPoint)->fd);

    return(0);

}


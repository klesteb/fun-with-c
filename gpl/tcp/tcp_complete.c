
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

int  tcp_complete (

#    if __STDC__
        TcpEndpoint  dataPoint,
        double  timeout,
        int  destroyOnError)
#    else
        dataPoint, timeout, destroyOnError)

        TcpEndpoint  dataPoint ;
        double  timeout ;
        int  destroyOnError ;
#    endif

{
/*
 * Function: tcp_complete.c
 * Version : 1.0
 * Created : 18-Feb-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function tcp_complete() waits for an asynchronous, network connection
 *    attempt to complete.  Invoking tcp_call() in no-wait mode initiates
 *    an attempt to connect to a network server.  At some later time, the
 *    application must call tcp_complete() to complete the connection attempt
 *    (if it is fated to complete).  See tcp_call() for an example of using
 *    tcp_complete().
 *
 *    The default behavior of tcp_complete() is to NOT destroy the endpoint
 *    created by tcp_call(), regardless of any errors.  Since the endpoint
 *    SHOULD be destroyed in the event of an error, the destroyOnError flag
 *    provides a short-hand means of waiting for a connection without
 *    explicitly destroying the endpoint if the connection attempt fails:
 *
 *        #include  "tcp_util.h"               -- TCP/IP networking utilities.
 *        TcpEndpoint  connection ;
 *        ...
 *        if (tcp_call("<server>[@<host>]", 1, &connection) ||
 *            tcp_complete(connection, 30.0, 1)) {
 *            ... timeout or error establishing connection;
 *            connection is destroyed ...
 *        } else {
 *            ... connection is established ...
 *        }
 *
 *    Some applications, however, may not wish to destroy the connection right
 *    away.  For example, the following code fragment periodically displays an
 *    in-progress message until a connection is established or refused:
 *
 *        if (tcp_call("<server>[@<host>]", 1, &connection)) {
 *            ... error initiating connection attempt ...
 *        } else {
 *            for (;;) {
 *                printf("Waiting for %s ...\n", tcp_name(connection));
 *                if (!tcp_complete(connection, 5.0, 0)) {
 *                    ... connection is established ...
 *                    break;
 *                } else if (errno != EWOULDBLOCK) {
 *                    ... connection attempt failed ...
 *                    tcp_destroy(connection);
 *                    connection = NULL;
 *                    break;
 *                }
 *            }
 *        }
 *
 *    Invocation:
 *
 *        status = tcp_complete(dataPoint, timeout, destroyOnError);
 *
 *    where
 *
 *        <dataPoint>         - I
 *            is the endpoint handle returned by tcpCall().
 *
 *        <timeout>           - I
 *            specifies the maximum amount of time (in seconds) that the
 *            caller wishes to wait for the call to complete.  A fractional
 *            time can be specified; e.g., 2.5 seconds.  A negative timeout
 *            (e.g., -1.0) causes an infinite wait; a zero timeout (0.0)
 *            causes an immediate return if the connection is not yet
 *            established.
 *
 *        <destroyOnError>    - I
 *            specifies if tcp_complete() should destroy the endpoint in the
 *            event of an error.  If this argument is true (a non-zero value),
 *            tcp_complete() calls tcp_destroy() to destroy the endpoint.  If
 *            this argument is false (zero), the calling routine itself is
 *            responsible for destroying the endpoint.
 *
 *        <status>            - O
 *            returns the status of completing the network connection:
 *            zero if there were no errors, EWOULDBLOCK if the timeout
 *            interval expired before the connection was established,
 *            and ERRNO otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

    fd_set  writeMask ;
    unsigned int length;
    int numActive, optval ;
    struct  timeval  deltaTime, expirationTime ;

/*
 * Main part of function.
 */

    /* If a timeout interval was specified, then compute the expiration */
    /* time of the interval as the current time plus the interval.      */

    if (timeout >= 0.0)
        expirationTime = tv_add(tv_tod(), tv_createf(timeout));

    /* Wait for the call to complete, which is indicated by the         */
    /* connection being writeable.                                      */

    for (;;) {

        if (timeout >= 0.0)  deltaTime = tv_subtract(expirationTime, tv_tod());
        FD_ZERO(&writeMask);  
        FD_SET(dataPoint->fd, &writeMask);

        numActive = select(dataPoint->fd+1, NULL, &writeMask, NULL,
                           (timeout < 0.0) ? NULL : &deltaTime);

        if (numActive > 0)  break;
        if (numActive == 0)  errno = EWOULDBLOCK;
        if (errno == EINTR)  continue;    /* Retry on signal interrupt. */

        vperror("(tcp_complete) Error waiting to connect to %s.\nselect: ",
                dataPoint->name);
        if (destroyOnError) CLEAN_UP(dataPoint);
        return(errno);

    }

    /* Check the connection's error status.                             */

    length = sizeof optval;
    if (getsockopt(dataPoint->fd, SOL_SOCKET, SO_ERROR,
                    (char *) &optval, &length) == -1) {

        vperror("(tcp_complete) Error checking error status of connection to %s.\ngetsockopt: ",
                dataPoint->name);
        if (destroyOnError) CLEAN_UP(dataPoint);
        return(errno);

    }

    if (optval) {

        errno = optval;
        vperror("(tcp_complete) Error connecting to %s.\nconnect: ",
                 dataPoint->name);
        if (destroyOnError)  CLEAN_UP(dataPoint);
        return(errno);

    }

    /* The connection has been established.  Configure the socket so    */
    /* that the operating system periodically verifies that the         */
    /* connection is still alive by "pinging" the server.               */

    optval = 1;                     /* Enable keep-alive transmissions. */

    if (setsockopt(dataPoint->fd, SOL_SOCKET, SO_KEEPALIVE,
                   (char *) &optval, sizeof optval) == -1) {

        vperror("(tcp_complete) Error enabling keep-alive mode for connection to %s.\nsetsockopt: ",
                 dataPoint->name);
        if (destroyOnError)  CLEAN_UP(dataPoint);
        return(errno);

    }

    /* Reconfigure the socket for blocking I/O.                         */

    optval = 0;                      /* A value of 0 sets blocking I/O. */

    if (ioctl(dataPoint->fd, FIONBIO, &optval) == -1) {

        vperror("(tcp_complete) Error reconfiguring %s's socket for blocking I/O.\nioctl: ",
                dataPoint->name);
        if (destroyOnError)  CLEAN_UP(dataPoint);
        return(errno);

    }

    if (tcp_util_debug)  
        printf("(tcp_complete) Connected to %s, port %d, socket %d.\n",
               dataPoint->name,
               skt_port(dataPoint->name, dataPoint->fd),
               dataPoint->fd);

    return(0);

}


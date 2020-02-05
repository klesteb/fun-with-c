
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

int  tcp_call (

#    if __STDC__
        const  char  *serverName,
        int lPort,
        int  noWait,
        TcpEndpoint  *dataPoint)
#    else
        serverName, noWait, dataPoint)

        char  *serverName ;
        int lPort;
        int  noWait ;
        TcpEndpoint  *dataPoint ;
#    endif

{
/*
 * Function: tcp_call.c
 * Version : 1.0
 * Created : 18-Feb-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function tcp_call() is used by a client task to "call" a server task
 *    and request a network connection to the server.  If its no-wait
 *    argument is false (zero), tcp_call() waits until the connection is
 *    established (or refused) before returning to the invoking function:
 *
 *        #include  "tcp_util.h"                -- TCP/IP networking utilities.
 *        TcpEndpoint  connection ;
 *        ...
 *        if (tcp_call ("<server>[@<host>]", 0, 0, &connection)) {
 *            ... error establishing connection ...
 *        } else {
 *            ... connection is established ...
 *        }
 *
 *    If the no-wait argument is true (non-zero), tcp_call() initiates the
 *    connection attempt and immediately returns; the application must then
 *    invoke tcp_complete() to complete the connection attempt:
 *
 *        if (tcp_call("<server>[@<host>]", 0, 1, &connection)) {
 *            ... error initiating connection attempt ...
 *        } else {
 *            ... do something else ...
 *            if (tcp_complete(connection, -1.0, 1)) {
 *                ... error establishing connection ...
 *            } else {
 *                ... connection is established ...
 *            }
 *        }
 *
 *    No-wait mode is useful in applications which monitor multiple I/O 
 *    sources using select(2).  After tcp_call() returns, a to-be-established 
 *    connection's file descriptor can be retrieved with tcp_fd() and placed 
 *    in select(2)'s write mask.  When select(2) detects that the connection 
 *    is writeable, the application can call tcp_complete() to complete the 
 *    connection.
 *
 *    IMPLEMENTATION NOTE:  Connecting to a server is usually accomplished
 *    by creating an unbound socket and calling connect(), a system call, to
 *    establish the connection.  In order to implement the timeout capability,
 *    the socket is configured for non-blocking I/O before connect() is called.
 *    If the connection cannot be established right away, connect() returns
 *    immediately and select() is called to wait for the timeout interval to
 *    expire or for the socket to become writeable (indicating the connection
 *    is complete).  Once the connection is established, the socket is
 *    reconfigured for blocking I/O.
 *
 *    The basic idea for implementing a timeout capability was outlined in
 *    a posting from W. Richard Stevens (*the* network guru) I retrieved
 *    from one of the WWW news archives.  VxWorks has a non-portable
 *    connectWithTimeout() function which, to save a few "#ifdef"s, I don't
 *    use.
 *
 *    Invocation:
 *
 *        status = tcp_call(serverName, lPort, noWait, &dataPoint);
 *
 *    where
 *
 *        <serverName>        - I
 *            Is the server's name: "<server>[@<host>]".  The server can be
 *            specified as a name or as a port number.  The host, if given,
 *            can be specified as a name or as a dotted Internet address.
 *
 *        <noWait>            - I
 *            Specifies if tcp_call() should wait for a connection to be
 *            established.  If noWait is false (zero), tcp_call() waits
 *            until a connection is established or refused before returning
 *            control to the caller.  If noWait is true (a non-zero value),
 *            tcp_call() initiates a connection attempt and returns to the
 *            caller immediately; the caller is responsible for eventually
 *            calling tcp_complete() to complete the connection.
 *
 *        <lPort>             - I
 *            Specifies the local IP port to use when making the connection.
 *            A "0" says use whatever is available.
 *
 *        <dataPoint>         - O
 *            Returns a handle for the endpoint.
 *
 *        <status>            - O
 *            Returns the status of establishing the network connection:
 *            zero if there were no errors and ERRNO otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

    char  *s, hostName[MAXHOSTNAMELEN+1], server_name[MAXHOSTNAMELEN+1] ;
    int length;
    int optval, portNumber ;
    struct  sockaddr_in  socketName ;
    struct  sockaddr_in  my_addr ;

/*
 * Main part of function.
 */

    /* Determine the host and server information needed to make the     */
    /* connection. Parse the host and server names.  If the host name   */
    /* is not defined explicitly, it defaults to the local host.        */

    if ((s = net_hostof(net_addrof(NULL))) == NULL) {

        vperror("(tcp_call) Error getting local host name.\nnet_hostof: ");
        return(errno);

    }

    strcpy(hostName, s);

    s = strchr(serverName, '@');
    if (s == NULL) {                            /* "<server>"           */

        strcpy(server_name, serverName);

    } else {                                    /* "<server>@<host>"    */

        length = s - serverName;
        strncpy(server_name, serverName, length);
        server_name[length] = '\0';
        strcpy(hostName, ++s);

    }

    /* Create an endpoint structure.                                    */

    if ((*dataPoint = (TcpEndpoint)malloc(sizeof(_TcpEndpoint))) == NULL) {

        vperror("(tcp_call) Error allocating connection structure for %s.\nmalloc: ",
                 serverName);
        return(errno);

    }

    (*dataPoint)->type = TcpDataPoint;
    (*dataPoint)->fd = -1;

    (*dataPoint)->name = malloc(strlen(server_name) + 1 +
                                strlen(hostName) + 1);
    if ((*dataPoint)->name == NULL) {

        vperror("(tcp_call) Error duplicating server name: %s@%s\nmalloc: ",
                 server_name, hostName);
        CLEAN_UP(*dataPoint);
        return(errno);

    }

    sprintf((*dataPoint)->name, "%s@%s", server_name, hostName);

    /* Lookup the port number bound to the server name.                 */

    portNumber = net_portof(server_name, "tcp");
    if (portNumber == -1) {

        vperror("(tcp_call) Error getting server entry for %s.\nnetPortOf: ",
                 serverName);
        CLEAN_UP(*dataPoint);
        return(errno);

    }

    /* Set up the network address for the connection.                   */

    memset(&socketName, '\0', sizeof socketName);
    socketName.sin_family = AF_INET;
    socketName.sin_port = htons(portNumber);
    socketName.sin_addr.s_addr = net_addrof(hostName);

    if ((long) socketName.sin_addr.s_addr == -1) {

        vperror("(tcp_call) Error getting host entry for %s.\nnetAddrOf: ",
                 hostName);
        CLEAN_UP(*dataPoint);
        return(errno);

    }

    /* Establish a connection with the server. Create a socket for the  */
    /* connection.                                                      */

    (*dataPoint)->fd = socket(AF_INET, SOCK_STREAM, 0);
    if ((*dataPoint)->fd < 0) {

        vperror("(tcp_call) Error creating socket for %s.\nsocket: ",
                 (*dataPoint)->name);
        CLEAN_UP(*dataPoint);
        return(errno);

    }

    /* Set up the desired local port for the connection.                */

    if (lPort != 0) {

        memset(&my_addr, '\0', sizeof(my_addr));
        my_addr.sin_family      = AF_INET;
        my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        my_addr.sin_port        = htons((u_short) lPort);

        if (bind((*dataPoint)->fd, (struct sockaddr *) &my_addr, sizeof(my_addr)) < 0) {

            vperror("(tcp_call) Error binding socket with a local port %d %s.\nsocket: ",
                 lPort, (*dataPoint)->name);
            CLEAN_UP(*dataPoint);
            return(errno);

        }

    }    

    /* Configure the socket so it can be discarded quickly when no      */
    /* longer needed. (See the description of the SO_REUSEADDR option   */
    /* under NET_ANSWER, above.)                                        */

    optval = 1 ;                        /* Enable address reuse. */

    if (setsockopt((*dataPoint)->fd, SOL_SOCKET, SO_REUSEADDR,
                   (char *) &optval, sizeof optval) == -1) {

        vperror("(tcp_call) Error setting %s's socket for re-use.\nsetsocketopt: ",
                 (*dataPoint)->name);
        CLEAN_UP(*dataPoint);
        return(errno);

    }

    /* If caller does not wish to wait for the connection to be         */
    /* established, then configure the socket for non-blocking I/O.     */
    /* This causes the connect(2) call to only initiate the attempt to  */
    /* connect; tcp_complete() must be called to complete the           */
    /* connection.                                                      */

    optval = 1 ;                /* A value of 1 enables non-blocking I/O. */

    if (noWait && (ioctl((*dataPoint)->fd, FIONBIO, &optval) == -1)) {

        vperror("(tcp_call) Error configuring %s's socket for non-blocking I/O.\nioctl: ",
                 (*dataPoint)->name);
        CLEAN_UP(*dataPoint);
        return(errno);

    }

    /* Attempt to establish the connection.                             */

    if (connect((*dataPoint)->fd,
                (struct sockaddr *) &socketName, sizeof socketName) &&
        (!noWait || (errno != EINPROGRESS))) {

        vperror("(tcp_call) Error attempting to connect to %s.\nconnect: ",
                 (*dataPoint)->name);
        CLEAN_UP(*dataPoint);
        return(errno);

    }

    /* If caller does not wish to wait for the connection to be         */
    /* established, then return immediately; the caller is responsible  */
    /* for subsequently calling tcp_complete().                         */

    if (noWait) return(0);

    /* The connection has been established.  Configure the socket so    */
    /* that the operating system periodically verifies that the         */
    /* connection is still alive by "pinging" the server.               */

    optval = 1;                     /* Enable keep-alive transmissions. */

    if (setsockopt((*dataPoint)->fd, SOL_SOCKET, SO_KEEPALIVE,
                   (char *) &optval, sizeof optval) == -1) {

        vperror("(tcp_call) Error enabling keep-alive mode for connection to %s.\nsetsockopt: ",
                (*dataPoint)->name);
        CLEAN_UP(*dataPoint);
        return(errno);

    }

    if (tcp_util_debug)  
        printf("(tcp_call) Connected to %s, port %d, socket %d.\n",
               (*dataPoint)->name,
               skt_port((*dataPoint)->name, (*dataPoint)->fd),
               (*dataPoint)->fd);


    return(0);        /* Successful completion. */

}


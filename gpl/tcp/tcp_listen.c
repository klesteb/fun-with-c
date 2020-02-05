
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

int  tcp_listen (

#    if __STDC__
        const  char  *serverName,
        int  backlog,
        TcpEndpoint  *listeningPoint)
#    else
        serverName, backlog, listeningPoint)

        char  *serverName ;
        int  backlog ;
        TcpEndpoint  *listeningPoint ;
#    endif

{
/*
 * Function: tcp_listen.c
 * Version : 1.0
 * Created : 18-Feb-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function tcp_listen() creates a "listening" endpoint on which a network
 *    server can listen for connection requests from clients.  The server
 *    then calls tcp_answer() to "answer" incoming requests.
 *
 *    Invocation:
 *
 *        status = tcp_listen(serverName, backlog, &listeningPoint);
 *
 *    where
 *
 *        <serverName>        - I
 *            Is the server's name.  This is used for determining the port
 *            associated with the server (via the system's name/port mappings).
 *            You can side-step the system maps and explicitly specify a
 *            particular port by passing in a decimal number encoded in ASCII
 *            (e.g., "1234" for port 1234).
 *
 *        <backlog>           - I
 *            Is the number of connection requests that can be outstanding
 *            for the server.  UNIX systems typically allow a maximum of 5.
 *
 *        <listeningPoint>    - O
 *            Returns a handle for the new listening endpoint.
 *
 *        <status>            - O
 *            Returns the status of creating the endpoint: zero if there
 *            were no errors and ERRNO otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

    int  optval, portNumber;
    struct  sockaddr_in  socketName;

/*
 * Main part of function.
 */

    if (serverName == NULL)  serverName = "0";
    if (backlog < 0)  backlog = 99;

    /* Create an endpoint structure.                                    */

    if ((*listeningPoint = (TcpEndpoint)malloc(sizeof(_TcpEndpoint))) == NULL) {

        vperror("(tcp_listen) Error allocating endpoint structure for %s.\nmalloc: ",
                 serverName);
        return(errno);

    }

    (*listeningPoint)->type = TcpListeningPoint;
    (*listeningPoint)->fd = -1;

    (*listeningPoint)->name = strdup(serverName);
    if ((*listeningPoint)->name == NULL) {

        vperror("(tcp_listen) Error duplicating server name: %s\nstrdup: ",
                 serverName);
        CLEAN_UP(*listeningPoint);
        return(errno);

    }

    /* Lookup the port number bound to the server name.                 */

    portNumber = net_portof(serverName, "tcp");
    if (portNumber == -1) {

        vperror("(tcp_listen) Error getting server entry for %s.\nnet_portof: ",
                 serverName);
        CLEAN_UP(*listeningPoint);
        return(errno);

    }

    /* Set up the network address for the connection.                   */

    memset(&socketName, '\0', sizeof socketName);
    socketName.sin_family = AF_INET;
    socketName.sin_port = htons(portNumber);
    socketName.sin_addr.s_addr = INADDR_ANY;

    /* Create a socket for the connection.                              */

    (*listeningPoint)->fd = socket(AF_INET, SOCK_STREAM, 0);
    if ((*listeningPoint)->fd < 0) {

        vperror("(tcp_listen) Error creating listening socket for endpoint %s.\nsocket: ",
                 serverName);
        CLEAN_UP(*listeningPoint);
        return(errno);

    }

    /* Configure the socket so it can be discarded quickly when no      */
    /* longer needed. If the SO_REUSEADDR option is not enabled and     */
    /* server A goes down without client B closing its half of the      */
    /* broken connection, server A can't come back up again.  Server A  */
    /* keeps getting an "address in use" error, even though A is the    */
    /* "owner" of the port and B can't really do anything with its      */
    /* broken connection!  The reuse-address option allows A to come    */
    /* right back up again and create a new listening socket.           */

    optval = 1;                                /* Enable address reuse. */

    if (setsockopt((*listeningPoint)->fd, SOL_SOCKET, SO_REUSEADDR,
                   (char *) &optval, sizeof optval) == -1) {

        vperror("(tcp_listen) Error setting %s endpoint's listening socket for re-use.\nsetsocketopt: ",
                 serverName);
        CLEAN_UP(*listeningPoint);
        return(errno);

    }

    /* Bind the network address to the socket and enable it to listen   */
    /* for connection requests.                                         */

    if (bind((*listeningPoint)->fd,
             (struct sockaddr *) &socketName, sizeof socketName)) {

        vperror("(tcp_listen) Error binding %s endpoint's socket name.\nbind: ",
                serverName);
        CLEAN_UP(*listeningPoint);
        return(errno);

    }

    if (portNumber == 0) {

        portNumber = skt_port(serverName, (*listeningPoint)->fd);
        free((*listeningPoint)->name);

        (*listeningPoint)->name = strndup(NULL, 16);
        if ((*listeningPoint)->name == NULL) {

            vperror("(tcp_listen) Error duplicating port name: %d\nstrndup: ",
                    portNumber);
            CLEAN_UP(*listeningPoint);

        }

        sprintf((*listeningPoint)->name, "%d", portNumber);

    }

    if (listen((*listeningPoint)->fd, backlog)) {

        vperror("(tcp_listen) Error enabling acceptance of connection requests on %s endpoint.\nlisten: ",
                (*listeningPoint)->name);
        CLEAN_UP(*listeningPoint);
        return(errno);

    }

    if (tcp_util_debug)
        printf("(tcp_listen) Listening on %s, port %d, socket %d.\n",
               (*listeningPoint)->name, portNumber, (*listeningPoint)->fd);

    return(0);

}


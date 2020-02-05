
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

#include "skt_priv.h"

/*----------------------------------------------------------------------*/

/**/

char  *skt_peer (

#    if __STDC__
        const  char  *name,
        int  fd)
#    else
        name, fd)

        char  *name ;
        int  fd ;
#    endif

{
/*
 * Function: skt_peer.c
 * Version : 1.0
 * Created : 18-Feb-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function skt_peer() returns the name (i.e., the Internet address) of
 *    the host at the other end of a network socket connection.
 *
 *    Invocation:
 *
 *        host = skt_peer(name, fd);
 *
 *    where
 *
 *        <name>              - I
 *            Is the name of the socket for debugging purposes; this argument
 *            can be NULL.
 *
 *        <fd>                - I
 *            Is the UNIX file descriptor for the socket.
 *
 *        <host>              - O
 *            Returns the Internet address of the connected host. The address
 *            is formatted in ASCII using the standard Internet dot notation:
 *            "a.b.c.d". NULL is returned in the event of an error. The
 *            ASCII host string is stored local to skt_peer() and it should
 *            be used or duplicated before calling skt_peer() again.
 *
 * Modification History
 *
 * Variables Used
 */

    struct  sockaddr  peerAddress;
    struct  sockaddr_in  *ipAddress;
    unsigned int length;

/*
 * Main part of function.
 */

    if (name == NULL)  name = "?";

    /* Get the IP address of the host on the other end of the network   */
    /* connection.                                                      */

    length = sizeof(peerAddress);

    if (getpeername(fd, &peerAddress, &length)) {

        vperror("(net_peer) Error getting peer's host for %s, socket %d.\ngetpeername: ",
                 name, fd);
        return(NULL);

    }

    ipAddress = (struct sockaddr_in *)&peerAddress;

    /* Convert the peer's IP address to a host name.                    */

    return(net_hostof(ipAddress->sin_addr.s_addr));

}


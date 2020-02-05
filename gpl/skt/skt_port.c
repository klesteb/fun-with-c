
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

int  skt_port (

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
 * Function: skt_port.c
 * Version : 1.0
 * Created : 18-Feb-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function skt_port() returns the number of the port to which a socket
 *    (either listening or data) is bound.
 *
 *    Invocation:
 *
 *        number = skt_port(name, fd);
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
 *        <number>            - O
 *            Returns the socket's port number; -1 is returned in the
 *            event of an error.
 *
 * Modification History
 *
 * Variables Used
 */

    unsigned int  length;
    struct  sockaddr_in  socketName;

/*
 * Main part of function.
 */

    /* Get the socket's port number and convert it to host byte         */
    /* ordering.                                                        */

    length = sizeof socketName;

    if (getsockname(fd, (struct sockaddr *)&socketName, &length)) {

        vperror("(skt_port) Error getting port number for %s, socket %d.\ngetsockname: ",
                 name, fd);
        return(-1);

    }

    return(ntohs(socketName.sin_port));

}



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

int  skt_setbuf (

#    if __STDC__
        const  char  *name,
        int  fd,
        int  receiveSize,
        int  sendSize)
#    else
        name, fd, receiveSize, sendSize)

        char  *name ;
        int  fd ;
        int  receiveSize ;
        int  sendSize ;
#    endif

{
/*
 * Function: skt_setbuf.c
 * Version : 1.0
 * Created : 18-Feb-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function skt_setbuf() changes the sizes of a socket's receive and/or 
 *    send buffers.
 *
 *    Invocation:
 *
 *        status = skt_setbuf(name, fd, receiveSize, sendSize);
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
 *        <receiveSize>       - I
 *            Specifies the new size of the socket's receive system buffer.
 *            If this argument is less than zero, the receive buffer retains
 *            its current size.
 *
 *        <sendSize>          - I
 *            Specifies the new size of the socket's send system buffer.  If
 *            this argument is less than zero, the send buffer retains its
 *            current size.
 *
 *        <status>            - O
 *            Returns the status of changing the buffers' sizes, zero if no
 *            error occurred and ERRNO otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    /* Change the size of the socket's receive system buffer.           */

    if ((receiveSize >= 0) &&
        setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &receiveSize, sizeof (int))) {

        vperror("(skt_setbuf) Error setting receive buffer size (%d) for %s, socket %d.\nsetsockopt: ",
                 receiveSize, name, fd);
        return(errno);

    }

    /* Change the size of the socket's send system buffer.              */

    if ((sendSize >= 0) &&
        setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &sendSize, sizeof (int))) {

        vperror("(skt_setbuf) Error setting send buffer size (%d) for %s, socket %d.\nsetsockopt: ",
                 sendSize, name, fd);
        return(errno);

    }

    return(0);

}


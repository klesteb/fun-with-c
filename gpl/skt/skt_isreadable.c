
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

int  skt_isreadable (

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
 * Function: skt_isreadable.c
 * Version : 1.0
 * Created : 18-Feb-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function skt_isreadable() checks to see if data is waiting to
 *    be read from a socket.
 *
 *    Invocation:
 *
 *        isReadable = skt_isreadable(name, fd);
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
 *        <isReadable>        - O
 *            Returns true (a non-zero value) if data is available for
 *            reading and false (zero) otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

    fd_set  readMask;
    long  length;
    struct  timeval  timeout;

/*
 * Main part of function.
 */

    if (name == NULL)  name = "?";

    /* Poll the data socket for input.                                  */

    for (;;) {

        FD_ZERO(&readMask);  
        FD_SET(fd, &readMask);
        timeout.tv_sec = timeout.tv_usec = 0;   /* No wait.             */

        if (select(fd+1, &readMask, NULL, NULL, &timeout) >= 0)  break;
        if (errno == EINTR)  continue;          /* Retry on signal interrupt. */

        vperror("(skt_isreadable) Error polling %s, socket %d.\nselect: ",
                 name, fd);
        return(0);

    }

    /* No input pending?                                                */

    if (!FD_ISSET(fd, &readMask)) return (0);

    /* Input is pending. Find out how many bytes of data are actually   */
    /* available for input. If SELECT(2) indicates pending input, but   */
    /* IOCTL(2) indicates zero bytes of pending input, the connection   */
    /* is broken.                                                       */

    while (ioctl(fd, FIONREAD, &length) == -1) {

        if (errno == EINTR)  continue;         /* Retry on signal interrupt. */

        vperror("(skt_isreadable) Error polling %s, socket %d.\nioctl: ",
                 name, fd);
        return(0);

    }

    if (length > 0) {

        return(length);                        /* Pending input.        */

    } else {

        errno = EPIPE;
        vperror("(skt_isreadable) Broken connection to %s, socket %d.\n",
                 name, fd);
        return(0);                             /* EOF.                  */

    }

}


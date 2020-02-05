
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

int  skt_isup (

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
 * Function: skt_isup.c
 * Version : 1.0
 * Created : 18-Feb-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function skt_isup() checks to see if a network connection is still up.
 *
 *    Invocation:
 *
 *        isUp = skt_isup(name, fd);
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
 *        <isUp>              - O
 *            Returns true (a non-zero value) if the network connection is
 *            up and false (zero) otherwise.
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
        timeout.tv_sec = timeout.tv_usec = 0;  /* No wait. */

        if (select(fd+1, &readMask, NULL, NULL, &timeout) >= 0)  break;
        if (errno == EINTR)  continue;         /* Retry on signal interrupt. */

        vperror("(skt_isup) Error polling %s, socket %d.\nselect: ",
                 name, fd);
        return(0) ;                            /* Connection is down.   */

    }

    /* No input pending?  Connection is up.                             */

    if (!FD_ISSET(fd, &readMask)) return(1);                            

    /* Input is pending. Find out how many bytes of data are actually   */
    /* available for input. If SELECT(2) indicates pending input, but   */
    /* IOCTL(2) indicates zero bytes of pending input, the connection   */
    /* is broken.                                                       */

    while (ioctl(fd, FIONREAD, &length) == -1) {

        if (errno == EINTR)  continue;    /* Retry on signal interrupt. */

        vperror("(skt_isup) Error polling %s, socket %d.\nioctl: ",
                 name, fd);
        return(0);

    }

    if (length > 0) {                         /* Pending input?         */

        return(1);                            /* Connection is up.      */

    } else {

        errno = EPIPE;
        vperror("(skt_isup) Broken connection to %s, socket %d.\n",
                 name, fd);
        return(0);                            /* Connection is down.    */

    }

}



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

int  tcp_request_pending (

#    if __STDC__
        TcpEndpoint  listeningPoint)
#    else
        listeningPoint)

        TcpEndpoint  listeningPoint ;
#    endif

{
/*
 * Function: tcp_request_pending.c
 * Version : 1.0
 * Created : 18-Feb-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    The tcp_request_pending() function checks to see if any connection 
 *    requests from potential clients are waiting to be answered.
 *
 *    Invocation:
 *
 *        isPending = tcp_request_pending(listeningPoint);
 *
 *    where
 *
 *        <listeningPoint>    - I
 *            Is the endpoint handle returned by tcpListen().
 *
 *        <isPending>         - O
 *            Returns true (a non-zero value) if connection requests are
 *            pending and false (zero) otherwise.
  *
 * Modification History
 *
 * Variables Used
 */

   fd_set  readMask;
   struct  timeval  timeout;

/*
 * Main part of function.
 */

    if (listeningPoint == NULL) return(0);

    /* Poll the listening socket for input.                             */

    for (;;) {

        FD_ZERO(&readMask);  
        FD_SET(listeningPoint->fd, &readMask);

        timeout.tv_sec = timeout.tv_usec = 0;  /* No wait.              */

        if (select(listeningPoint->fd+1, &readMask, NULL, NULL, &timeout) >= 0)
            break;

        if (errno == EINTR) continue ;         /* Retry on signal interrupt. */

        vperror("(tcp_request_pending) Error polling endpoint %s, socket %d.\nselect: ",
                 listeningPoint->name, listeningPoint->fd);
        return(0);

    }

    return(FD_ISSET(listeningPoint->fd, &readMask)); /* Request pending? */

}


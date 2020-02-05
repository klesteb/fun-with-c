
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

int  tcp_fd (

#    if __STDC__
        TcpEndpoint  endpoint)
#    else
        endpoint)

        TcpEndpoint  endpoint ;
#    endif

{
/*
 * Function: tcp_fd.c
 * Version : 1.0
 * Created : 18-Feb-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function tcp_fd() returns a listening or data endpoint's socket.
 *
 *    Invocation:
 *
 *        fd = tcp_fd(endpoint);
 *
 *    where
 *
 *        <endpoint>          - I
 *            Is the endpoint handle returned by tcp_listen(), tcp_answer(),
 *            or tcp_call().
 *
 *        <fd>                - O
 *            Returns the UNIX file descriptor for the endpoint's socket.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    return((endpoint == NULL) ? -1 : endpoint->fd);

}


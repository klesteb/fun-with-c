
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

int tcp_util_debug = 0;          /* Global debug switch (1/0 = yes/no). */

/*----------------------------------------------------------------------*/

int  tcp_destroy (

#    if __STDC__
        TcpEndpoint  endpoint)
#    else
        endpoint)

        TcpEndpoint  endpoint ;
#    endif

{
/*
 * Function: tcp_destroy.c
 * Version : 1.0
 * Created : 18-Feb-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function tcp_destroy() deletes listening endpoints created by 
 *    tcp_listen() and data endpoints created by tcp_answer() or tcp_call().
 *
 *    Invocation:
 *
 *        status = tcp_destroy(endpoint);
 *
 *    where
 *
 *        <endpoint>          - I
 *            is the endpoint handle returned by tcp_listen(), tcp_answer(),
 *            or tcp_call().
 *
 *        <status>            - O
 *            returns the status of deleting the endpoint, zero if there were
 *            no errors and ERRNO otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    if (endpoint == NULL) return(0);

    if (tcp_util_debug)
        printf("(tcp_destroy) Closing %s, socket %d.\n",
               endpoint->name, endpoint->fd);

    /* Close the endpoint's socket.                                     */

    if (endpoint->fd >= 0)  close(endpoint->fd);

    /* Deallocate the endpoint structure.                               */

    if (endpoint->name != NULL)  free(endpoint->name);
    free(endpoint);

    return(0);

}


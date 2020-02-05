
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

char  *tcp_name (

#    if __STDC__
        TcpEndpoint  endpoint)
#    else
        endpoint)

        TcpEndpoint  endpoint ;
#    endif

{
/*
 * Function: tcp_name.c
 * Version : 1.0
 * Created : 18-Feb-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function tcp_name() returns a listening or data endpoint's name.
 *
 *    Invocation:
 *
 *        name = tcp_name(endpoint);
 *
 *    where
 *
 *        <endpoint>          - I
 *            Is the endpoint handle returned by tcp_listen(), tcp_answer(),
 *            or tcp_call().
 *
 *        <name>              - O
 *            Returns the endpoint's name.  The name is stored in memory local
 *            to the TCP utilities and it should not be modified or freed
 *            by the caller.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    if (endpoint == NULL)  return("");
    if (endpoint->name == NULL)  return("");

    return(endpoint->name);

}


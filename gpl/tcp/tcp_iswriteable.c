
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

int  tcp_iswriteable (

#    if __STDC__
        TcpEndpoint  dataPoint)
#    else
        dataPoint)

        TcpEndpoint  dataPoint ;
#    endif

{
/*
 * Function: tcp_iswriteable.c
 * Version : 1.0
 * Created : 18-Feb-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    The tcp_iswriteable() function checks to see if data can be written
 *    to a connection.
 *
 *    Invocation:
 *
 *        isWriteable = tcp_iswriteable(dataPoint);
 *
 *    where
 *
 *        <dataPoint>         - I
 *            Is the endpoint handle returned by tcp_answer() or tcp_call().
 *
 *        <isWriteable>       - O
 *            Returns true (a non-zero value) if data connection is ready
 *            for writing and false (zero) otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    if (dataPoint == NULL) {

        return(0);

    } else return(skt_iswriteable(dataPoint->name, dataPoint->fd));

}


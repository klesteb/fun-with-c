
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

int  tcp_isreadable (

#    if __STDC__
        TcpEndpoint  dataPoint)
#    else
        dataPoint)

        TcpEndpoint  dataPoint ;
#    endif

{
/*
 * Function: tcp_isreadable.c
 * Version : 1.0
 * Created : 18-Feb-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    The tcp_isreadable() function checks to see if data is waiting to
 *    be read from a connection.
 *
 *    Invocation:
 *
 *        isReadable = tcp_isreadable(dataPoint);
 *
 *    where
 *
 *        <dataPoint>     - I
 *            is the endpoint handle returned by tcp_answer() or tcp_call().
 *
 *        <isReadable>    - O
 *            returns true (a non-zero value) if data is available for
 *            reading and false (zero) otherwise.
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

    } else return(skt_isreadable(dataPoint->name, dataPoint->fd));

}


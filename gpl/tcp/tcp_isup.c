
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

int  tcp_isup (

#    if __STDC__
        TcpEndpoint  dataPoint)
#    else
        dataPoint)

        TcpEndpoint  dataPoint ;
#    endif

{
/*
 * Function: tcp_isup.c
 * Version : 1.0
 * Created : 18-Feb-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    The tcp_isup() function checks to see if a network connection is 
 *    still up.
 *
 *    Invocation:
 *
 *        isUp = tcp_isup(dataPoint);
 *
 *    where
 *
 *        <dataPoint>         - I
 *            Is the endpoint handle returned by tcp_answer() or tcp_call().
 *
 *        <isUp>              - O
 *            Returns true (a non-zero value) if the network connection is
 *            up and false (zero) otherwise.
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

    } else return(skt_isup(dataPoint->name, dataPoint->fd));

}


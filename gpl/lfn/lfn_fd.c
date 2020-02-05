
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

#include "lfn_priv.h"

/*----------------------------------------------------------------------*/

int  lfn_fd (

#    if __STDC__
    LfnStream  stream)
#    else
    stream)

    LfnStream  stream ;
#    endif

{
/*
 * Function: lfn_fd.c
 * Version : 1.0
 * Created : 02-Oct-1997
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function lfn_fd() returns the Unix file descriptor for the socket
 *    connection associated with a LF-terminated network stream.
 *
 *    Invocation:
 *
 *        fd = lfn_fd(stream);
 *
 *    where
 *
 *        <stream>            - I
 *            Is the stream handle returned by lfn_create().
 *
 *        <fd>                - O
 *            Returns the UNIX file descriptor for the stream's socket.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    return((stream == NULL) ? -1 : tcp_fd(stream->connection));

}



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

int  lfn_isup (

#    if __STDC__
    LfnStream  stream)
#    else
    stream)

    LfnStream  stream ;
#    endif

{
/*
 * Function: lfn_isup.c
 * Version : 1.0
 * Created : 02-Oct-1997
 * Author  : Charles Measday
 *
 * Description
 *
 *    The lfn_isup() function checks to see if an LFN stream's underlying
 *    network connection is still up.
 *
 *    Invocation:
 *
 *        isUp = lfn_isup(stream);
 *
 *    where
 *
 *        <stream>            - I
 *            Is the stream handle returned by lfn_create().
 *
 *        <isUp>              - O
 *            Returns true (a non-zero value) if the stream's network
 *            connection is up and false (zero) otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    return ((stream == NULL) ? 0 : tcp_isup(stream->connection));

}



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

char  *lfn_name (

#    if __STDC__
    LfnStream  stream)
#    else
    stream)

    LfnStream  stream ;
#    endif

{
/*
 * Function: lfn_name.c
 * Version : 1.0
 * Created : 02-Oct-1997
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function lfn_name() returns the name of a LF-terminated network stream.
 *
 *    Invocation:
 *
 *        name = lfn_name(stream);
 *
 *    where
 *
 *        <stream>            - I
 *            Is the stream handle returned by lfn_create().
 *
 *        <name>              - O
 *            Returns the stream's name.  The name is stored in memory local
 *            to the LFN utilities and it should not be modified or freed
 *            by the caller.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    return((stream == NULL) ? "" : tcp_name(stream->connection));

}


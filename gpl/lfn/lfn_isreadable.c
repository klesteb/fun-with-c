
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

int  lfn_isreadable (

#    if __STDC__
    LfnStream  stream)
#    else
    stream)

    LfnStream  stream ;
#    endif

{
/*
 * Function: lfn_isreadable.c
 * Version : 1.0
 * Created : 02-Oct-1997
 * Author  : Charles Measday
 *
 * Description
 *
 *    The lfn_isreadable() function checks to see if data is waiting to
 *    be read from a LF-terminated network stream.
 *
 *    Invocation:
 *
 *        isReadable = lfn_isreadable(stream);
 *
 *    where
 *
 *        <stream>            - I
 *            Is the stream handle returned by lfn_create().
 *
 *        <isReadable>        - O
 *            Returns true (a non-zero value) if data is available for
 *            reading and false (zero) otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    if (stream == NULL) {

        return(0);

    } else if (stream->nextChar <= stream->lastChar) {      /* Buffered input? */

        return(1);

    } else return(tcp_isreadable (stream->connection)) ;   /* Real input? */

}


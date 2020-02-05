
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

int  lfn_destroy (

#    if __STDC__
    LfnStream  stream)
#    else
    stream)

    LfnStream  stream ;
#    endif

{
/*
 * Function: lfn_destroy.c
 * Version : 1.0
 * Created : 02-Oct-1997
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function lfn_destroy() destroys a LF-terminated network stream.  The
 *    underlying network connection is closed and the LFN stream structure
 *    is deallocated.
 *
 *    Invocation:
 *
 *        status = lfn_destroy(stream);
 *
 *    where
 *
 *        <stream>            - I
 *            Is the stream handle returned by lfn_create().
 *
 *        <status>            - O
 *            Returns the status of deleting the stream, zero if there
 *            were no errors and ERRNO otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    if (stream == NULL) return(0);

    if (lfn_util_debug || stream->debug)
        printf ("(lfn_destroy) Closing %s stream ...\n", lfn_name(stream));

    /* Close the underlying network connection.                         */

    tcp_destroy(stream->connection);

    /* Deallocate the LFN stream structure.                             */

    if (stream->inputBuffer != NULL)  free(stream->inputBuffer);
    if (stream->inputString != NULL)  free(stream->inputString);
    if (stream->outputString != NULL)  free(stream->outputString);
    free((char *)stream);

    return(0);

}


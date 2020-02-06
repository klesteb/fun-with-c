
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 2002 by Kevin L. Esteb                                     */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include "lpd_priv.h"

/*----------------------------------------------------------------------*/

/**/

int lpd_put(

#    if __STDC__
    LpdHandle handle, int len, void *buffer)
#    else
    handle, len, buffer)

    LpdHandle handle;
    int len;
    void *buffer;
#    endif

{
/*
 * Function: lpd_put.c
 * Version : 1.0
 * Created : 22-Feb-2002
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function lpd_put() will send raw data to the LPD server.
 *
 *    Invocation:
 *
 *        status = lpd_put(handle, len, buffer);
 *
 *    where
 *
 *        <handle>            - I
 *            The handle returned from lpd_open().
 *
 *        <len>               - I
 *            The length of the buffer.
 *
 *        <buffer>            - I
 *            The buffer to be sent.
 *
 *        <status>            - O
 *            This function will return 0 or ERRNO.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat = -1;
    int out_len;

/*
 * Main part of function.
 */

    stat = lfn_write(handle->stream, handle->timeout, len, buffer, &out_len);

    return(stat);

}



/*---------------------------------------------------------------------------*/
/*  Copyright (c) 2000 by Kevin L. Esteb                                     */
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

int lpd_wait_ack(

#    if __STDC__
    LpdHandle handle)
#    else
    handle)

    LpdHandle handle;
#    endif

{
/*
 * Function: lpd_wait_ack.c
 * Version : 1.0
 * Created : 22-Feb-2002
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function lpd_wait_ack() will wait for the LPD ACK. This is usually one 
 *    byte with the value of 0. If the response is something else, then a 
 *    problem has occured. RFC-1179 dosen't mention how to handle problems. 
 *
 *    Invocation:
 *
 *        status = lpd_wait_ack(handle);
 *
 *    where
 *
 *        <handle>            - I
 *            The handle returned from lpd_open().
 *
 *        <status>            - O
 *            This function will return 0 or the one byte response from the
 *            LPD server.
 *
 * Modification History
 *
 * Variables Used
 */

    char resp[LPD_K_BUFSIZ];
    int resp_size;
    int stat;

/*
 * Main part of function.
 */

    memset(resp, '\0', sizeof(resp));

    stat = lfn_read(handle->stream, handle->timeout, 1, resp, &resp_size);
    if (stat == 0) {

        stat = resp[0];

    }

    return(stat);

}


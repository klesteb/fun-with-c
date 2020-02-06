
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

int lpd_start_job(

#    if __STDC__
    LpdHandle handle, char *que)
#    else
    handle, que)

    LpdHandle handle;
    char *que;
#    endif

{
/*
 * Function: lpd_start_job.c
 * Version : 1.0
 * Created : 25-Feb-2002
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function lpd_start_job() will send the "receive a print job" command.
 *
 *    Receiving a job is controlled by a second level of commands. The
 *    daemon is given commands by sending them over the same connection.
 *
 *    After this command is sent, the client must read an acknowledgement
 *    octet from the daemon. A positive acknowledgement is an octet of
 *    zero bits. A negative acknowledgement is an octet of any other
 *    pattern
 *
 *    Invocation:
 *
 *        status = lpd_start_job(handle, que);
 *
 *    where
 *
 *        <handle>            - I
 *            The handle returned from lpd_open().
 *
 *        <que>               - I
 *            The print queue to submit the job to.
 *
 *        <status>            - O
 *            This function will return 0 or ERRNO.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;

/*
 * Main part of function.
 */

    /* Establish the LPD queue.                                         */

    stat = lfn_putline(handle->stream, handle->timeout, "\002%s\n", que);
    if (stat == 0) {

        /* Wait for the ACK.                                            */

        stat = lpd_wait_ack(handle);

    }

    return(stat);

}


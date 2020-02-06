
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

int lpd_end_data(

#    if __STDC__
    LpdHandle handle)
#    else
    handle)

    LpdHandle handle;
#    endif

{
/*
 * Function: lpd_end_data.c
 * Version : 1.0
 * Created : 25-Feb-2002
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will provide a logical end to sending the data file.
 *    Function lpd_end_data() provides a logical end to sending the data
 *    stream to the LPD server.
 *
 *    Invocation:
 *
 *        status = lpd_end_data(handle);
 *
 *    where
 *
 *        <handle>                - I
 *            The handle returned from lpd_open().
 *
 *        <status>                - O
 *            This function will return 0 or ERRNO.
 *
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;

/*
 * Main part of function.
 */

    /* Send an EOF.                                                     */

    stat = lpd_eof(handle);
    if (stat == 0) {

        /* Wait for the ACK.                                            */

        stat = lpd_wait_ack(handle);

    }

    return(stat);

}


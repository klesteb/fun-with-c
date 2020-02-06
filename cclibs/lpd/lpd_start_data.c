
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

int lpd_start_data(

#    if __STDC__
    LpdHandle handle, char *host, int job, int size)
#    else
    handle, host, job, size)

    LpdHandle handle;
    char *host;
    int job;
    int size;
#    endif

{
/*
 * Function: lpd_start_data.c
 * Version : 1.0
 * Created : 25-Feb-2002
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function lpd_start_data() notifies the LPD server that the data
 *    file is now being sent.
 *
 *    The data file may contain any 8 bit values at all. The total number
 *    of bytes in the stream may be sent as the first operand, otherwise
 *    the field should be cleared to 0. The name of the data file should
 *    start with ASCII "dfA". This should be followed by a three digit job
 *    number. The job number should be followed by the host name which has
 *    constructed the data file. Interpretation of the contents of the
 *    data file is determined by the contents of the corresponding control
 *    file. If a data file length has been specified, the next set of
 *    octets over the same TCP connection are the intended contents of the
 *    data file. In this case, once all of the contents have been
 *    delivered, an octet of zero bits is sent as an indication that the
 *    file being sent is complete. A second level of acknowledgement
 *    processing must occur at this point
 *
 *    Invocation:
 *
 *        status = lpd_start_data(handle, host, job, size);
 *
 *    where
 *
 *        <handle>            - I
 *            The handle returned from lpd_open().
 *
 *        <host>              - I
 *            Is the local host.
 *
 *        <job>               - I
 *            The job numer for this print job, RFC-1179 says this should be 
 *            between 1 - 999. It should also match the job number in the 
 *            control file. 
 *
 *        <size>              - I
 *            The total size of the data file in 8 bit bytes.
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

    /* Send the data file information.                                  */

    stat = lfn_putline(handle->stream, handle->timeout, "\003%d dfA%03d%s\n", size, job, host);

    /* The rest of the print file is sent outside this routine.         */
    /* When done a lpd_end_data() should be called.                     */

    return(stat);

}


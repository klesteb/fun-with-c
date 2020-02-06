
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

int lpd_print_jobs(

#    if __STDC__
    LpdHandle handle, char *que)
#    else
    handle, que)

    LpdHandle handle;
    char *que;
#    endif

{
/*
 * Function: lpd_print_jobs.c
 * Version : 1.0
 * Created : 22-Feb-2002
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function lpd_print_job() will send the "print any waiting jobs" 
 *    command. This command starts the printing process if it not already 
 *    running.
 *
 *    Invocation:
 *
 *        status = lpd_print_job(handle, queue);
 *
 *    where
 *
 *        <handle>            - I
 *            The handle returned from lpd_open().
 *
 *        <queue>             - I
 *            The print queue to start print jobs on.
 *
 *        <status>            - O
 *            This function will return 0.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;

/*
 * Main part of function.
 */

    stat = lfn_putline(handle->stream, handle->timeout, "\001%s\n", que);

    return(stat);

}


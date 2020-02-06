
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

int lpd_remove_job(

#    if __STDC__
    LpdHandle handle, char *que, char *agent, int job)
#    else
    handle, que, agent, job)

    LpdHandle handle;
    char *que;
    char *agent;
    int job;
#    endif

{
/*
 * Function: lpd_remove_job.c
 * Version : 1.0
 * Created : 22-Feb-2002
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function lpd_remove_job() will issue a "remove job" command.
 *
 *    This command deletes the print jobs from the specified queue which
 *    are listed as the other operands. If only the agent is given, the
 *    command is to delete the currently active job. Unless the agent is
 *    "root", it is not possible to delete a job which is not owned by the
 *    user. This is also the case for specifying user names instead of
 *    numbers. That is, agent "root" can delete jobs by user name but no
 *    other agents can.
 *
 *    Invocation:
 *
 *        status = lpd_remove_job(handle, que, agent, job);
 *
 *    where
 *
 *        <handle>            - I
 *            The handle returned from lpd_open().
 *
 *        <que>               - I
 *            The print queue the job is on.
 *
 *        <agent>             - I
 *            The "agent" requesting this action.
 *
 *        <job>               - I
 *            The job id number.
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

    stat = lfn_putline(handle->stream, handle->timeout, "\005%s %s %s\n", que, agent, job);

    return(stat);

}



/*---------------------------------------------------------------------------*/
/*               Copyright (c) 2020 by Kevin L. Esteb                        */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include "pjl_priv.h"

/*----------------------------------------------------------------------*/

int pjl_job(

#if __STDC__
    PjlHandle handle, char *jobname, int start, int end, int password, char *display)
#else
    handle, jobname, start, end, password, display)

    PjlHandle handle;
    char *jobname;
    int start;
    int end;
    int password;
    char *display;
#endif

{
/*
 * Function: pjl_job.c
 * Version : 1.0
 * Created : 22-Feb-2020
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will signal the pjl stream that new job is being
 *    started.
 *
 *    Invocation:
 *
 *        status = pjl_job(handle, jobname, start, end, password, display);
 *
 *    where
 *
 *        <handle>            - I
 *            The handle from pjl_create().
 *
 *        <jobname>           - I
 *            The name of the print job.
 *
 *        <start>             - I
 *            The page to start the print job on.
 *
 *        <end>               - I
 *            The page to end the print job on.
 *
 *        <password>          - I
 *            The password to use.
 *
 *        <display>           - I
 *            The text to display on the printers panel.
 *
 *        <status>            - O
 *            This function will return either OK or ERR.
 *
 * Modification History
 *
 * Variables Used
 */

    char temp[32];
    int stat = ERR;
    char *fend = "END = %d ";
    char buffer[PJL_K_BUFSIZ];
    char *fstart = "START = %d ";
    char *fdisplay = "DISPLAY = \"%s\" ";
    char *fpassword = "PASSWORD = \"%d\" ";
    char *command = "@PJL JOB NAME = \"%s\" ";

/*
 * Main part of function.
 */

    if (handle == NULL) {

        vperror("(pjl_job) Invalid parameters.\n");
        goto fini;

    }

    if (strlen(jobname) > 80) {

        jobname[80] = '\0';

    }

    handle->jobname = strdup(jobname);
    sprintf(buffer, command, jobname);

    if (start > 0) {

        memset(temp, '\0', 32);
        snprintf(temp, 31, fstart, start);
        strcat(buffer, temp);

    }

    if (end > 0) {

        memset(temp, '\0', 32);
        snprintf(temp, 31, fend, end);
        strcat(buffer, temp);

    }

    if (password > 0) {

        memset(temp, '\0', 32);
        snprintf(temp, 31, fpassword, password);
        strcat(buffer, temp);

    }

    if (display != NULL) {

        memset(temp, '\0', 32);
        snprintf(temp, 31, fdisplay, display);
        strcat(buffer, temp);

    }

    strcat(buffer, "\r\n");

    if ((stat = _pjl_put(handle, buffer)) != OK) {

        vperror("(pjl_job) Unable to send the JOB command.\n");

    }

    fini:
    return stat;

}


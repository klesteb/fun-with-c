
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <clidef.h>
#include <jpidef.h>
#include <starlet.h>
#include <descrip.h>
#include <lib$routines.h>

#include "vms.h"
#include "que_util.h"
#include "mbxio.h"

/*-Local-Prototypes-----------------------------------------------------*/

extern int   pos(const char *, const char *, int);
extern char *trim(char *);
extern void *xmalloc(int);

/*----------------------------------------------------------------------*/

/**/

ULONG vms_subprocess_exhandler(ULONG *status, void *data) {
/*
 * Function: vms_subprocess_exhandler.c
 * Version : 1.0
 * Created : 26-Nov-2002
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This is a jacket routine to terminate the subprocess for
 *    abnormal program exit.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;
    SUBPROC *sub;

/*
 * Main part of function.
 */

    sub = data;

    stat = sys$delprc(&sub->spawn_id, 0);
    vms_check_status(stat);

    mbx_close(sub->ifd);
    mbx_close(sub->ofd);

    sub->cli_active = FALSE;

    return(0);

}

/**/

int vms_create_subprocess(SUBPROC *sub) {
/*
 * Function: vms_create_subprocess.c
 * Version : 1.0
 * Created : 14-May-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Create a subprocess for the calling program. This sub program will
 *    create the necesary mailboxs, exit handler and will then spawn the 
 *    sub process.
 *
 * Modification History
 *
 * Variables Used
 */

    WORD length;
    int stat;
    int flags;
    ULONG item;
    ULONG privs;
    char input_mbx[128];
    char output_mbx[128];
    char process_id[32];
    struct dsc$descriptor process_id_dsc;
    struct dsc$descriptor output_mbx_dsc;
    struct dsc$descriptor input_mbx_dsc;

/*
 * Main part of function.
 */

    /* Get our process ID.                                              */

    memset(process_id, '\0', 32);
    SET_SDESC(process_id_dsc, process_id, sizeof(process_id) - 1);

    item = JPI$_PID;
    stat = lib$getjpi(&item, 0,0,0, &process_id_dsc, &length);
    vms_check_status(stat);
    trim(process_id);

    /* Setup the input mailbox.                                         */

    privs != 240;
    sprintf(input_mbx, "CLI_INPUT_MBX_%s", process_id);
    sub->ifd = mbx_open(input_mbx, FALSE, privs, mbx_a_both, 256);

    /* Setup the output mailbox.                                        */

    sprintf(output_mbx, "CLI_OUTPUT_MBX_%s", process_id);
    sub->ofd = mbx_open(output_mbx, FALSE, privs, mbx_a_both, 256);

    /* Spawn the subprocess.                                            */

    flags = (CLI$M_NOWAIT | CLI$M_TRUSTED);
    SET_SDESC(input_mbx_dsc, input_mbx, strlen(input_mbx));
    SET_SDESC(output_mbx_dsc, output_mbx, strlen(output_mbx));

    stat = lib$spawn(0, &input_mbx_dsc, &output_mbx_dsc, &flags, 0, 
                     &sub->spawn_id, 0,0,0,0,0,0);
    vms_check_status(stat);

    sub->cli_active = TRUE;

    /* Declare an exception handler to delete the subprocess.            */

    SET_ECB(sub->ecb, vms_subprocess_exhandler, &sub->cli_status, sub);

    stat = sys$dclexh(&sub->ecb);
    vms_check_status(stat);

    return(0);

}

/**/

int vms_execute_command(SUBPROC *sub, char *command, queue *results) {
/*
 * Function: vms_execute_command.c
 * Version : 1.0
 * Created : 14-May-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Send the command to the CLI subprocess. Actually, three commands are
 *    sent to the CLI subprocess:
 *
 *        $on severe_error then continue
 *        < user_command >
 *        $show symbol $status
 *
 *    The "on error" statement prevents the CLI subprocess from aborting
 *    because of errors arising during the execution of the user command.
 *    The output of the "show symbol $status" command signals the completion
 *    status of the user's command and provides the completion status of the
 *    user's command.
 *
 * Modification History
 *
 * Variables Used
 */

    char *err_com = "$on severe_error then continue!discard";
    char *stat_com = "$show symbol $status!discard";

    int i;
    int x;
    int cnt;
    int dcl_status;
    char *buff;

/*
 * Main part of function.
 */

    /* Send the command to the subprocess                               */

    mbx_write(sub->ifd, err_com, strlen(err_com));
    mbx_write(sub->ifd, command, strlen(command));
    mbx_write(sub->ifd, stat_com, strlen(stat_com));

    /* Retrieve the results.                                            */

    buff = xmalloc(257);

    while ((cnt = mbx_read(sub->ofd, buff, 256)) != -1) {

        if ((x = pos(buff, "$STATUS ==", 0)) > 0) {

            dcl_status = atoi(&buff[x+14]);
            break;

        } else {

            i++;
            buff[cnt++] = '\0';
            que_push_tail(results, buff);
            buff = xmalloc(257);

        }

    }

    return(dcl_status);

}

/**/

void vms_delete_subprocess(SUBPROC *sub) {
/*
 * Function: vms_delete_subprocess.c
 * Version : 1.0
 * Created : 14-May-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Delete the subprocess.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;

/*
 * Main part of function.
 */

    if (sub->cli_active == TRUE) {

        stat = sys$delprc(&sub->spawn_id, 0);
        vms_check_status(stat);

        mbx_close(sub->ifd);
        mbx_close(sub->ofd);

        stat = sys$canexh(&sub->ecb);
        vms_check_status(stat);

        sub->cli_active = FALSE;

    }

}



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

#include <opcdef.h>
#include <jpidef.h>
#include <descrip.h>
#include <starlet.h>
#include <lib$routines.h>
#include <stdio.h>
#include <string.h>

#include "vms.h"
#include "mbxio.h"

/*----------------------------------------------------------------------*/

extern int stricmp(const char *, const char *);
extern int pos(const char *, const char *, int);

/*----------------------------------------------------------------------*/

void vms_sndopr_req(char *facility, char *msg, char *reply, int reply_len) {
/*
 * Function: vms_sndopr_req.c
 * Author  : Kevin Esteb
 * Version : 1.1
 * Date    : 29-Sep-2000
 *
 * Synopsis
 *
 *    This function will send a message to the operators console using
 *    opcom, it will wait for a reply from the operator.
 *
 * Modifications
 *
 *
 * Variables
 */

    struct opc_request {
        BYTE ms_type;
        BYTE ms_oper;
        long ms_id;
        char ms_text[120];
    };
    
    struct opc_reply {          /* This structure conflicts with the    */
        BYTE ms_type;           /* documentation for sys$sndopr(), or I */
        BYTE ms_reserv;         /* am missing something, is for VMS 7.1-2.*/
        UWORD ms_status;
        long ms_rplyid;
        char ms_text[255];
    };

    int cnt;
    int stat;
    int offset;
    char *buff;
    WORD chan;
    IFILE *fd;
    WORD length;
    ULONG item;
    ULONG privs;

    char buffer[512];
    char msg_text[280];
    char process_id[32];
    char opcom_mbx[128];
    char process_name[16];

    struct opc_request opc;
    struct opc_reply rpl;
    struct dsc$descriptor process_dsc;
    struct dsc$descriptor input_mbx_dsc;
    struct dsc$descriptor_s opc_desc =
                {0, DSC$K_DTYPE_T, DSC$K_CLASS_S, 0};

/*
 * Main part of function.
 */

    buff = strtok(facility, ",");
    while (buff != NULL) {

        if (stricmp(buff, "CARDS") == 0) {

            opc.ms_oper |= OPC$M_NM_CARDS;

        } else if (stricmp(buff, "CENTRAL") == 0) {

            opc.ms_oper |= OPC$M_NM_CENTRL;

        } else if (stricmp(buff, "CLUSTER") == 0) {

            opc.ms_oper |= OPC$M_NM_CLUSTER;

        } else if (stricmp(buff, "DEVICES") == 0) {

            opc.ms_oper |= OPC$M_NM_DEVICE;

        } else if (stricmp(buff, "DISKS") == 0) {

            opc.ms_oper |= OPC$M_NM_DISKS;

        } else if (stricmp(buff, "LICENSE") == 0) {

            opc.ms_oper |= OPC$M_NM_LICENSE;

        } else if (stricmp(buff, "NETWORK") == 0) {

            opc.ms_oper |= OPC$M_NM_NTWORK;

        } else if (stricmp(buff, "OPER1") == 0) {

            opc.ms_oper |= OPC$M_NM_OPER1;

        } else if (stricmp(buff, "OPER2") == 0) {

            opc.ms_oper |= OPC$M_NM_OPER2;

        } else if (stricmp(buff, "OPER3") == 0) {

            opc.ms_oper |= OPC$M_NM_OPER3;

        } else if (stricmp(buff, "OPER4") == 0) {

            opc.ms_oper |= OPC$M_NM_OPER4;

        } else if (stricmp(buff, "OPER5") == 0) {

            opc.ms_oper |= OPC$M_NM_OPER5;

        } else if (stricmp(buff, "OPER6") == 0) {

            opc.ms_oper |= OPC$M_NM_OPER6;

        } else if (stricmp(buff, "OPER7") == 0) {

            opc.ms_oper |= OPC$M_NM_OPER7;

        } else if (stricmp(buff, "OPER8") == 0) {

            opc.ms_oper |= OPC$M_NM_OPER8;

        } else if (stricmp(buff, "OPER9") == 0) {

            opc.ms_oper |= OPC$M_NM_OPER9;

        } else if (stricmp(buff, "OPER10") == 0) {

            opc.ms_oper |= OPC$M_NM_OPER10;

        } else if (stricmp(buff, "OPER11") == 0) {

            opc.ms_oper |= OPC$M_NM_OPER11;

        } else if (stricmp(buff, "OPER12") == 0) {

            opc.ms_oper |= OPC$M_NM_OPER12;

        } else if (stricmp(buff, "PRINTER") == 0) {

            opc.ms_oper |= OPC$M_NM_PRINT;

        } else if (stricmp(buff, "SECURITY") == 0) {

            opc.ms_oper |= OPC$M_NM_SECURITY;

        } else if (stricmp(buff, "TAPES") == 0) {

            opc.ms_oper |= OPC$M_NM_TAPES;

        }

        buff = strtok(NULL, ",");

    }

    /* Get our process ID.                                              */

    SET_SDESC(process_dsc, process_id, sizeof(process_id) - 1);

    item = JPI$_PID;
    stat = lib$getjpi(&item, 0,0,0, &process_dsc, &length);
    vms_check_status(stat);

    process_id[length] = '\0';
    sprintf(opcom_mbx, "OPCOM_%s", process_id);

    /* Get our process name.                                            */

    SET_SDESC(process_dsc, process_name, sizeof(process_name) - 1);

    item = JPI$_PRCNAM;
    stat = lib$getjpi(&item, 0,0,0, &process_dsc, &length);
    vms_check_status(stat);

    process_name[length] = '\0';

    /* Setup the reply mailbox.                                         */

    privs != 240;
    fd = mbx_open(opcom_mbx, FALSE, privs, mbx_a_both, 290);

    /* Create the OPCOM request.                                        */

    opc.ms_type = OPC$_RQ_RQST;
    opc.ms_id = 0;
    if (strlen(msg) > 104) msg[104] = '\0';
    sprintf(opc.ms_text, "%s, %s", process_name, msg);

    opc_desc.dsc$a_pointer = (char *)&opc;
    opc_desc.dsc$w_length = strlen(opc.ms_text) + 8;

    chan = mbx_lun(fd);
    stat = sys$sndopr(&opc_desc, chan);
    vms_check_status(stat);

    cnt = mbx_read(fd, buffer, 290);

    memcpy(&rpl, buffer, sizeof(struct opc_reply));

    /* It appears that the reply from the opcom request is terminated   */
    /* by a <CR><LF> pair. Find the first occurance of this and copy    */
    /* that portion into our reply buffer.                              */

    cnt = (pos(rpl.ms_text, "\r", 0) - 1);
    cnt = ((cnt > reply_len) ? reply_len : cnt);
    strncpy(reply, rpl.ms_text, cnt);
    reply[cnt] = '\0';

    mbx_close(fd);

}


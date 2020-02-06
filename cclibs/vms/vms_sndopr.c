
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 1998 by Kevin L. Esteb                                     */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include <opcdef.h>
#include <descrip.h>
#include <starlet.h>
#include <lib$routines.h>
#include <string.h>

#include "vms.h"

/*----------------------------------------------------------------------*/

extern int stricmp(const char *, const char *);

/*----------------------------------------------------------------------*/

void vms_sndopr(char *facility, char *msg) {
/*
 * Function: vms_sndopr.c
 * Author  : Kevin Esteb
 * Version : 1.1
 * Date    : 21-Apr-1998
 *
 * Synopsis
 *
 *    This function will send a message to the operators console using
 *    opcom. Allowes a message to be logged into the operators log.
 *
 * Modifications
 *
 *    18-Aug-1999 K.Esteb
 *        This function is now able to determine which opcom facitily to 
 *        send a message too. This is passed as a comma delimited string.
 *
 * Variables
 */

    struct opc_request {
        char ms_type;
        char ms_oper;
        long ms_id;
        char ms_text[120];
    };
    
    int stat;
    char *buff;

    struct opc_request opc;
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

    opc.ms_type = OPC$_RQ_RQST;
    opc.ms_id = 0;
    strncpy(opc.ms_text, msg, 119);

    opc_desc.dsc$a_pointer = (char *)&opc;
    opc_desc.dsc$w_length = strlen(msg) + 8;

    stat = sys$sndopr(&opc_desc, 0);
    vms_check_status(stat);

}


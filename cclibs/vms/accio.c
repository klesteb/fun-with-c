
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 1999 by Kevin L. Esteb                                     */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include <stdlib.h>
#include <string.h>
#include <lnmdef.h>
#include <rmsdef.h>

#include "vms.h"
#include "accio.h"
#include "rmsio.h"

#define ACCOUNTNG "SYS$MANAGER:ACCOUNTNG.DAT"

/*-Prototypes-----------------------------------------------------------*/

extern void *xmalloc(int);

static int decode_id(ACRDEF2 *, ACR_IDENT *);
static int decode_resource(ACRDEF3 *, ACR_RESOURCE *);
static int decode_imagename(ACRDEF4 *, ACR_IMAGENAME *);
static int decode_print(ACRDEF5 *, ACR_PRINT *);
static int decode_filename(ACRDEF6 *, ACR_FILENAME *);
static int decode_userdata(ACRDEF7 *, ACR_USERDATA *);
static void *get_data(void *, int, int *);

/*----------------------------------------------------------------------*/

/**/

RFILE *acc_open(char *filename) {
/*
 * Function: acc_open.c
 * Version : 1.0
 * Created : 17-Nov-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will open a VMS accounting file. The accouting 
 *    filename will be determined by one of three methods:
 *
 *        1) Passed to this rountine.
 *        2) By the system wide logical ACCOUNTNG.
 *        3) By defaulting to SYS$MANAGER:ACCOUNTNG.DAT.
 *
 *    A successfull opening of the file will return a RFILE, otherwise
 *    NULL will be returned.
 *
 * Modification History
 *
 * Variables Used
 */

    RFILE *fp = NULL;
    char accounting[256];

/*
 * Main part of function.
 */

    if (strlen(filename) == 0) {

        vms_trnlnm("LNM$SYSTEM_TABLE", "ACCOUNTNG", ACCOUNTNG, LNM$_STRING, accounting);

    } else strcpy(accounting, filename);

    fp = rms_open(accounting, RMS_O_ACCESS, RMS_K_ACCESS_READ, 
                  RMS_O_ALLOW, RMS_K_ALLOW_UPDATE, RMS_O_END);

    return(fp);

}

/**/

void acc_close(RFILE *fp) {
/*
 * Function: acc_close.c
 * Version : 1.0
 * Created : 17-Nov-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will close the accounting file.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    rms_close(fp);

}

/**/

int acc_read_record(RFILE *fp, void *buffer, int buflen) {
/*
 * Function: acc_read_record.c
 * Version : 1.0
 * Created : 17-Nov-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will read a record from the accounting file. Each file 
 *    record will contain a header block and multiple packet blocks. The 
 *    type of header block will indicate how many packets are contained in 
 *    the record.
 *
 * Modification History
 *
 * Variables Used
 */

    size_t count;

/*
 * Main part of function.
 */

    memset(buffer, '\0', buflen);

    if (rms_getseq(fp, buffer, buflen, &count) != RMS$_EOF) {

        return(count);

    }

    return(-1);

}

/**/

int acc_get_header(void *buffer, int buflen, int *length, int *type, 
                   int *subtype, QUADWORD *systime) {
/*
 * Function: acc_get_record.c
 * Version : 1.0
 * Created : 17-Nov-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will get the header block. The length, type and subtype
 *    are returned to the caller. 
 *
 * Modification History
 *
 * Variables Used
 */

    ACR_HEADER header;

/*
 * Main part of function.
 */

    *type = 0;
    *length = 0;
    *subtype = 0;

    memcpy(&header, buffer, sizeof(ACR_HEADER));

    /* Check to see if this is really a header block, and if the       */
    /* length of the buffer is really the same as the record length.    */
    /* Also check what version of the accounting system is being        */
    /* used. These routines only understand the ACR$K_VERSION3 packet   */
    /* structure.                                                       */

    if ((header.x.acr$v_packet == 0) && 
        (header.x.acr$w_length == buflen) &&
        (header.x.acr$v_version == ACR$K_VERSION3)) {

        *type = header.x.acr$v_type;
        *length = header.x.acr$w_length;
        *subtype = header.x.acr$v_subtype;
        memcpy(systime, &header.y.acr$q_systime, sizeof(QUADWORD));

        return(TRUE);

    }

    return(FALSE);

}

/**/

void *acc_get_packet(void *buffer, int buflen, int offset, int *length, 
                     int *type, int *subtype) {
/*
 * Function: acc_get_packet.c
 * Version : 1.0
 * Created : 17-Nov-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will return a packet from the buffer. The packet will 
 *    start at offset. The first four bytes will contain a structure that
 *    defines the packet type and how big it is. The packet will be copied 
 *    out of the buffer and the packet length, type and subtype will be 
 *    returned to the caller.
 *
 * Modification History
 *
 * Variables Used
 */

    ACRDEF pheader;
    void *packet = NULL;
    char *start = NULL;
    char *buff = NULL;

/*
 * Main part of function.
 */

    *type = 0;
    *length = 0;
    *subtype = 0;

    buff = buffer;
    start = buff + offset;
    memcpy(&pheader, start, sizeof(ACRDEF));

    /* Check to see if this is really a packet block and if the packet  */
    /* length is less then the buffer size. Also check to see what      */
    /* version of the accounting system is being used. These routines   */
    /* only understand the ACR$K_VERSION3 packet structure.             */

    if ((pheader.acr$v_packet == 1) && 
        (pheader.acr$w_length < buflen) &&
        (pheader.acr$v_version == ACR$K_VERSION3)) {

        *type = pheader.acr$v_type;
        *length = pheader.acr$w_length;
        *subtype = pheader.acr$v_subtype;

        packet = xmalloc(pheader.acr$w_length);
        memcpy(packet, start, pheader.acr$w_length);

        return(packet);

    }

    return(NULL);

}

/**/

int acc_decode_packet(int type, int subtype, void *packet, void *record) {
/*
 * Function: acc_decode_packet.c
 * Version : 1.0
 * Created : 17-Nov-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will decode the packet based on type and return 
 *    that information in the record.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;

/*
 * Main part of function.
 */

    switch (type) {
        case ACR$K_ID:
            stat = decode_id(packet, record);
            break;

        case ACR$K_RESOURCE:
            stat = decode_resource(packet, record);
            break;

        case ACR$K_IMAGENAME:
            stat = decode_imagename(packet, record);
            break;

        case ACR$K_PRINT:
            stat = decode_print(packet, record);
            break;

        case ACR$K_FILENAME:
            stat = decode_filename(packet, record);
            break;

        case ACR$K_USER_DATA:
            stat = decode_userdata(packet, record);
            break;

        default:
            stat = FALSE;

    }

    return(stat);
        
}

/**/

void acc_free_record(int type, int subtype, void *record) {
/*
 * Function: acc_free_record.c
 * Version : 1.0
 * Created : 18-Nov-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will free the memory used by data items in 
 *    each record type.
 *
 * Modification History
 *
 * Variables Used
 */

    ACR_IDENT *ident = NULL;
    ACR_IMAGENAME *image = NULL;
    ACR_FILENAME *file = NULL;
    ACR_USERDATA *user = NULL;

/*
 * Main part of function.
 */

    switch (type) {
        case ACR$K_ID:
            ident = record;
            if (ident->username != NULL) free(ident->username);
            if (ident->account != NULL)  free(ident->account);
            if (ident->nodename != NULL) free(ident->nodename);
            if (ident->terminal != NULL) free(ident->terminal);
            if (ident->jobname != NULL)  free(ident->jobname);
            if (ident->queue != NULL)    free(ident->queue);
            if (ident->nodeaddr != NULL) free(ident->nodeaddr);
            if (ident->remoteid != NULL) free(ident->remoteid);
            if (ident->fulladdr != NULL) free(ident->fulladdr);
            if (ident->fullname != NULL) free(ident->fullname);
            break;

        case ACR$K_IMAGENAME:
            image = record;
            if (image->imagename != NULL) free(image->imagename);
            break;

        case ACR$K_FILENAME:
            file = record;
            if (file->filename != NULL) free(file->filename);
            break;

        case ACR$K_USER_DATA:
            user = record;
            user->length = 0;
            if (user->userdata != NULL) free(user->userdata);
            break;

        case ACR$K_PRINT:
        case ACR$K_RESOURCE:
            break;

        default:
            break;

    }

}

/**/

void acc_free_packet(int type, int subtype, void *packet) {
/*
 * Function: acc_free_packet.c
 * Version : 1.0
 * Created : 18-Nov-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will free the memory used by the packet.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    free(packet);

}

/**/

static int decode_id(ACRDEF2 *packet, ACR_IDENT *record) {
/*
 * Function: decode_id.c
 * Version : 1.0
 * Created : 17-Nov-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will move items from the packet to the data record.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat = FALSE;
    int length;
    ACRDEF pheader;

/*
 * Main part of function.
 */

    memcpy(&pheader, packet, sizeof(ACRDEF));

    if (pheader.acr$v_type == ACR$K_ID) {

        record->pid = packet->acr$l_pid;
        record->owner = packet->acr$l_owner;
        record->uic = packet->acr$l_uic;
        memcpy(&record->priv, &packet->acr$q_priv, sizeof(QUADWORD));
        record->idflgs = packet->acr$b_idflgs;
        record->jobid = packet->acr$l_jobid;
        record->username = get_data(packet, packet->acr$w_username, &length);
        record->account = get_data(packet, packet->acr$w_account, &length);
        record->nodename = get_data(packet, packet->acr$w_nodename, &length);
        record->terminal = get_data(packet, packet->acr$w_terminal, &length);
        record->jobname = get_data(packet, packet->acr$w_jobname, &length);
        record->queue = get_data(packet, packet->acr$w_queue, &length);
        record->nodeaddr = get_data(packet, packet->acr$w_nodeaddr, &length);
        record->remoteid = get_data(packet, packet->acr$w_remoteid, &length);
        record->fulladdr = get_data(packet, packet->acr$w_fulladdr, &length);
        record->fullname = get_data(packet, packet->acr$w_fullname, &length);

        stat = TRUE;
    
    }

    return(stat);

}

/**/

static int decode_resource(ACRDEF3 *packet, ACR_RESOURCE *record) {
/*
 * Function: decode_resource.c
 * Version : 1.0
 * Created : 18-Nov-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will move items from the packet to the data record.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat = FALSE;
    ACRDEF pheader;

/*
 * Main part of function.
 */

    memcpy(&pheader, packet, sizeof(ACRDEF));

    if (pheader.acr$v_type == ACR$K_RESOURCE) {

        memcpy(&record->login, &packet->acr$q_login, sizeof(QUADWORD));
        record->status = packet->acr$l_status;
        record->imgcnt = packet->acr$l_imgcnt;
        record->cputime = packet->acr$l_cputime;
        record->faults = packet->acr$l_faults;
        record->faultio = packet->acr$l_faultio;
        record->wspeak = packet->acr$l_wspeak;
        record->pagefl = packet->acr$l_pagefl;
        record->diocnt = packet->acr$l_diocnt;
        record->biocnt = packet->acr$l_biocnt;
        record->volumes = packet->acr$l_volumes;
        record->vp_cputime = packet->acr$l_vp_cputime;

        stat = TRUE;
    
    }

    return(stat);

}

/**/

static int decode_imagename(ACRDEF4 *packet, ACR_IMAGENAME *record) {
/*
 * Function: decode_imagename.c
 * Version : 1.0
 * Created : 18-Nov-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will move items from the packet to the data record.
 *
 * Modification History
 *
 * Variables Used
 */

    int off;
    int length;
    int stat = FALSE;
    ACRDEF pheader;

/*
 * Main part of function.
 */

    memcpy(&pheader, packet, sizeof(ACRDEF));

    if (pheader.acr$v_type == ACR$K_IMAGENAME) {

        off = sizeof(ACRDEF);
        record->imagename = get_data(packet, off, &length);

        stat = TRUE;
    
    }

    return(stat);

}

/**/

static int decode_print(ACRDEF5 *packet, ACR_PRINT *record) {
/*
 * Function: decode_print.c
 * Version : 1.0
 * Created : 18-Nov-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will move items from the packet to the data record.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat = FALSE;
    ACRDEF pheader;

/*
 * Main part of function.
 */

    memcpy(&pheader, packet, sizeof(ACRDEF));

    if (pheader.acr$v_type == ACR$K_PRINT) {

        record->printsts = packet->acr$l_printsts;
        memcpy(&record->quetime, &packet->acr$q_quetime, sizeof(QUADWORD));
        memcpy(&record->begtime, &packet->acr$q_begtime, sizeof(QUADWORD));
        record->symcputim = packet->acr$l_symcputim;
        record->pagecnt = packet->acr$l_pagecnt;
        record->qiocnt = packet->acr$l_qiocnt;
        record->getcnt = packet->acr$l_getcnt;

        stat = TRUE;
    
    }

    return(stat);

}

/**/

static int decode_filename(ACRDEF6 *packet, ACR_FILENAME *record) {
/*
 * Function: decode_filename.c
 * Version : 1.0
 * Created : 18-Nov-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will move items from the packet to the data record.
 *
 * Modification History
 *
 * Variables Used
 */

    int off;
    int length;
    int stat = FALSE;
    ACRDEF pheader;

/*
 * Main part of function.
 */

    memcpy(&pheader, packet, sizeof(ACRDEF));

    if (pheader.acr$v_type == ACR$K_FILENAME) {

        off = sizeof(ACRDEF);
        record->filename = get_data(packet, off, &length);

        stat = TRUE;
    
    }

    return(stat);

}

/**/

static int decode_userdata(ACRDEF7 *packet, ACR_USERDATA *record) {
/*
 * Function: decode_userdata.c
 * Version : 1.0
 * Created : 18-Nov-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will move items from the packet to the data record.
 *
 * Modification History
 *
 * Variables Used
 */

    int off;
    int length;
    int stat = FALSE;
    ACRDEF pheader;

/*
 * Main part of function.
 */

    memcpy(&pheader, packet, sizeof(ACRDEF));

    if (pheader.acr$v_type == ACR$K_USER_DATA) {

        off = sizeof(ACRDEF);
        record->userdata = get_data(packet, off, &length);
        record->length = length;

        stat = TRUE;
    
    }

    return(stat);

}

/**/

static void *get_data(void *packet, int offset, int *length) {
/*
 * Function: get_data.c
 * Version : 1.0
 * Created : 19-Nov-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will return the data from the packet starting at
 *    offset. Offset is the beginning of a counted string.
 *
 * Modification History
 *
 * Variables Used
 */

    char *buffer = NULL;
    char *data = NULL;

/*
 * Main part of function.
 */

    buffer = packet;
    *length = 0;

    if (offset > 0) {

        buffer += offset;
        *length = *buffer++;

        data = xmalloc(*length + 1);
        memcpy(data, buffer, *length);
        data[*length + 1] ='\0';

        return(data);

    }

    return(NULL);

}


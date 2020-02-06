
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
#include <ssdef.h>
#include <iodef.h>
#include <cmbdef.h>
#include <dvidef.h>
#include <descrip.h>
#include <starlet.h>
#include <lib$routines.h>

#include "vms.h"
#include "mbxio.h"

/*----------------------------------------------------------------------*/

IFILE *mbx_open(char *locnam, ULONG perm, ULONG privs, ULONG acc_mode, 
                ULONG rec_size) {
/*
 * Function: mbx_open.c
 * Version : 1.0
 * Created : 12-May-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will create a mailbox device.
 *
 * Modification History
 *
 *    25-Sep-2001 K.Esteb
 *        Changed the function return to use the new IFILE stucture.
 *
 * Variables Used
 */

    BYTE prmflg = 0;
    WORD chan;
    long stat;
    ULONG flags;
    IFILE *fd = NULL;
    struct dsc$descriptor locnam_dsc;

/*
 * Main part of function.
 */

    if (perm == TRUE) prmflg = 1;

    switch (acc_mode) {
        case mbx_a_read_only:
            flags = CMB$M_READONLY;
            break;

        case mbx_a_write_only:
            flags = CMB$M_WRITEONLY;
            break;

        default:
            flags = 0;

    }

    if ((fd = calloc(1, sizeof(IFILE))) != NULL) {

        fd->ef = vms_get_ef();
        fd->qued = FALSE;
        fd->bufsiz = rec_size;

        if ((fd->buffer = malloc(rec_size + 1)) == NULL) {

            free(fd);
            return(NULL);

        }

        SET_SDESC(locnam_dsc, locnam, strlen(locnam));

        stat = sys$crembx(prmflg, &fd->lun, rec_size, 0, privs, 0, &locnam_dsc, flags);
        vms_check_status(stat);

    }

    return(fd);

}

/**/

void mbx_close(IFILE *fd) {
/*
 * Function: mbx_close.c
 * Version : 1.0
 * Created : 11-May-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will close the mailbox channel.
 *
 * Modification History
 *
 *    25-Sep-2001 K.Esteb
 *        Changed the function to use the new IFILE stucture. I will now
 *        release the event flag and free any used memory.
 *
 * Variables Used
 */

    long stat;

/*
 * Main part of function.
 */

    if (fd != NULL) {

        stat = sys$delmbx(fd->lun);
        vms_check_status(stat);

        stat = sys$dassgn(fd->lun);
        vms_check_status(stat);

        vms_free_ef(fd->ef);

        free(fd->buffer);
        free(fd); 

    }

}

/**/

int mbx_read(IFILE *fd, void *buff, int length) {
/*
 * Function: mbx_read.c
 * Version : 1.0
 * Created : 11-May-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Read a buffer from the mailbox. Return the number of bytes
 *    actually read. If an end-of-file is encoutered then return -1.
 *
 * Modification History
 *
 *    25-Sep-2001 K.Esteb
 *        Changed the function to use the new IFILE stucture.
 *
 * Variables Used
 */

    ULONG func;
    int stat;
    int cnt;

/*
 * Main part of function.
 */

    if (fd->qued == FALSE) {

        func = (IO$_READVBLK);

        stat = sys$qiow(0, fd->lun, func, &fd->iosb.quad, 0,0, buff, length, 0,0,0,0);
        if (stat != SS$_ENDOFFILE) {

            vms_check_status(stat);
            cnt = fd->iosb.io.byte_count;

        } else cnt = -1;

    } else {

        fd->qued = FALSE;

        if (fd->iosb.io.stat != SS$_ENDOFFILE) {

            vms_check_status(fd->iosb.io.stat);
            cnt = fd->iosb.io.byte_count;
            memcpy(buff, &fd->buffer, cnt);

        } else cnt = -1;

    }

    return(cnt);

}

/**/

int mbx_write(IFILE *fd, void *buff, int length) {
/*
 * Function: mbx_write.c
 * Version : 1.0
 * Created : 11-May-1998
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Write a buffer to the mailbox. Return the actual number of bytes
 *    written.
 *
 * Modification History
 *
 *    25-Sep-2001 K.Esteb
 *        Changed the function to use the new IFILE stucture.
 *
 * Variables Used
 */

    ULONG func;
    int stat;

/*
 * Main part of function.
 */

    func = (IO$_WRITEVBLK | IO$M_NOW | IO$M_NORSWAIT);

    stat = sys$qiow(0, fd->lun, func, &fd->iosb.quad, 0,0, buff, length, 0,0,0,0);
    vms_check_status(stat);
    vms_check_status(fd->iosb.io.stat);

    return(fd->iosb.io.byte_count);

}
 
/**/

int mbx_count(IFILE *fd) {
/*
 * Function: mbx_count.c
 * Version : 1.0
 * Created : 11-May-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *     This function will return the number of records in the mailbox.
 *
 * Modification History
 *
 *    25-Sep-2001 K.Esteb
 *        Changed the function to use the new IFILE stucture.
 *
 * Variables Used
 */

    int stat;
    ULONG mbx_recs;
    ULONG mbx_recs_len;
    ITEM_LIST_3 getdvi[2];

/*
 * Main part of function.
 */

    SET_ITEM3(getdvi[0], DVI$_DEVDEPEND, &mbx_recs, 4, &mbx_recs_len);
    SET_ITEM3(getdvi[1], 0, 0, 0, 0);

    stat = sys$getdvi(0, fd->lun, &getdvi, &fd->iosb.quad, 0,0,0);
    vms_check_status(stat);

    return(mbx_recs);

}

/**/

void mbx_eof(IFILE *fd) {
/*
 * Function: mbx_eof.c
 * Version : 1.0
 * Created : 11-May-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Send an end-of-file marker to a mailbox.
 *
 * Modification History
 *
 *    25-Sep-2001 K.Esteb
 *        Changed the function to use the new IFILE stucture.
 *
 * Variables Used
 */

    ULONG func;
    int stat;

/*
 * Main part of function.
 */

    func = (IO$_WRITEOF | IO$M_NOW | IO$M_NORSWAIT);

    stat = sys$qiow(0, fd->lun, func, &fd->iosb.quad, 0,0,0,0,0,0,0,0);
    vms_check_status(stat);
    vms_check_status(fd->iosb.io.stat);

}

/**/

char *mbx_name(IFILE *fd) {
/*
 * Function: mbx_name.c
 * Version : 1.0
 * Created : 11-May-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Return the name of a mailbox. The resulting name buffer must 
 *    be freed with free();
 *
 * Modification History
 *
 *    25-Sep-2001 K.Esteb
 *        Changed the function to use the new IFILE stucture.
 *
 * Variables Used
 */

    int stat;
    char *result = NULL;

/*
 * Main part of function.
 */

    if ((result = malloc(255)) != NULL) {

        vms_getdvi("", fd->lun, DVI$_ALLDEVNAM, result);

    }

    return(result);

}


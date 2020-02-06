
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <rms.h>
#include <ssdef.h>
#include <acldef.h>
#include <armdef.h>
#include <chpdef.h>
#include <fdldef.h>
#include <jpidef.h>
#include <descrip.h>
#include <stsdef.h>
#include <starlet.h>
#include <lib$routines.h>
#include <fdl$routines.h>

#include "vms.h"

struct RHB {                     /* record header buffer                 */
    unsigned char prefix;
    unsigned char postfix;
};

typedef struct {
    struct FAB fab;
    struct RAB rab;
    struct NAM nam;
    struct RHB rhb;
    struct XABDAT dat;
    struct XABFHC fhc;
    struct XABPRO pro;
    int index;
    union {
        int rc;
        struct {
            unsigned severity:1;
            unsigned return_condition:31;
        } status_parts;
    } status;
} RFILE;

#define RFILE_T 1
#include "rmsio.h"

/*----------------------------------------------------------------------*/

/**/

int rms_create(char *fdl, char *fname) {
/*
 * Function: rms_create.c
 * Version : 1.0
 * Created : 20-Dec-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Use fdl$create() to create a new file.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat, flags;
    struct dsc$descriptor fdl_dsc;
    struct dsc$descriptor fln_dsc;

/*
 * Main part of function.
 */

    /* Set descriptors to point to passed strings and call FDL$CREATE.  */

    SET_SDESC(fdl_dsc, fdl, strlen(fdl));
    SET_SDESC(fln_dsc, fname, strlen(fname));

    flags = (FDL$M_FDL_STRING | FDL$M_SIGNAL);

    stat = fdl$create(&fdl_dsc, &fln_dsc, 0,0,0, &flags, 0,0,0,0,0);

    return(stat);

}

/**/

int rms_check_access(char *fname, int uic) {
/*
 * Function: rms_check_access.c
 * Version : 1.0
 * Created : 20-Dec-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Retrieve ACL on indicated filename and see if it permits read access
 *    to specified UIC.
 *
 * Modification History
 *
 * Variables Used
 */

    ITEM_LIST_3 items[10];
    int stat, acllen,  context, objtype;
    long access, rights[2], privs[2];
    char *acl;
    struct dsc$descriptor fln_dsc;

/*
 * Main part of function.
 */

    /* Get length of ACL.                                               */

    SET_ITEM3(items[0], ACL$C_ACLLENGTH, &acllen, 4, 0);
    SET_ITEM3(items[1], 0, 0, 0, 0);
    SET_SDESC(fln_dsc, fname, strlen(fname));

    objtype = ACL$C_FILE;
    context = 0;

    stat = sys$change_acl(0, &objtype, &fln_dsc, &items, 0,0,0,0,0);
    if ((stat & 1) == 0) return(stat);

    /* Allocate buffer and read entire ACL.                             */

    acl = malloc(acllen);
    if (!acl) return(12);

    SET_ITEM3(items[0], ACL$C_READACL, acl, acllen, &acllen)
    SET_ITEM3(items[1], 0, 0, 0, 0);

    stat = sys$change_acl( 0, &objtype, &fln_dsc, &items, 0,0,0,0,0);
    if (stat == SS$_ACLEMPTY ) {

        /* Make phony ACL granting access to current process UIC.       */

        int iosb[2], pid, proc_uic;
        SET_ITEM3(items[0], JPI$_UIC, &proc_uic, 4, 0);
        SET_ITEM3(items[1], 0, 0, 0, 0);
        pid = 0;

        stat = sys$getjpiw(0, &pid, 0, &items, &iosb, 0,0);
        if ((stat & 1) == 1) stat = iosb[0];
        if ((stat & 1) == 1) {
            if (proc_uic != uic) stat = SS$_NOPRIV;
        }

    } else if ((stat & 1) == 1) {

        /* We have ACL, construct item list for $CHKPRO.                */

        access = ARM$M_READ;
        rights[0] = uic;
        rights[1] = 0;
        privs[0] = privs[1] = 0;

        SET_ITEM3(items[0], CHP$_ACCESS, &access, 4, 0);
        SET_ITEM3(items[1], CHP$_RIGHTS, rights, 8, 0);
        SET_ITEM3(items[2], CHP$_PRIV, privs, 8, 0);
        SET_ITEM3(items[3], CHP$_ACL, acl, acllen, 0);
        SET_ITEM3(items[4], 0, 0, 0, 0);

        stat = sys$chkpro(&items);

    }

    free(acl);

    return(stat);

}

/**/

RFILE *rms_open(char *fn, int params, ...) {
/*
 * Function: rms_open.c
 * Version : 1.0
 * Created : 20-Dec-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Create the data structures needed to open an RMS file for processing.
 *    Invoke the sys$open() and sys$connect() functions to access that file.
 *    If this is successful then return a pointer to the RFILE otherwise
 *    return NULL.
 *
 * Modification History
 *
 * Variables Used
 */

    RFILE *rfile = NULL;
    int item;
    int access_mode;
    int allow_mode;
    int rec_mode;
    va_list ap;

/*
 * Main part of function.
 */

    if ((rfile = malloc(sizeof(RFILE))) == NULL) return(NULL);

    rfile->fab = cc$rms_fab;
    rfile->rab = cc$rms_rab;
    rfile->dat = cc$rms_xabdat;
    rfile->fhc = cc$rms_xabfhc;
    rfile->pro = cc$rms_xabpro;

    va_start(ap, params);

    item = params;

    do {

        switch (item) {
            case RMS_O_ACCESS:
                access_mode = va_arg(ap, int);
                break;

            case RMS_O_ALLOW:
                allow_mode = va_arg(ap, int);
                break;

            default:
                free(rfile);
                fprintf(stderr, "rms_open() - invalid parameter\n");
                return(NULL);

        }

    } while ((item = va_arg(ap, int)) != RMS_O_END);

    va_end(ap);

    switch (access_mode) {
        case RMS_K_ACCESS_APPEND:
            rfile->fab.fab$b_fac = (FAB$M_PUT);
            rfile->rab.rab$l_rop = (RAB$M_EOF);
            break;

        case RMS_K_ACCESS_READ:
            rfile->fab.fab$b_fac = (FAB$M_GET);
            break;

        case RMS_K_ACCESS_WRITE:
            rfile->fab.fab$b_fac = (FAB$M_DEL | FAB$M_GET | FAB$M_PUT);
            break;

        case RMS_K_ACCESS_UPDATE:
            rfile->fab.fab$b_fac = (FAB$M_DEL | FAB$M_GET | FAB$M_PUT | 
                                    FAB$M_UPD);
            break;

        default:
            free(rfile);
            fprintf(stderr, "rms_open() - invalid access mode \"%d\"", access_mode);
            return(NULL);

    }

    switch (allow_mode) {
        case RMS_K_ALLOW_NONE:
            rfile->fab.fab$b_shr = 0;
            break;

        case RMS_K_ALLOW_READ:
            rfile->fab.fab$b_shr = (FAB$V_MSE | FAB$V_SHRGET);
            break;

        case RMS_K_ALLOW_WRITE:
            rfile->fab.fab$b_shr = (FAB$V_MSE | FAB$V_SHRPUT | 
                                    FAB$V_SHRGET | FAB$V_SHRDEL);
            break;

        case RMS_K_ALLOW_UPDATE:
            rfile->fab.fab$b_shr = (FAB$V_MSE | FAB$V_SHRPUT | 
                                    FAB$V_SHRGET | FAB$V_SHRDEL | FAB$V_SHRUPD);
            break;

        default:
            free(rfile);
            fprintf(stderr, "rms_open() - invalid allow mode \"%d\"", allow_mode);
            return(NULL);

    }

    rfile->fab.fab$l_fna = fn;
    rfile->fab.fab$b_fns = strlen(fn);
    rfile->fab.fab$l_xab = (char *)&(rfile->dat);
    rfile->dat.xab$l_nxt = (char *)&(rfile->fhc);
    rfile->fhc.xab$l_nxt = (char *)&(rfile->pro);

    rfile->status.rc = sys$open(&(rfile->fab), 0, 0);

    if (rfile->status.status_parts.severity != STS$K_SUCCESS) {

        free(rfile);
        return(NULL);

    }

    rfile->rab.rab$l_fab = &(rfile->fab);

    rfile->status.rc = sys$connect(&(rfile->rab), 0, 0);

    if (rfile->status.status_parts.severity != STS$K_SUCCESS) {

        free(rfile);
        return(NULL);

    }

    rfile->index = -1;

    return(rfile);

}

/**/

RFILE *rms_open_blk(char *fn, int params, ...) {
/*
 * Function: rms_openb.c
 * Version : 1.0
 * Created : 20-Dec-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Create the data structures needed to open an RMS file for processing.
 *    Invoke the sys$open() and sys$connect() functions to access that file.
 *    If this is successful then return a pointer to the RFILE otherwise
 *    return NULL.
 *
 * Modification History
 *
 * Variables Used
 */

    RFILE *rfile = NULL;
    int item;
    int access_mode;
    int allow_mode;
    int rec_mode;
    va_list ap;

/*
 * Main part of function.
 */

    if ((rfile = malloc(sizeof(RFILE))) == NULL) return(NULL);

    rfile->fab = cc$rms_fab;
    rfile->rab = cc$rms_rab;
    rfile->dat = cc$rms_xabdat;
    rfile->fhc = cc$rms_xabfhc;
    rfile->pro = cc$rms_xabpro;

    va_start(ap, params);

    item = params;

    do {

        switch (item) {
            case RMS_O_ACCESS:
                access_mode = va_arg(ap, int);
                break;

            case RMS_O_ALLOW:
                allow_mode = va_arg(ap, int);
                break;

            default:
                free(rfile);
                fprintf(stderr, "rms_open() - invalid parameter\n");
                return(NULL);

        }

    } while ((item = va_arg(ap, int)) != RMS_O_END);

    va_end(ap);

    switch (access_mode) {
        case RMS_K_ACCESS_APPEND:
            rfile->fab.fab$b_fac = (FAB$M_PUT | FAB$M_BIO);
            rfile->rab.rab$l_rop = (RAB$M_BIO | RAB$M_EOF);
            break;

        case RMS_K_ACCESS_READ:
            rfile->fab.fab$b_fac = (FAB$M_GET | FAB$M_BIO);
            rfile->rab.rab$l_rop = RAB$M_BIO;
            break;

        case RMS_K_ACCESS_WRITE:
            rfile->fab.fab$b_fac = (FAB$M_GET | FAB$M_PUT | FAB$M_BIO);
            rfile->rab.rab$l_rop = RAB$M_BIO;
            break;

        default:
            free(rfile);
            fprintf(stderr, "rms_open_blk() - invalid access mode \"%d\"", access_mode);
            return(NULL);

    }

    switch (allow_mode) {
        case RMS_K_ALLOW_NONE:
            rfile->fab.fab$b_shr = 0;
            break;

        case RMS_K_ALLOW_READ:
            rfile->fab.fab$b_shr = (FAB$M_MSE | FAB$M_UPI);
            break;

        case RMS_K_ALLOW_WRITE:
            rfile->fab.fab$b_shr = (FAB$M_MSE | FAB$M_UPI);
            break;

        default:
            free(rfile);
            fprintf(stderr, "rms_open_blk() - invalid allow mode \"%d\"", allow_mode);
            return(NULL);

    }

    rfile->fab.fab$l_fna = fn;
    rfile->fab.fab$b_fns = strlen(fn);
    rfile->fab.fab$l_xab = (char *)&(rfile->dat);
    rfile->dat.xab$l_nxt = (char *)&(rfile->fhc);
    rfile->fhc.xab$l_nxt = (char *)&(rfile->pro);

    rfile->status.rc = sys$open(&(rfile->fab), 0, 0);

    if (rfile->status.status_parts.severity != STS$K_SUCCESS) {

        free(rfile);
        return(NULL);

    }

    rfile->rab.rab$l_fab = &(rfile->fab);

    rfile->status.rc = sys$connect(&(rfile->rab), 0, 0);

    if (rfile->status.status_parts.severity != STS$K_SUCCESS) {

        free(rfile);
        return(NULL);

    }

    rfile->index = -1;

    return(rfile);

}

/**/

RFILE *rms_open_fid(fid fid, int params, ...) {
/*
 * Function: rms_open_fid.c
 * Version : 1.0
 * Created : 20-Dec-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Create the data structures needed to open an RMS file for processing.
 *    Invoke the sys$open() and sys$connect() functions to access that file.
 *    If this is successful then return a pointer to the RFILE otherwise
 *    return NULL.
 *
 * Modification History
 *
 * Variables Used
 */

    RFILE *rfile = NULL;
    int item;
    int access_mode;
    int allow_mode;
    int rec_mode;
    va_list ap;

/*
 * Main part of function.
 */

    if ((rfile = malloc(sizeof(RFILE))) == NULL) return(NULL);

    rfile->fab = cc$rms_fab;
    rfile->rab = cc$rms_rab;
    rfile->dat = cc$rms_xabdat;
    rfile->fhc = cc$rms_xabfhc;
    rfile->pro = cc$rms_xabpro;

    va_start(ap, params);

    item = params;

    do {

        switch (item) {
            case RMS_O_ACCESS:
                access_mode = va_arg(ap, int);
                break;

            case RMS_O_ALLOW:
                allow_mode = va_arg(ap, int);
                break;

            default:
                free(rfile);
                fprintf(stderr, "rms_open() - invalid parameter\n");
                return(NULL);

        }

    } while ((item = va_arg(ap, int)) != RMS_O_END);

    va_end(ap);

    switch (access_mode) {
        case RMS_K_ACCESS_APPEND:
            rfile->fab.fab$b_fac = (FAB$M_PUT);
            rfile->rab.rab$l_rop = (RAB$M_EOF);
            break;

        case RMS_K_ACCESS_READ:
            rfile->fab.fab$b_fac = (FAB$M_GET);
            break;

        case RMS_K_ACCESS_WRITE:
            rfile->fab.fab$b_fac = (FAB$M_DEL | FAB$M_GET | FAB$M_PUT);
            break;

        case RMS_K_ACCESS_UPDATE:
            rfile->fab.fab$b_fac = (FAB$M_DEL | FAB$M_GET | FAB$M_PUT | 
                                    FAB$M_UPD);
            break;

        default:
            free(rfile);
            fprintf(stderr, "rms_open() - invalid access mode \"%d\"", access_mode);
            return(NULL);

    }

    switch (allow_mode) {
        case RMS_K_ALLOW_NONE:
            rfile->fab.fab$b_shr = 0;
            break;

        case RMS_K_ALLOW_READ:
            rfile->fab.fab$b_shr = (FAB$V_MSE | FAB$V_SHRGET);
            break;

        case RMS_K_ALLOW_WRITE:
            rfile->fab.fab$b_shr = (FAB$V_MSE | FAB$V_SHRPUT | 
                                    FAB$V_SHRGET | FAB$V_SHRDEL);
            break;

        case RMS_K_ALLOW_UPDATE:
            rfile->fab.fab$b_shr = (FAB$V_MSE | FAB$V_SHRPUT | 
                                    FAB$V_SHRGET | FAB$V_SHRDEL | FAB$V_SHRUPD);
            break;

        default:
            free(rfile);
            fprintf(stderr, "rms_open() - invalid allow mode \"%d\"", allow_mode);
            return(NULL);

    }

    rfile->nam = cc$rms_nam;
    memcpy(&rfile->nam.nam$t_dvi, &fid.t_dvi, 16);
    memcpy(&rfile->nam.nam$w_fid, &fid.w_fid, 6);
    memcpy(&rfile->nam.nam$w_did, &fid.w_did, 6);
    rfile->fab.fab$l_nam = &rfile->nam;        /* use NAM to open by FID */
    rfile->fab.fab$l_fop |= FAB$M_NAM;
    rfile->fab.fab$l_xab = (char *)&(rfile->dat);
    rfile->dat.xab$l_nxt = (char *)&(rfile->fhc);
    rfile->fhc.xab$l_nxt = (char *)&(rfile->pro);

    rfile->status.rc = sys$open(&(rfile->fab), 0, 0);

    if (rfile->status.status_parts.severity != STS$K_SUCCESS) {

        free(rfile);
        return(NULL);

    }

    rfile->rab.rab$l_fab = &(rfile->fab);

    rfile->status.rc = sys$connect(&(rfile->rab), 0, 0);

    if (rfile->status.status_parts.severity != STS$K_SUCCESS) {

        free(rfile);
        return(NULL);

    }

    rfile->index = -1;

    return(rfile);

}

/**/

int rms_close(RFILE *rfile) {
/*
 * Function: rms_close.c
 * Version : 1.0
 * Created : 21-Dec-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will close the file stream to the RMS file and 
 *    deallocate the structures used.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    if (rfile != NULL) {

        rfile->fab.fab$l_xab = NULL;
        rfile->status.rc = sys$close(&(rfile->fab));
        if (rfile->status.status_parts.severity != STS$K_SUCCESS) return EOF;

        free(rfile);

    }

    return(0);

}

/**/

int rms_error(RFILE *rfile) {
/*
 * Function: rms_error.c
 * Version : 1.0
 * Created : 21-Dec-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Check status of last rfile call and signal any errors.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    if (rfile->status.status_parts.severity == STS$K_SUCCESS) {

        return 0;

    } else {

        lib$signal(rfile->status.rc);
        return(rfile->status.rc);

    }

}

/**/

int rms_getidx(RFILE *rfile, void *ptr, size_t size_of_item,
               int index, void *key, int key_size, size_t *size_read) {
/*
 * Function: rms_getidx.c
 * Version : 1.0
 * Created : 21-Dec-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Read keyed record from indexed file, locking for update.
 *
 * Modification History
 *
 * Variables Used
 */

    int i;

/*
 * Main part of function.
 */

    *size_read = 0;

    if (key_size == 0) {

        rfile->rab.rab$b_rac = RAB$C_SEQ;
        rfile->rab.rab$l_rop = RAB$M_WAT;

    } else {

        rfile->rab.rab$b_rac = RAB$C_KEY;
        rfile->rab.rab$l_rop = (RAB$M_WAT | RAB$M_RLK);
        rfile->rab.rab$l_kbf = key;
        rfile->rab.rab$b_ksz = key_size;

    }

    rfile->rab.rab$l_ubf = ptr;
    rfile->rab.rab$w_usz = size_of_item;
    rfile->rab.rab$b_krf = rfile->index = index;

    rfile->status.rc = sys$get(&(rfile->rab), 0, 0);

    *size_read = rfile->rab.rab$w_rsz;

    return(rfile->status.rc);

}

/**/

int rms_getrel(RFILE *rfile, void *ptr, size_t size_of_item, size_t key, size_t *size_read) {
/*
 * Function: rms_getrel.c
 * Version : 1.0
 * Created : 21-Dec-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Read keyed record from relative file, locking for update.
 *
 * Modification History
 *
 * Variables Used
 */

    int i;

/*
 * Main part of function.
 */

    *size_read = 0;

    rfile->rab.rab$b_rac = RAB$C_KEY;
    rfile->rab.rab$l_rop = (RAB$M_WAT | RAB$M_RLK);
    rfile->rab.rab$l_kbf = (char *)&key;
    rfile->rab.rab$b_ksz = 4;
    rfile->rab.rab$l_ubf = ptr;
    rfile->rab.rab$w_usz = size_of_item;

    rfile->status.rc = sys$get(&(rfile->rab), 0, 0);

    *size_read = rfile->rab.rab$w_rsz;

    return(rfile->status.rc);

}

/**/

int rms_getseq(RFILE *rfile, void *ptr, size_t size_of_item, size_t *size_read) {
/*
 * Function: rms_getseq.c
 * Version : 1.0
 * Created : 21-Dec-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Read the next record from a file, locking for update.
 *
 * Modification History
 *
 * Variables Used
 */

    int i;

/*
 * Main part of function.
 */

    *size_read = 0;

    rfile->rab.rab$b_rac = RAB$C_SEQ;
    rfile->rab.rab$l_rop = (RAB$M_WAT | RAB$M_RLK);
    rfile->rab.rab$l_ubf = ptr;
    rfile->rab.rab$w_usz = size_of_item;
    rfile->rab.rab$l_rhb = (char *)NULL;

    if ((rfile->fab.fab$b_fsz == sizeof(struct RHB)) && rfile->fab.fab$v_prn) {

        rfile->rab.rab$l_rhb = (char *)&rfile->rhb;

    }

    rfile->status.rc = sys$get(&(rfile->rab), 0, 0);

    *size_read = rfile->rab.rab$w_rsz;

    return(rfile->status.rc);

}

/**/

int rms_put(RFILE *rfile, void *ptr, size_t size_of_item, size_t *size_wrote) {
/*
 * Function: rms_put.c
 * Version : 1.0
 * Created : 21-Dec-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Write a record to a file.
 *
 * Modification History
 *
 * Variables Used
 */

    int file_type = rms_file_org(rfile);

/*
 * Main part of function.
 */

    switch (file_type) {
        case RMS_K_ORG_IDX:
            rfile->rab.rab$b_rac = RAB$C_KEY;
            rfile->rab.rab$l_rop = RAB$M_UIF;
            break;

        case RMS_K_ORG_REL:
            rfile->rab.rab$b_rac = RAB$C_KEY;
            rfile->rab.rab$l_rop = RAB$M_UIF | RAB$M_WAT;
            break;

        case RMS_K_ORG_SEQ:
            rfile->rab.rab$b_rac = RAB$C_SEQ;
            rfile->rab.rab$l_rop = 0;

    }

    *size_wrote = 0;

    rfile->rab.rab$l_rbf = ptr;
    rfile->rab.rab$w_rsz = size_of_item;
    rfile->rab.rab$l_rhb = (char *)NULL;
    rfile->status.rc = sys$put(&(rfile->rab), 0, 0);

    *size_wrote = rfile->rab.rab$w_rsz;

    return(rfile->status.rc);

}

/**/

int rms_read(RFILE *rfile, void *ptr, size_t size_of_item, size_t *size_read) {
/*
 * Function: rms_read.c
 * Version : 1.0
 * Created : 21-Dec-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Read a record from a file (block mode).
 *
 * Modification History
 *
 * Variables Used
 */

    int i;

/*
 * Main part of function.
 */

    *size_read = 0;

    rfile->rab.rab$l_ubf = ptr;
    rfile->rab.rab$w_usz = size_of_item;
    rfile->rab.rab$l_rhb = (char *)NULL;

    rfile->status.rc = sys$read(&(rfile->rab), 0, 0);

    *size_read = rfile->rab.rab$w_rsz;

    return(rfile->status.rc);

}

/**/

int rms_write(RFILE *rfile, void *ptr, size_t size_of_item, size_t *size_wrote) {
/*
 * Function: rms_write.c
 * Version : 1.0
 * Created : 21-Dec-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Write a record to a file (block mode).
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    *size_wrote = 0;

    rfile->rab.rab$l_rbf = ptr;
    rfile->rab.rab$w_rsz = size_of_item;
    rfile->rab.rab$l_rhb = (char *)NULL;

    rfile->status.rc = sys$write(&(rfile->rab), 0, 0);

    *size_wrote = rfile->rab.rab$w_rsz;

    return(rfile->status.rc);

}

/**/

int rms_update(RFILE *rfile, void *ptr, size_t size_of_item) {
/*
 * Function: rms_update.c
 * Version : 1.0
 * Created : 21-Dec-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Update the current record in the file.
 *
 * Modification History
 *
 * Variables Used
 */

    int file_type = rms_file_org(rfile);

/*
 * Main part of function.
 */

    switch (file_type) {
        case RMS_K_ORG_IDX:
        case RMS_K_ORG_REL:
            rfile->rab.rab$b_rac = RAB$C_KEY;
            break;

        case RMS_K_ORG_SEQ:
            rfile->rab.rab$b_rac = RAB$C_SEQ;

    }

    rfile->rab.rab$l_rop = RAB$M_UIF | RAB$M_WAT;
    rfile->rab.rab$l_rbf = ptr;
    rfile->rab.rab$w_rsz = size_of_item;
    rfile->rab.rab$l_rhb = (char *)NULL;
    rfile->status.rc = sys$update(&(rfile->rab), 0, 0);

    return(rfile->status.rc);

}

/**/

int rms_delete(RFILE *rfile) {
/*
 * Function: rms_delete.c
 * Version : 1.0
 * Created : 21-Dec-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Delete the current record from the file.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    rfile->rab.rab$l_rop = 0;
    rfile->status.rc = sys$delete(&(rfile->rab), 0, 0);

    return(rfile->status.rc);

}

/**/

int rms_rewind(RFILE *rfile) {
/*
 * Function: rms_rewind.c
 * Version : 1.0
 * Created : 21-Dec-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Reposition to the beginning of the file.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    rfile->rab.rab$l_rop = 0;
    rfile->status.rc = sys$rewind(&(rfile->rab), 0, 0);

    return(rfile->status.rc);

}

/**/

int rms_flush(RFILE *rfile) {
/*
 * Function: rms_flush.c
 * Version : 1.0
 * Created : 16-Oct-2002
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Flush the RMS buffers to disk.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    rfile->rab.rab$l_rop = 0;
    rfile->status.rc = sys$flush(&(rfile->rab), 0, 0);

    return(rfile->status.rc);

}

/**/

int rms_file_rat(RFILE *rfile) {
/*
 * Function: rms_file_rat.c
 * Version : 1.0
 * Created : 21-Dec-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Return record attributes. 
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    if (rfile->fab.fab$b_rat & FAB$M_FTN) {

        return(RMS_K_RAT_FTN);

    } else if (rfile->fab.fab$b_rat & FAB$M_PRN) {

        return(RMS_K_RAT_PRN);

    }

    return(RMS_K_RAT_CR);

}

/**/

int rms_file_org(RFILE *rfile) {
/*
 * Function: rms_file_org.c
 * Version : 1.0
 * Created : 21-Dec-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Return the file organization type.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    if (rfile->fab.fab$b_org == FAB$C_IDX) {

        return(RMS_K_ORG_IDX);

    } else if (rfile->fab.fab$b_org == FAB$C_REL) {

        return(RMS_K_ORG_REL);

    }

    return(RMS_K_ORG_SEQ);

}

/**/

/****************************************************************************/
/* Return file attributes. 
 */
int rms_file_rfm(RFILE *rfile) {
/*
 * Function: rms_file_rfm.c
 * Version : 1.0
 * Created : 21-Dec-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Return the record format.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    if (rfile->fab.fab$b_rfm == FAB$C_FIX) {

        return(RMS_K_RFM_FIX);

    } else if (rfile->fab.fab$b_rfm == FAB$C_STM) {

        return(RMS_K_RFM_STM);

    } else if (rfile->fab.fab$b_rfm == FAB$C_STMCR) {

        return(RMS_K_RFM_STMCR);

    } else if (rfile->fab.fab$b_rfm == FAB$C_STMLF) {

        return(RMS_K_RFM_STMLF);

    } else if (rfile->fab.fab$b_rfm == FAB$C_UDF) {

        return(RMS_K_RFM_UDF);

    } else if (rfile->fab.fab$b_rfm == FAB$C_VFC) {

        return(RMS_K_RFM_VFC);

    }

    return(RMS_K_RFM_VAR);

}

/**/

void rms_get_rhb(RFILE *rfile, rhb *rhb) {
/*
 * Function: rms_get_rhb.c
 * Version : 1.0
 * Created : 21-Dec-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Return the record header bytes. Only applicable to VFC and print file
 *    record formats.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    rhb->prefix  = rfile->rhb.prefix;
    rhb->postfix = rfile->rhb.postfix;

}

/**/

int rms_file_size_bytes(RFILE *rfile) {
/*
 * Function: rms_file_size_bytes.c
 * Version : 1.0
 * Created : 21-Dec-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Return size of the file in bytes.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    return(((rfile->fhc.xab$l_ebk - 1) * 512) + rfile->fhc.xab$w_ffb);

}

/**/

int rms_file_size_blocks(RFILE *rfile) {
/*
 * Function: rms_file_size_blocks.c
 * Version : 1.0
 * Created : 21-Dec-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Return size of the file in bytes.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    return(rfile->fhc.xab$l_ebk);

}


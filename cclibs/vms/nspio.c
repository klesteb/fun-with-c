
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

#include <descrip.h>
#include <starlet.h>
#include <lib$routines.h>
#include <ssdef.h>
#include <iodef.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vms.h"
#include "nspio.h"

/*----------------------------------------------------------------------*/

extern int stricmp(const char *, const char *);
static int write_ast(IFILE *fd);
static int timeout_ast(IFILE *fd);

/*----------------------------------------------------------------------*/

IFILE *nsp_open(char *nodename, char *username, char *password, 
                char *object, int rec_size) {
/*
 * Function: nsp_open.c
 * Version : 1.0
 * Created : 01-Oct-2001
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This funcion will open a DECnet connection to the specified object.
 *
 * Modification History
 *
 * Variables Used
 */

    long stat;
    IFILE *fd = NULL;
    char *mask1 = "%s::\"%s\"";
    char *mask2 = "%s\"%s %s\"::\"%s\"";
    char net_object[255];
    struct dsc$descriptor net_object_dsc;

/*
 * Main part of function.
 */

    if ((fd = calloc(1, sizeof(IFILE))) != NULL) {

        fd->ef = vms_get_ef();
        fd->qued = FALSE;
        fd->bufsiz = rec_size;

        if ((fd->buffer = malloc(rec_size + 1)) == NULL) {

            free(fd);
            return(NULL);

        }

        if (stricmp(nodename, "SYS$NET") == 0) {

            sprintf(net_object, "%s", nodename);

        } else {

            if (strlen(username) == 0) {

                sprintf(net_object, mask1, nodename, object);

            } else {

                sprintf(net_object, mask2, nodename, username, password, object);

            }

        }

        SET_SDESC(net_object_dsc, net_object, strlen(net_object));

        stat = sys$assign(&net_object_dsc, &fd->lun, 0,0);
        vms_check_status(stat);

    }

    return(fd);

}

/**/

void nsp_close(IFILE *fd) {
/*
 * Function: nsp_close.c
 * Version : 1.0
 * Created : 01-Oct-2001
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This sub program will close the network channel and release the lun.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;

/*
 * Main part of function.
 */

    if (fd != NULL) {

        stat = sys$dassgn(fd->lun);
        vms_check_status(stat);

        vms_free_ef(fd->ef);

        free(fd); 

    }

}

/**/

int nsp_read(IFILE *fd, void *buffer, int length) {
/*
 * Function: nsp_read.c
 * Version : 1.0
 * Created : 01-Oct-2001
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Read a buffer from the network. Return the number of bytes actually
 *    read. If ss$_linkabort or ss$_linkdiscon or ss$_linkexit error should
 *    occur, then return a -1. All other errors will cause lib$stop() to be
 *    called.
 *
 * Modification History
 *
 * Variables Used
 */

    ULONG func;
    int stat;
    int cnt = -1;

/*
 * Main part of function.
 */

    if (fd->qued == FALSE) {

        func = (IO$_READVBLK);

        stat = sys$qiow(0, fd->lun, func, &fd->iosb.quad, 0,0, buffer, length, 0,0,0,0);
        if (((stat != SS$_LINKABORT) && 
             (stat != SS$_LINKDISCON) &&
             (stat != SS$_LINKEXIT)) && 
            ((fd->iosb.io.stat != SS$_LINKABORT) && 
             (fd->iosb.io.stat != SS$_LINKDISCON) &&
             (fd->iosb.io.stat != SS$_LINKEXIT))) {

            vms_check_status(stat);
            vms_check_status(fd->iosb.io.stat);
            cnt = fd->iosb.io.byte_count;

        }

    } else {

        fd->qued = FALSE;

        if ((fd->iosb.io.stat != SS$_LINKABORT) && 
            (fd->iosb.io.stat != SS$_LINKDISCON) &&
            (fd->iosb.io.stat != SS$_LINKEXIT)) {

            vms_check_status(fd->iosb.io.stat);
            cnt = fd->iosb.io.byte_count;
            memcpy(buffer, &fd->buffer, cnt);

        } 

    }

    return(cnt);

}

/**/

int nsp_write(IFILE *fd, void *buffer, int length, int timeout) {
/*
 * Function: nsp_write.c
 * Version : 1.1
 * Created : 01-Oct-2001
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Write a buffer out to the network. Return the number of bytes actually
 *    written. If ss$_linkabort or ss$_linkdiscon or ss$_linkexit errors
 *    should occur, then return -1. All other errors will call lib$stop().
 *
 * Modification History
 *
 *    01-Jun-2005 K.Esteb
 *        Added a timeout capability to this function so that network
 *        writes will exit gracefully when something happens on the
 *        otherside.
 *
 * Variables Used
 */

    ULONG func;
    int stat;
    int cnt = -1;
    char mask[] = "0 %02.d:%02.d:%02.d.00";
    char timbuf[24];
    long timadr[2];
    long hours, minutes, seconds, junk1;
    struct dsc$descriptor timdsc;

/*
 * Main part of function.
 */

    fd->timer_ef = vms_get_ef();

    if (timeout > 0) {

        hours = timeout / 3600;
        junk1 = (timeout % 3600);
        minutes = junk1 / 60;
        seconds = junk1 % 60;
    
        sprintf(timbuf, mask, hours, minutes, seconds);
        SET_SDESC(timdsc, timbuf, strlen(timbuf));

        stat = sys$bintim(&timdsc, &timadr);
        vms_check_status(stat);

        stat = sys$setimr(fd->timer_ef, timadr, &timeout_ast, fd, 0);
        vms_check_status(stat);

    }

    func = (IO$_WRITEVBLK);

    stat = sys$qio(0, fd->lun, func, &fd->iosb.quad, &write_ast, fd, buffer, length, 0,0,0,0);
    if ((stat != SS$_LINKABORT) && 
        (stat != SS$_LINKDISCON) &&
        (stat != SS$_LINKEXIT)) {

        vms_check_status(stat);

    }

    stat = sys$hiber();
    vms_check_status(stat);

    vms_free_ef(fd->timer_ef);

    if ((fd->iosb.io.stat != SS$_LINKABORT) && 
        (fd->iosb.io.stat != SS$_LINKDISCON) &&
        (fd->iosb.io.stat != SS$_LINKEXIT)) {

        vms_check_status(fd->iosb.io.stat);

    }

    cnt = fd->iosb.io.byte_count;

    return(cnt);

}

/**/

static int timeout_ast(IFILE *fd) {
/*
 * Timeout AST - cancel any pending I/O and wake the main process up.
 */

    int stat;

    stat = sys$cancel(fd->lun);
    vms_check_status(stat);

    stat = sys$wake(0,0);
    vms_check_status(stat);

    return(0);

}

static int write_ast(IFILE *fd) {
/*
 * Write AST - Cancel the timer, synch with the qio and wake 
 * the main process up.
 */

    int stat;

    stat = sys$cantim(fd->timer_ef, 0);
    vms_check_status(stat);

    stat = sys$synch(fd->ef, &fd->iosb.quad);
    vms_check_status(stat);

    stat = sys$wake(0,0);
    vms_check_status(stat);

    return(0);

}


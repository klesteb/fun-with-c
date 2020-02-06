
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 1992 by Kevin L. Esteb                                     */
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
#include <iodef.h>
#include <ttdef.h>
#include <ssdef.h>
#include <descrip.h>
#include <starlet.h>
#include <lib$routines.h>

#include "vms.h"
#include "latio.h"

#ifndef IO$M_LT_CONNECT
/*
 * define the next three here, if they don't appear in IODEF
 */
#define IO$M_LT_CONNECT     64
#define IO$M_LT_DISCON     128
#define IO$_TTY_PORT        41

#endif

/*-Prototypes---------------------------------------------------------------*/

static int ctrly_ast(int);

/*--------------------------------------------------------------------------*/

latio_t *lat_open(char *lta_device) {
/*
 * File: lat_open.c
 * Date: 30-Jul-1992
 * By  : Kevin Esteb
 *
 * Open a channel to the lat port. Also assign a CTRL/Y ast to channel.
 *
 */

    latio_t *lp;
    unsigned int status;                    /* system service status        */
    struct dsc$descriptor desc;             /* descriptor for lat port name */
    mode imode;                             /* old terminal characteristics */

/*
 * Main part of function.
 */

    if ((lp = malloc(sizeof(latio_t))) != NULL) {

        /* build our string descriptor */

		SET_SDESC(desc, lta_device, strlen(lta_device));

        /* assign an I/O channel to the LTA device */

        status = sys$assign(&desc, &lp->channel, 0, 0);
        vms_check_status(status);

        /* connect to LTA device */

        status = sys$qiow(0, lp->channel, (IO$_TTY_PORT | IO$M_LT_CONNECT),
                          &lp->iosb, 0, 0, 0, 0, 0, 0, 0, 0);
        vms_check_status(status);
        vms_check_status(lp->iosb.lat.stat);

        /* set up the CTRL/Y AST to detect dataset hangups */

        status = sys$qiow(0, lp->channel, (IO$_SETMODE | IO$M_CTRLYAST),
                          &lp->iosb, 0, 0, &ctrly_ast, 0, 0, 0, 0, 0);
        vms_check_status(status);
        vms_check_status(lp->iosb.lat.stat);

        /* set up the terminal modes that we want */

        status = sys$qiow(0, lp->channel, (IO$_SENSEMODE),
                      &lp->iosb, 0, 0, &lp->mode, 12, 0, 0, 0, 0);
        vms_check_status(status);
        vms_check_status(lp->iosb.lat.stat);

        memmove(&imode, &lp->mode, 12);   /* copy global value to local one*/
        imode.map.basic |= TT$M_HOSTSYNC;   /* needed for flow control       */

        status = sys$qiow(0, lp->channel, (IO$_SETMODE),
                          &lp->iosb, 0, 0, &imode, 12, 0, 0, 0, 0);
        vms_check_status(status);
        vms_check_status(lp->iosb.lat.stat);

        /* return the channel number to caller */

    }

    return (lp);

}

void lat_close(latio_t *lp) {
/*
 * File: lat_close.c
 * Date: 30-Jul-1992
 * By  : Kevin Esteb
 *
 * Close the channel to the lat port.
 *
 */

    unsigned int status;                    /* system service status        */

/*
 * Main part of function.
 */

    if (lp != NULL) {

        /* restore terminal characteristics, use global value 'mode' */

        status = sys$qiow(0, lp->channel, (IO$_SETMODE),
                          &lp->iosb, 0, 0, &lp->mode, 12, 0, 0, 0, 0);
        vms_check_status(status);
        vms_check_status(lp->iosb.lat.stat);

        /* disconnect the LTA device */

        status = sys$qiow(0, lp->channel, (IO$_TTY_PORT | IO$M_LT_DISCON),
                          &lp->iosb, 0, 0, 0, 0, 0, 0, 0, 0);
        vms_check_status(status);
        vms_check_status(lp->iosb.lat.stat);

        /* and deassign the I/O channel */

        status = sys$dassgn(lp->channel);
        vms_check_status(status)

        free(lp);

    }

}

int lat_read(latio_t *lp, char *buffer, UWORD length, UWORD timeout) {
/*
 * File: lat_read.c
 * Date: 30-Jul-1992
 * By  : Kevin Esteb
 *
 * Read a set amount of data from the the lat channel.
 *
 */

    unsigned int status;                    /* system service status        */
    struct dsc$descriptor desc;             /* descriptor for data buffer   */

/*
 * Main part of function.
 */

    /* build our string descriptor */

	SET_SDESC(desc, buffer, length);

    /* perform our read request */

    if (timeout == 0) {

        status = sys$qiow(0, lp->channel, IO$_READVBLK,
                          &lp->iosb, 0, 0, desc.dsc$a_pointer, desc.dsc$w_length, 
                          0, 0, 0, 0);

    } else {

        status = sys$qiow(0, lp->channel, IO$_READVBLK|IO$M_TIMED,
                          &lp->iosb, 0, 0, desc.dsc$a_pointer, desc.dsc$w_length, 
                          timeout, 0, 0, 0);

        if (lp->iosb.lat.stat == SS$_TIMEOUT) return (-1);

    }

    vms_check_status(status);
    vms_check_status(lp->iosb.lat.stat);

    return (lp->iosb.lat.variable_info.byte_count);

}

int lat_write(latio_t *lp, char *buffer, UWORD length) {
/*
 * File: lat_write.c
 * Date: 30-jul-1992
 * By  : Kevin Esteb
 *
 * Write a set amount of data to the lat port.
 *
 */

    unsigned int status;                    /* system service status        */
    struct dsc$descriptor desc;             /* descriptor for data buffer   */

/*
 * Main part of function.
 */

    /* build our string descriptor */

	SET_SDESC(desc, buffer, length);

    /* perform our write request */

    status = sys$qiow(0, lp->channel, IO$_WRITEVBLK, &lp->iosb, 0, 0,
                      desc.dsc$a_pointer, desc.dsc$w_length, 0, 0, 0, 0);
    vms_check_status(status);
    vms_check_status(lp->iosb.lat.stat);

    return (lp->iosb.lat.variable_info.byte_count);

}

static int ctrly_ast(int junk) {

/*    printf("ERROR: error %d has occured\n", junk); */
/*    lib$stop(SS$_HANGUP);*/

    return(0);

}


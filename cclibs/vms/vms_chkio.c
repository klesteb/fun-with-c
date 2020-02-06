
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

#include <lib$routines.h>
#include <starlet.h>

#include "vms.h"

/*----------------------------------------------------------------------*/

int vms_chkio(IFILE *fd) {
/*
 * Function: vms_chkio.c
 * Version : 1.0
 * Created : 23-Feb-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will wait for the completion of asynchronous I/O.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;

/*
 * Main part of function.
 */

    stat = sys$synch(fd->ef, &fd->iosb.quad);
    vms_check_status(stat);

    return(stat);

}


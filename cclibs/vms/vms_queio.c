
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

#include <lib$routines.h>
#include <starlet.h>
#include <ssdef.h>
#include <iodef.h>
#include <string.h>

#include "vms.h"

/*----------------------------------------------------------------------*/

static int read_ast(IFILE *fd) {
/*
 * Function: read_ast.c
 * Version : 1.0
 * Created : 25-Sep-2001
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will wake up the main process and set the 
 *    appropiate event flag for the read I/O.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;

/*
 * Main part of function.
 */

    if (vms_chkio(fd)) {

        vms_set_ef(fd->ef);    

    }

    return(0);

}

/**/

int vms_queio(IFILE *fd) {
/*
 * Function: vms_queio.c
 * Version : 1.0
 * Created : 25-Sep-2001
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will queue a read I/O AST.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;
    ULONG func;

/*
 * Main part of function.
 */

    fd->qued = TRUE;
    func = (IO$_READVBLK);

    stat = sys$qio(0, fd->lun, func, &fd->iosb.quad, &read_ast, fd, 
                   fd->buffer, fd->bufsiz, 0,0,0,0);
    vms_check_status(stat);

    return(stat);

}


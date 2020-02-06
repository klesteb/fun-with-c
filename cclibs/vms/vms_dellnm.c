
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

#include <string.h>
#include <descrip.h>
#include <lib$routines.h>
#include <starlet.h>

#include "vms.h"

/*----------------------------------------------------------------------*/

void vms_dellnm(char *tabname, char *logname, UBYTE accmode) {
/*
 * Function: vms_dellnm.c
 * Version : 1.0
 * Created : 22-Feb-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will delete a logical name from the specified table.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;

    struct dsc$descriptor tabnam_dsc;
    struct dsc$descriptor lognam_dsc;

/*
 * Main part of function.
 */

    SET_SDESC(tabnam_dsc, tabname, strlen(tabname));
    SET_SDESC(lognam_dsc, logname, strlen(logname));

    stat = sys$dellnm(&tabnam_dsc, &lognam_dsc, &accmode);
    vms_check_status(stat);

}


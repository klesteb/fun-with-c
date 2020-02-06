
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
#include <lnmdef.h>
#include <lib$routines.h>
#include <starlet.h>

#include "vms.h"

/*----------------------------------------------------------------------*/

void vms_crelnm(long nattr, char *tabname, char *logname, char *logvalue, 
                UBYTE accmode, long tattr) {
/*
 * Function: vms_crelnm.c
 * Version : 1.0
 * Created : 22-Feb-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will create a logical name in the specified table.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;

    ITEM_LIST_3 items[3];
    struct dsc$descriptor tabnam_dsc;
    struct dsc$descriptor lognam_dsc;

/*
 * Main part of function.
 */

    SET_SDESC(tabnam_dsc, tabname, strlen(tabname));
    SET_SDESC(lognam_dsc, logname, strlen(logname));

    SET_ITEM3(items[0], LNM$_ATTRIBUTES, &tattr, 4, 0);
    SET_ITEM3(items[1], LNM$_STRING, logvalue, strlen(logvalue), 0);
    SET_ITEM3(items[2], 0, 0, 0, 0);

    stat = sys$crelnm(&nattr, &tabnam_dsc, &lognam_dsc, &accmode, &items);
    vms_check_status(stat);

}


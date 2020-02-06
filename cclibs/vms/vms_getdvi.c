
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

#include <ssdef.h>
#include <starlet.h>
#include <string.h>
#include <lib$routines.h>

#include "vms.h"

/*----------------------------------------------------------------------*/

void vms_getdvi(char *logical_name, short lun, int code, char *result) {
/*
 * Function: vms_getdvi.c
 * Version : 1.0
 * Created : 11-May-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will return device information to the calling
 *    program depending on the code that is sent to it.
 *
 * Modification History
 *
 * Variables Used
 */

    short outnam_length;
    int stat;
    IOSB iosb;
    ITEM_LIST_3 getdvi[2];
    char outnam[255];

/*
 * Main part of function.
 */

    SET_ITEM3(getdvi[0], code, outnam, 254, &outnam_length);
    SET_ITEM3(getdvi[1], 0, 0, 0, 0);

    stat = sys$getdviw(0, lun, logical_name, &getdvi, &iosb.quad, 0,0,0);
    vms_check_status(stat);
    vms_check_status(iosb.io.stat);

    strncpy(result, outnam, outnam_length);

}


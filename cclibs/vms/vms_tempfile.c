
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
#include <string.h>
#include <jpidef.h>
#include <starlet.h>
#include <lib$routines.h>

#include "vms.h"

/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/

int vms_tempfile(char *directory, char *filename, int len) {
/*
 * Function: vms_tempfile.c
 * Version : 1.0
 * Created : 25-Feb-2002
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will create a temporay file name.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;
    char buff[256];
    IOSB iosb;
    ULONG pid;
    QUADWORD holder;
    ITEM_LIST_3 items[2];

/*
 * Main part of function.
 */

    stat = sys$gettim(&holder);
    vms_check_status(stat);

    SET_ITEM3(items[0], JPI$_PID, &pid, 4, 0);
    SET_ITEM3(items[1], 0, 0, 0, 0);

    stat = sys$getjpi(0,0,0, items, &iosb.quad, 0,0);
    vms_check_status(stat);
    vms_check_status(iosb.jbc.stat);

    sprintf(buff, "%s%d%u%u.TMP", directory, pid, holder.junk[0], holder.junk[1]);
    if (strlen(buff) > len) {

        stat = -1;

    } else {

        strcpy(filename, buff);
        stat = 0;

    }

    return(stat);

}


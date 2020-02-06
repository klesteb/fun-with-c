
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

#include <string.h>
#include <descrip.h>
#include <starlet.h>
#include <lib$routines.h>

#include "vms.h"

/*----------------------------------------------------------------------*/

void vms_asctim(QUADWORD *date_time, char *buffer, int buflen) {
/*
 * Function: vms_asctim.c
 * Version : 1.0
 * Created : 03-Sep-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function is a wrapper function for the sys$asctim() 
 *    system service call.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;
    UWORD rlen;
    struct dsc$descriptor dtime_dsc;

/*
 * Main part of function.
 */

    SET_SDESC(dtime_dsc, buffer, buflen);

    stat = sys$asctim(&rlen, &dtime_dsc, date_time, 0);
    vms_check_status(stat);

    buffer[rlen] = '\0';

}



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
#include <descrip.h>
#include <starlet.h>
#include <lib$routines.h>

#include "vms.h"

/*----------------------------------------------------------------------*/

char *strupr(char *);

/*----------------------------------------------------------------------*/

void vms_bintim(char *date_time_buf, QUADWORD *date_time) {
/*
 * Function: vms_bintim.c
 * Version : 1.0
 * Created : 29-Nov-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function is a wrapper function for the sys$bintim() 
 *    system service call.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;
    QUADWORD holder;
    struct dsc$descriptor date_time_dsc;

/*
 * Main part of function.
 */

    strupr(date_time_buf);
    SET_SDESC(date_time_dsc, date_time_buf, strlen(date_time_buf));

    stat = sys$bintim(&date_time_dsc, &holder);
    vms_check_status(stat);

    memcpy(date_time, &holder, sizeof(QUADWORD));

}


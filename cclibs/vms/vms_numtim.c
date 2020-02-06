
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

#include <starlet.h>
#include <lib$routines.h>

#include "vms.h"

/*----------------------------------------------------------------------*/

void vms_numtim(QUADWORD *date_time, TIMBUF *timbuf) {
/*
 * Function: vms_bintim.c
 * Version : 1.0
 * Created : 29-Nov-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function is a wrapper function for the sys$numtim() 
 *    system service call.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;

/*
 * Main part of function.
 */

    stat = sys$numtim(timbuf, date_time);
    vms_check_status(stat);

}


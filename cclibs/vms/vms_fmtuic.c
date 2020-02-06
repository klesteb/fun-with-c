
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

void vms_fmtuic(long uic, char *buffer, int buflen) {
/*
 * Function: vms_fmtuic.c
 * Version : 1.0
 * Created : 03-Dec-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Format a UIC using sys$faol().
 *
 * Modification History
 *
 * Variables Used
 */

	int stat;
    UWORD rlen;
    char *fmt = "!%I";
    struct dsc$descriptor fmt_dsc;
    struct dsc$descriptor uic_dsc;

/*
 * Main part of function.
 */

    SET_SDESC(fmt_dsc, fmt, strlen(fmt));
    SET_SDESC(uic_dsc, buffer, buflen);

    stat = sys$faol(&fmt_dsc, &rlen, &uic_dsc, &uic);
    vms_check_status(stat);

    buffer[rlen] = '\0';

}


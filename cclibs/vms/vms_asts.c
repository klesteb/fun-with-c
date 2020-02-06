
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 1998 by Kevin L. Esteb                                     */
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

void vms_disable_asts(void) {
/*
 * Function: vms_disable_asts.c
 * Author  : Kevin Esteb
 * Version : 1.0
 * Date    : 11-Mar-1998
 *
 * Synopsis
 *
 *    This routine will temporaily disable ast delivery.
 *
 * Modifications
 *
 * Variables
 */

    int stat;

/*
 * Main part of function.
 */

    stat = sys$setast(0);
    vms_check_status(stat);

}

/**/

void vms_enable_asts(void) {
/*
 * Function: enable_asts.c
 * Author  : Kevin Esteb
 * Version : 1.0
 * Date    : 11-Mar-1998
 *
 * Synopsis
 *
 *    This routine will enable ast delivery.
 *
 * Modifications
 *
 * Variables
 */

    int stat;

/*
 * Main part of function.
 */

    stat = sys$setast(1);
    vms_check_status(stat);

}


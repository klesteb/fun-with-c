
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

#include <ssdef.h>
#include <lnmdef.h>
#include <descrip.h>
#include <starlet.h>
#include <lib$routines.h>
#include <string.h>

#include "vms.h"

/*----------------------------------------------------------------------*/

int vms_chklnm(char *tabname, char *logname, long accmode) {
/*
 * Function: vms_chklnm.c
 * Author  : Kevin Esteb
 * Version : 1.0
 * Date    : 21-Feb-2000
 *
 * Synopsis
 *
 *    This function will check to see if a logical name exists within a logical
 *    name table at the specified access mode. The access mode is specified in
 *    psldef.h
 *
 * Modifications
 *
 * Variables
 */

    int stat;
    long attr;
    struct dsc$descriptor tabnam_dsc;
    struct dsc$descriptor lognam_dsc;

/*
 * Main part of function.
 */

    SET_SDESC(tabnam_dsc, tabname, strlen(tabname));
    SET_SDESC(lognam_dsc, logname, strlen(logname));

    attr = LNM$M_CASE_BLIND;

    stat = sys$trnlnm(&attr, &tabnam_dsc, &lognam_dsc, &accmode, 0);
    if (stat == SS$_NOLOGNAM) {

        return(FALSE);

    } else vms_check_status(stat);

    return(TRUE);

}


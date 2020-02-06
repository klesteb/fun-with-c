
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
#include <rmsdef.h>
#include <descrip.h>
#include <lib$routines.h>

#include "vms.h"

/*----------------------------------------------------------------------*/

int vms_file_exists(char *file_spec) {
/*
 * Function: vms_file_exists.c
 * Version : 1.0
 * Created : 02-Sep-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will use lib$find_file() to see if a file exits. If 
 *    it does then return true, otherwise return false.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;
    int ctx = 0;
    int estat = TRUE;
    char result[256];
    struct dsc$descriptor file_dsc;
    struct dsc$descriptor result_dsc;

/*
 * Main part of function.
 */

    SET_SDESC(file_dsc, file_spec, strlen(file_spec));
    SET_SDESC(result_dsc, result, sizeof(result));

    stat = lib$find_file(&file_dsc, &result_dsc, &ctx, 0,0,0,0);
    if (stat != RMS$_NORMAL) estat = FALSE;

    stat = lib$find_file_end(&ctx);
    vms_check_status(stat);

    return(estat);

}


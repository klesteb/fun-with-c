
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

#include <ssdef.h>
#include <lnmdef.h>
#include <descrip.h>
#include <starlet.h>
#include <lib$routines.h>
#include <string.h>

#include "vms.h"

/*----------------------------------------------------------------------*/

void vms_trnlnm(char *table_name, char *logical_name, char *default_name, 
                long code, char *outname) {
/*
 * Function: vms_trnlnm.c
 * Author  : Kevin Esteb
 * Version : 1.0
 * Date    : 17-Mar-1998
 *
 * Synopsis
 *
 *    This function will take a logical name and look in the specified
 *    logical name table for the equivalance name. Table_name is the logical
 *    name table to look in. logical_name is the name to act upon, code is
 *    $lmndef code defined in the $lmndef macro, outname is the result
 *    of the operation. 
 *
 * Modifications
 *
 * Variables
 */

    int stat;
    short outname_length = 0;
    long attr;
    ITEM_LIST_3 item[2];
    struct dsc$descriptor table_name_desc;
    struct dsc$descriptor logical_name_desc;

/*
 * Main part of function.
 */

    SET_SDESC(logical_name_desc, logical_name, strlen(logical_name));
    SET_SDESC(table_name_desc, table_name, strlen(table_name));

	SET_ITEM3(item[0], code, outname, 255, &outname_length);
    SET_ITEM3(item[1], 0, 0, 0, 0);

    attr = LNM$M_CASE_BLIND;

    stat = sys$trnlnm(&attr, &table_name_desc, &logical_name_desc, 0, &item);
    if (stat == SS$_NOLOGNAM) {

        strcpy(outname, default_name);
        return;

    } else vms_check_status(stat);

    outname[outname_length] = '\0';

}



/*---------------------------------------------------------------------------*/
/*  Copyright (c) 2002 by Kevin L. Esteb                                     */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include <ssdef.h>
#include <dvidef.h>
#include <starlet.h>
#include <string.h>
#include <descrip.h>
#include <lib$routines.h>

#include "vms.h"

/*----------------------------------------------------------------------*/

long vms_freespace(char *device) {
/*
 * Function: vms_freespace.c
 * Version : 1.0
 * Created : 08-Apr-2002
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will return the free space in blocks on the device.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;
    IOSB iosb;
    long blocks;
    ITEM_LIST_3 getdvi[2];
    struct dsc$descriptor device_dsc;

/*
 * Main part of function.
 */

    SET_SDESC(device_dsc, device, strlen(device));
    SET_ITEM3(getdvi[0], DVI$_FREEBLOCKS, &blocks, 4, 0);
    SET_ITEM3(getdvi[1], 0, 0, 0, 0);

    stat = sys$getdviw(0,0, &device_dsc, &getdvi, &iosb.quad, 0,0,0);
    vms_check_status(stat);
    vms_check_status(iosb.io.stat);

    return(blocks);

}


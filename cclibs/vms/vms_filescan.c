
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
#include <ssdef.h>
#include <fscndef.h>
#include <descrip.h>
#include <starlet.h>
#include <lib$routines.h>

#include "vms.h"

/*----------------------------------------------------------------------*/

extern int strnicmp(const char *, const char *, int);

/*----------------------------------------------------------------------*/

int vms_filescan(char *filename, char *action, char *result) {
/*
 * Function: vms_filescan.c
 * Version : 1.0
 * Created : 02-Sep-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function is an interface routine to the sys$filescan system service.
 *    It allowes the calling program to parse a filename in a predicatable
 *    manner. Its action is similar to the F$PARSE dcl lexical. The following
 *    actions have meaning.
 *
 *        Actions:
 *
 *            node      - returns the node name.
 *            device    - returns the device name.
 *            directory - returns the directory name.
 *            name      - returns the files name.
 *            type      - returns the file type.
 *            version   - returns the files version.
 *
 *        ex.
 *            vms_filescan("test.file", "name", result)
 *
 *    An error will result when you ask for the node name in the above 
 *    example. Since none is supplied with the file name, none is returned. 
 *    This routine has the same restrictions as sys$filescan. 
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;
    int rlen;
    int offset;
    char *ptr1;
    char *ptr2;
    ITEM_LIST_2 itmlst[2];
    struct dsc$descriptor filename_dsc;

/*
 * Main part of function.
 */

    SET_SDESC(filename_dsc, filename, strlen(filename));

    if (strnicmp(action, "MOD", 3) == 0) {

        SET_ITEM2(itmlst[0], FSCN$_NODE, 0, 0);
                        
    } else if (strnicmp(action, "DEV", 3) == 0) {

        SET_ITEM2(itmlst[0], FSCN$_DEVICE, 0, 0);
                
    } else if (strnicmp(action, "DIR", 3) == 0) {

        SET_ITEM2(itmlst[0], FSCN$_DIRECTORY, 0, 0);

    } else if (strnicmp(action, "NAM", 3) == 0) {

        SET_ITEM2(itmlst[0], FSCN$_NAME, 0, 0);

    } else if (strnicmp(action, "TYP", 3) == 0) {

        SET_ITEM2(itmlst[0], FSCN$_TYPE, 0, 0);

    } else if (strnicmp(action, "VER", 3) == 0) {

        SET_ITEM2(itmlst[0], FSCN$_VERSION, 0, 0);

    } else return(FALSE);

    SET_ITEM2(itmlst[1], 0, 0, 0);
 
    stat = sys$filescan(&filename_dsc, &itmlst, 0,0,0);
    vms_check_status(stat);

    if ((itmlst[0].comp_length != 0) && (itmlst[0].comp_address != 0)) {

        /* Convert the returned item_list_2 value into a pointer. Get   */
        /* the address of filename and convert it into a pointer.       */
        /* Subtract the two to come up with the offset within filename. */
        /* Once that is found, then do a simple strncpy() to move the   */
        /* desired chunk of filename into result.                       */

        ptr1 = (char *)itmlst[0].comp_address;
        ptr2 = (char *)&(*filename);            /* Cute! isn't C fun!!! */
        offset = ptr1 - ptr2;
        rlen = itmlst[0].comp_length;
        strncpy(result, &filename[offset], rlen);

        /* Why, oh why, couldn't DEC have used a item_list_3 with this  */
        /* routine! It would have made everything so much easier! I     */
        /* thing this is the only system service that uses an           */
        /* item_list_2 to return results to the caller...               */

    } else return(FALSE);

    return(TRUE);

}



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
#include <sjcdef.h>
#include <jbcmsgdef.h>
#include <starlet.h>
#include <lib$routines.h>

#include "vms.h"

/*----------------------------------------------------------------------*/

int vms_print(char *filename, char *queuename, char *formname, long copies) {
/*
 * Function: vms_print.c
 * Version : 1.0
 * Created : 12-May-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This routine queues a file to a line printer spooler.
 *
 *        vms_print(filename, quename, formname, copies)
 *
 *    A negative number of copies will delete the file_name after
 *    printing. 
 *
 *        ex.
 *            entry = vms_print("lpque.bas","simon_lpa1","default",0)
 *            entry = vms_print("lpque.bas","simon_lpa1,"",0)
 *            entry = vms_print("lpque.bas,"","",0)
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;
    int del_flag;
    int entrynum;
    IOSB iosb;
    ITEM_LIST_3 items[6];
    char *sysprint = "SYS$PRINT";
    char *deform = "DEFAULT";

/*
 * Main part of function.
 */

    if (strlen(queuename) == 0) {

        SET_ITEM3(items[0], SJC$_QUEUE, sysprint, strlen(sysprint), 0);

    } else SET_ITEM3(items[0], SJC$_QUEUE, queuename, strlen(queuename), 0);

    SET_ITEM3(items[1], SJC$_FILE_SPECIFICATION, filename, strlen(filename), 0);

    if (strlen(formname) == 0) {

        SET_ITEM3(items[2], SJC$_FORM_NAME, deform, strlen(deform), 0);

    } else SET_ITEM3(items[2], SJC$_FORM_NAME, formname, strlen(formname), 0);

    if (copies < 0) {

        copies = 0 - copies;
        del_flag = TRUE;

    } else if (copies == 0) {

        copies = 1;
        del_flag = FALSE;

    } else {

        del_flag = FALSE;

    }

    SET_ITEM3(items[3], SJC$_FILE_COPIES, &copies, 4, 0);
    SET_ITEM3(items[4], SJC$_ENTRY_NUMBER_OUTPUT, &entrynum, 4, 0);

    if (del_flag) {

        SET_ITEM3(items[5], SJC$_DELETE_FILE, 0, 0, 0);

    } else SET_ITEM3(items[5], SJC$_NO_DELETE_FILE, 0, 0, 0);

    SET_ITEM3(items[6], 0, 0, 0, 0);

    stat = sys$sndjbcw(0, SJC$_ENTER_FILE, 0, &items, &iosb.quad, 0,0);
    vms_check_status(stat);
    vms_check_status(iosb.jbc.stat);

    return(entrynum);

}


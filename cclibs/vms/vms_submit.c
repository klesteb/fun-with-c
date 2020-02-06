
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

int vms_submit(char *filename, char *queuename, submit_t *sub) {
/*
 * Function: vms_submit.c
 * Version : 1.0
 * Created : 19-Nov-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will take passed file name and submit it to the 
 *    designated batch queue. They are optional parmeters that can be 
 *    passed to control how the file is to be submitted. 
 *
 *    ex.
 *        entry = vms_submit("login.com", "", NULL);
 *
 * Modification History
 *
 * Variables Used
 */

    int x = 3;
    int stat;
    int entrynum;
    int del_flag;
    IOSB iosb;
    ITEM_LIST_3 items[20];
    QUADWORD date_time;
    char *sysbatch = "SYS$BATCH";

/*
 * Main part of function.
 */

    if (strlen(queuename) == 0) {

        SET_ITEM3(items[0], SJC$_QUEUE, sysbatch, strlen(sysbatch), 0);

    } else SET_ITEM3(items[0], SJC$_QUEUE, queuename, strlen(queuename), 0);

    SET_ITEM3(items[1], SJC$_FILE_SPECIFICATION, filename, strlen(filename), 0);
    SET_ITEM3(items[2], SJC$_ENTRY_NUMBER_OUTPUT, &entrynum, 4, 0);

    if (sub != NULL) {

        if (sub->notify_sw == TRUE) {

            x++;
            SET_ITEM3(items[x], SJC$_NOTIFY, 0, 0, 0);

        }

        if (sub->logname != NULL) {

            x++;
            SET_ITEM3(items[x], SJC$_LOG_SPECIFICATION, sub->logname, strlen(sub->logname), 0);

        }

        if (sub->after != NULL) {

            x++;
            stat = sys$bintim(sub->after, &date_time);
            vms_check_status(stat);
            SET_ITEM3(items[x], SJC$_AFTER_TIME, &date_time, sizeof(QUADWORD), 0);

        }

        if (sub->hold_sw == TRUE) {

            x++;
            SET_ITEM3(items[x], SJC$_HOLD, 0, 0, 0);

        }

        if (sub->keep_sw == FALSE) {

            x++;
            SET_ITEM3(items[x], SJC$_LOG_DELETE, 0, 0, 0);

        }

        if (sub->del_file_sw == TRUE) {

            x++;
            SET_ITEM3(items[x], SJC$_DELETE_FILE, 0, 0, 0);

        }

        if (sub->log_print_sw == TRUE) {

            x++;
            SET_ITEM3(items[x], SJC$_LOG_QUEUE, sub->logqueue, strlen(sub->logqueue), 0);

        } else {

            x++;
            SET_ITEM3(items[x], SJC$_NO_LOG_SPOOL, 0, 0, 0);

        } 

        if (sub->username != NULL) {

            x++;
            SET_ITEM3(items[x], SJC$_USERNAME, sub->username, strlen(sub->username), 0);

        } 

        if (sub->p1 != NULL ) {

            x++;
            SET_ITEM3(items[x], SJC$_PARAMETER_1, sub->p1, strlen(sub->p1), 0);

        } 

        if (sub->p2 != NULL ) {

            x++;
            SET_ITEM3(items[x], SJC$_PARAMETER_2, sub->p2, strlen(sub->p2), 0);

        } 

        if (sub->p3 != NULL ) {

            x++;
            SET_ITEM3(items[x], SJC$_PARAMETER_3, sub->p3, strlen(sub->p3), 0);

        } 

        if (sub->p4 != NULL ) {

            x++;
            SET_ITEM3(items[x], SJC$_PARAMETER_4, sub->p4, strlen(sub->p4), 0);

        } 

        if (sub->p5 != NULL ) {

            x++;
            SET_ITEM3(items[x], SJC$_PARAMETER_5, sub->p5, strlen(sub->p5), 0);

        } 

        if (sub->p6 != NULL ) {

            x++;
            SET_ITEM3(items[x], SJC$_PARAMETER_6, sub->p6, strlen(sub->p6), 0);

        } 

        if (sub->p7 != NULL ) {

            x++;
            SET_ITEM3(items[x], SJC$_PARAMETER_7, sub->p7, strlen(sub->p7), 0);

        } 

        if (sub->p8 != NULL ) {

            x++;
            SET_ITEM3(items[x], SJC$_PARAMETER_8, sub->p8, strlen(sub->p8), 0);

        } 

    }

    x++;
    SET_ITEM3(items[x], 0, 0, 0, 0);

    stat = sys$sndjbcw(0, SJC$_ENTER_FILE, 0, &items, &iosb.quad, 0,0);
    vms_check_status(stat);
    vms_check_status(iosb.jbc.stat);

    return(entrynum);

}


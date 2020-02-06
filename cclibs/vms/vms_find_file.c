
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ssdef.h>
#include <rmsdef.h>
#include <descrip.h>
#include <lib$routines.h>

#include "vms.h"
#include "que_util.h"

/*--------------------------------------------------------------------------*/

void vms_find_file(char *filter, void *qresults) {
/*
 * Function: vms_find_file.c
 * Version : 1.0
 * Created : 31-Aug-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will do a scan of the current directory using "filter".
 *    The results will be returned in the linked list "results".
 *
 * Modification History
 *
 * Variables Used
 */

    int x;
    int stat;
    ULONG ctx = 0;
    char *buff;
    char file_result[256];
    struct dsc$descriptor result_dx;
    struct dsc$descriptor filter_dx;
	queue *results;

/*
 * Main part of function.
 */

	results = qresults;

    SET_SDESC(filter_dx, filter, strlen(filter));
    SET_SDESC(result_dx, file_result, sizeof(file_result));

    /* Locate the file which matches the given filter criteria */

    for (;;) {

        stat = lib$find_file(&filter_dx, &result_dx, &ctx);
        if (stat == RMS$_NORMAL) {

            for (x = 0; x < 256; x++)
                if (file_result[x] == ' ') break;

            if ((buff = malloc(x + 1)) != NULL) {

                strncpy(buff, file_result, x);
                que_push_tail(results, buff);

            }

        } else if (stat == RMS$_NMF) {

            stat = lib$find_file_end(&ctx);
            vms_check_status(stat);
            break;

        } else vms_check_status(stat);

    }

}


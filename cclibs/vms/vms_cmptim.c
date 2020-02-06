
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

#include <lib$routines.h>

#include "vms.h"

/*----------------------------------------------------------------------*/

int vms_cmptim(QUADWORD *q1, QUADWORD *q2) {
/*
 * Function: vms_cmptim.c
 * Version : 1.1
 * Created : 02-Dec-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will compare two VMS datetime quadwords for equality. 
 *    The following rules apply.
 *
 *        1) If t1 is equal to t2 then return 0.
 *        2) If t1 is less then t2 return -1.
 *        3) If t1 is greater then t2 return 1. 
 *
 *    This is the same semantics as strcmp().
 *
 * Modification History
 *
 *    01-Dec-2000 K.Esteb
 *        Updated the function to use lib$day() to return the number of days
 *        from 17-Nov-1858 and the number of milliseconds from midnight of
 *        the current day.
 *
 * Variables Used
 */

    int stat;
    ULONG day1;
    ULONG day2;
    ULONG msec1;
    ULONG msec2;

/*
 * Main part of function.
 */

    stat = lib$day(&day1, q1, &msec1);
    vms_check_status(stat);

    stat = lib$day(&day2, q2, &msec2);
    vms_check_status(stat);

    if (day1 == day2) {

        if (msec1 == msec2) return(0);
        if (msec1 < msec2)  return(-1);
        if (msec1 > msec2)  return(1);

    } 

    if (day1 > day2) return(1);
    if (day1 < day2) return(-1);

}


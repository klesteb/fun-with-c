
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 1995 by Kevin L. Esteb                                     */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <descrip.h>
#include <starlet.h>
#include <lib$routines.h>

#include "vms.h"

/*----------------------------------------------------------------------*/

void delay(long wait_time) {
/*
 * Function: delay.c
 * Author  : Kevin Esteb
 * Version : 1.0
 * Date    : 10-Aug-1995
 *
 * Description
 *
 *    This function will pause for the specified time. A fix for a
 *    broken sleep() funciton in the vaxcrtl??
 *
 * Modifications
 *
 * Variables
 */

    char mask[] = "0 %02.d:%02.d:%02.d.00";
    char timbuf[24];
    long timadr[2];
    unsigned long stat;
    long hours, minutes, seconds, junk1;
    struct dsc$descriptor timdsc;

/*
 * Main part of function.
 */

    if (wait_time == 0) return;

    hours = wait_time / 3600;
    junk1 = (wait_time % 3600);
    minutes = junk1 / 60;
    seconds = junk1 % 60;
    
    sprintf(timbuf, mask, hours, minutes, seconds);
    SET_SDESC(timdsc, timbuf, strlen(timbuf));

    stat = sys$bintim(&timdsc, &timadr);
    vms_check_status(stat);

    stat = sys$schdwk(0, 0, timadr, 0);
    vms_check_status(stat);

    stat = sys$hiber();
    vms_check_status(stat);

}


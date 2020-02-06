
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
#include "tools.h"

/*-------------------------------------------------------------------*/

extern char *xmalloc(int);

/*-------------------------------------------------------------------*/

char *format_date(DATE jdate, enum date_fmt) {
/*
 * Function: format_date.c
 * Version : 1.0
 * Created : 21-Nov-1998
 * Author  : Kevin Esteb
 *
 * Description
 *
 *  Format the julian day number into proper format.
 *
 * Modification History
 *
 * Variables Used
 */
 
    int day;
    int month;
    int year;
    char *datebuf;
    char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", Jun",
                      "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    
/*
 * Main part of function.
 */
 
    calendar_date(jdate, &day, &month, &year);
    datebuf = xmalloc(12);

    sprintf(datebuf, "%d-%s-%d", day, months[month - 1], year);
    
    return(datebuf);
    
}
    

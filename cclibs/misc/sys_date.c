
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

#include <time.h>
#include "tools.h"

/*-----------------------------------------------------------------------*/

DATE sys_date(void) {
/*
 * Function: sys_date.c
 * Version : 1.0
 * Created : 21-Nov-1998
 * Author  : Kevin Esteb
 *
 * Description
 *
 *  This function will return the julian day number of the current 
 *  system date.
 *
 * Modification History
 *
 * Variables Used
 */
 
    DATE epoch = 2440587L;              /* start of UNIX time           */
    DATE today;
    time_t t;
    struct tm *tb;
    
/*
 * Main part of function
 */
 
    t = time(NULL);
    tb = localtime(&t);

    today = julian_date(tb->tm_mday, tb->tm_mon, tb->tm_year + 1900);

    return(today);

}
    

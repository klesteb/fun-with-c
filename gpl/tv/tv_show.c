
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 1996 by Charles A. Measday                                 */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include "tv_priv.h"

/*----------------------------------------------------------------------*/

/**/

/*******************************************************************************

Procedure:

    tv_show ()


Purpose:


*******************************************************************************/


char  *tv_show (

#    if __STDC__
        struct  timeval  binaryTime,
        int  inLocal,
        const  char  *format)
#    else
        binaryTime, inLocal, format)

        struct  timeval  binaryTime ;
        int  inLocal ;
        const  char  *format ;
#    endif

{
/*
 * Function: tv_show.c
 * Version : 1.0
 * Created : 11-Dec-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function tv_show() returns the ASCII representation of a binary TIMEVAL
 *    structure (seconds and microseconds).  A caller-specified, strftime(3)
 *    format is used to format the ASCII representation.
 *
 *    Invocation:
 *
 *    asciiTime = tv_show(binaryTime, inLocal, format);
 *
 *    where
 *
 *        <binaryTime>        - I
 *            Is the binary TIMEVAL that will be converted to ASCII.
 *
 *        <inLocal>           - I
 *            Specifies (true or false) if the binary time is to be adjusted
 *            to local time by adding the GMT offset.
 *
 *        <format>            - I
 *            Is the STRFTIME(3) format used to format the binary time in
 *            ASCII.  If this argument is NULL, the time is formatted as
 *            "YYYY-DOY-HR:MN:SC.MLS".
 *
 *        <asciiTime>         - O
 *            Returns a pointer to an ASCII string, which contains the 
 *            formatted time. The ASCII string is stored in multiply-buffered 
 *            memory local to tv_show(); tv_show() can be called 4 times 
 *            before it overwrites the result from the earliest call.
 *
 * Modification History
 *
 * Variables Used
 */

    struct tm calendarTime;
#define  MAX_TIMES  4
    static char asciiTime[MAX_TIMES][64];
    static int  current = 0;

/*
 * Main part of function.
 */

    /* Convert the TIMEVAL to calendar time: year, month, day, etc.     */

#ifdef VXWORKS
    if (inLocal) {

        localtime_r((time_t *) &binaryTime.tv_sec, &calendarTime);

    } else {

        gmtime_r((time_t *) &binaryTime.tv_sec, &calendarTime);

    }
#else
    if (inLocal) {

        calendarTime = *(localtime((time_t *) &binaryTime.tv_sec));

    } else {

        calendarTime = *(gmtime((time_t *) &binaryTime.tv_sec));

    }
#endif

    /* Format the time in ASCII.                                        */

    current = (current + 1) % MAX_TIMES;

    if (format == NULL) {

        strftime(asciiTime[current], 64, "%Y-%j-%H:%M:%S", &calendarTime);
        sprintf(asciiTime[current] + strlen(asciiTime[current]),
                ".%03ld", (binaryTime.tv_usec % 1000000) / 1000);

    } else {

        strftime(asciiTime[current], 64, format, &calendarTime);

    }

    return(asciiTime[current]);

}


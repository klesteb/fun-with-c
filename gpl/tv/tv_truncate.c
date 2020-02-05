
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

struct  timeval  tv_truncate (

#    if __STDC__
        struct  timeval  fullTime,
        TvUnit  unit)
#    else
        fullTime, unit)

        struct  timeval  fullTime ;
        TvUnit  unit ;
#    endif

{
/*
 * Function: tv_truncate.c
 * Version : 1.0
 * Created : 11-Dec-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function tv_truncate() truncates a TIMEVAL to the beginning of the year,
 *    the beginning of the day, etc.  The possible units of truncation are
 *    enumerated in "tv_util.h".
 *
 *    Invocation:
 *
 *        truncatedTime = tv_truncate(fullTime, unit);
 *
 *    where
 *
 *        <fullTime>          - I
 *            Is a time represented as a UNIX TIMEVAL structure.
 *
 *        <unit>              - I
 *            Is the unit of truncation; see the enumerated type TvUnit defined
 *            in "tv_util.h".
 *
 *        <truncatedTime>     - O
 *            Returns a UNIX TIMEVAL structure representing the specified time,
 *            truncated to the specified unit.
 *
 * Modification History
 *
 * Variables Used
 */

    time_t seconds;
    struct timeval result;
    struct tm gmt;

/*
 * Main part of function.
 */

    /* Break the TIMEVAL down into years, months, days, hours, minutes, */
    /* and seconds and then truncate the appropriate units.             */

    seconds = fullTime.tv_sec;
    gmt = *gmtime(&seconds);

    switch (unit) {
        case TvDecade:
            gmt.tm_year = (gmt.tm_year / 10) * 10;

        case TvYear:
            gmt.tm_mon = 0;

        case TvMonth:
            gmt.tm_mday = 1;

        case TvWeek: /* ... truncate to day first ... */
        case TvDay:
            gmt.tm_hour = 0;

        case TvHour:
            gmt.tm_min = 0;

        case TvMinute:
            gmt.tm_sec = 0;

        default:
            break;

    }

    gmt.tm_isdst = -1;

    /* If truncating to the beginning of the week, call mktime(3) to    */
    /* determine the day of the week and subtract that from the day of  */
    /* the month.                                                       */

    if (unit == TvWeek) {

        mktime(&gmt);
        gmt.tm_mday -= gmt.tm_wday;

    }

    /* Convert the truncated TM structure back to a TIMEVAL.            */

    result.tv_sec = mktime(&gmt);
    result.tv_usec = 0;

    return(result);

}


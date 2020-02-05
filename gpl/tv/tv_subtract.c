
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

struct  timeval  tv_subtract (

#    if __STDC__
        struct  timeval  time1,
        struct  timeval  time2)
#    else
        time1, time2)

        struct  timeval  time1 ;
        struct  timeval  time2 ;
#    endif

{
/*
 * Function: tv_subtract.c
 * Version : 1.0
 * Created : 11-Dec-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function tv_subtract() subtracts one TIMEVAL from another TIMEVAL.
 *
 *    Invocation:
 *
 *        result = tv_subtract(time1, time2);
 *
 *    where
 *
 *        <time1>             - I
 *            Is the minuend (didn't you graduate from elementary school?),
 *            a time represented by a UNIX TIMEVAL structure.
 *
 *        <time2>             - I
 *            Is the subtrahend (I repeat the question), a time represented
 *            by a UNIX TIMEVAL structure.
 *
 *        <result>            - O
 *            Returns, in a UNIX TIMEVAL structure, TIME1 minus TIME2.  If
 *            TIME2 is greater than TIME1, then a time of zero is returned.
 *
 * Modification History
 *
 * Variables Used
 */

    struct timeval result;

/*
 * Main part of function.
 */

    /* Subtract the second time from the first.                         */

    if ((time1.tv_sec < time2.tv_sec) ||
        ((time1.tv_sec == time2.tv_sec) &&
         (time1.tv_usec <= time2.tv_usec))) {       /* TIME1 <= TIME2?  */
                                                        
        result.tv_sec = result.tv_usec = 0;

    } else {                                        /* TIME1 > TIME2    */

        result.tv_sec = time1.tv_sec - time2.tv_sec;

        if (time1.tv_usec < time2.tv_usec) {

            result.tv_usec = time1.tv_usec + 1000000 - time2.tv_usec;
            result.tv_sec--;                        /* Borrow a second. */

        } else {

            result.tv_usec = time1.tv_usec - time2.tv_usec;

        }

    }

    return(result);

}


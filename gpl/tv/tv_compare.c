
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

int  tv_compare (

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
 * Function: tv_compare.c
 * Version : 1.0
 * Created : 11-Dec-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function tv_compare() compares two TIMEVALs.
 *
 *    Invocation:
 *
 *        comparison = tv_compare(time1, time2);
 *
 *    where
 *
 *        <time1>             - I
 *            Is a time represented by a UNIX TIMEVAL structure.
 *
 *        <time2>             - I
 *            Is another time represented by a UNIX TIMEVAL structure.
 *
 *        <comparison>        - O
 *            Returns an integer indicating how the times compare:
 *                -1  if TIME1 < TIME2
 *                 0  if TIME1 = TIME2
 *                +1  if TIME1 > TIME2
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    if (time1.tv_sec < time2.tv_sec) {

        return(-1);                           /* Less than.             */

    } else if (time1.tv_sec > time2.tv_sec) {

        return(1);                            /* Greater than.          */

    } else if (time1.tv_usec < time2.tv_usec) {

        return(-1);                           /* Less than.             */

    } else if (time1.tv_usec > time2.tv_usec) {

        return(1);                            /* Greater than.          */

    } else {

        return(0);                            /* Equal.                 */

    }

}



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

struct  timeval  tv_createf (

#    if __STDC__
        double  fSeconds)
#    else
        fSeconds)

        double  fSeconds ;
#    endif

{
/*
 * Function: tv_createf.c
 * Version : 1.0
 * Created : 11-Dec-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function tv_createf(), given a floating-point number representing a
 *    time in seconds and fractional seconds, creates and returns a TIMEVAL
 *    representing that time.
 *
 *    Invocation:
 *
 *        result = tv_createf(fSeconds);
 *
 *    where
 *
 *        <fSeconds>          - I
 *            Is a floating-point number representing a time in seconds and
 *            fractional seconds.
 *
 *        <result>            - O
 *            Returns a UNIX TIMEVAL structure representing the specified 
 *            time.
 *
 * Modification History
 *
 * Variables Used
 */

    struct timeval result;

/*
 * Main part of function.
 */

    if (fSeconds < 0) {                             /* Negative time?   */

        result.tv_sec = 0;
        result.tv_usec = 0;

    } else if (fSeconds > (double)LONG_MAX) {       /* Time too large?  */

        result.tv_sec = LONG_MAX;
        result.tv_usec = 999999;

    } else {                                        /* Valid time.      */

        result.tv_sec = fSeconds;
        result.tv_usec = (fSeconds - (double) result.tv_sec) * 1000000.0;

    }

    return(result);

}


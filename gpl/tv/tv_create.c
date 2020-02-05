
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

struct  timeval  tv_create (

#    if __STDC__
        long  seconds,
        long  microseconds)
#    else
        seconds, microseconds)

        long  seconds ;
        long  microseconds ;
#    endif

{
/*
 * Function: tv_create.c
 * Version : 1.0
 * Created : 11-Dec-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function tv_create(), given a time specified in seconds and 
 *    microseconds, creates and returns a TIMEVAL representing that 
 *    time.
 *
 *    Invocation:
 *
 *        result = tv_create(seconds, microseconds);
 *
 *    where
 *
 *        <seconds>           - I
 *            Is the whole number of seconds in the time.
 *
 *        <microseconds>      - I
 *            Is the remaining microseconds in the time.
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

    seconds = (seconds < 0) ? 0 : seconds;
    microseconds = (microseconds < 0) ? 0 : microseconds;

    /* "Normalize" the time so that the microseconds field is less      */
    /* than a million.                                                  */

    while (microseconds >= 1000000) {

        seconds++;  
        microseconds = microseconds - 1000000;

    }

    /* Return the time in a TIMEVAL structure.                          */

    result.tv_sec = seconds;
    result.tv_usec = microseconds;

    return(result);

}


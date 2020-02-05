
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

double  tv_float (

#    if __STDC__
        struct  timeval  time)
#    else
        time)

        struct  timeval  time ;
#    endif

{
/*
 * Function: tv_float.c
 * Version : 1.0
 * Created : 11-Dec-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function tv_float() converts a TIMEVAL structure (seconds and 
 *    microseconds) into the equivalent, floating-point number of seconds.
 *
 *    Invocation:
 *
 *        realTime = tv_float(time);
 *
 *    where
 *
 *        <time>          - I
 *            Is the TIMEVAL structure that is to be converted to floating 
 *            point.
 *
 *        <realTime>      - O
 *            Returns the floating-point representation in seconds of the 
 *            time.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    return((double) time.tv_sec + (time.tv_usec / 1000000.0));

}


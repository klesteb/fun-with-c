
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

struct  timeval  tv_tod (

#    if __STDC__
        void)
#    else
        )
#    endif

{
/*
 * Function: tv_tod.c
 * Version : 1.0
 * Created : 11-Dec-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function tv_tod() returns the current time-of-day (GMT).  tv_tod()
 *    simply calls the UNIX system function, GETTIMEOFDAY(3), but it
 *    provides a simple means of using the current time in "timeval"
 *    calculations without having to allocate a special variable for
 *    it and making a separate call (i.e., you can "inline" a call to
 *    tv_tod() within a call to one of the other TV_UTIL functions).
 *
 *        NOTE: Under VxWorks, tv_tod() returns the current value
 *              of the real-time clock, not GMT.
 *
 *    Invocation:
 *
 *        currentGMT = tv_tod();
 *
 *    where
 *
 *        <currentGMT>        - O
 *            Returns, in a UNIX TIMEVAL structure, the current GMT.  (Under
 *            VxWorks, the current value of the real-time clock is returned
 *            instead of GMT.)
 *
 * Modification History
 *
 * Variables Used
 */

    struct timeval currentGMT;

/*
 * Main part of function.
 */

#ifdef  VXWORKS
    struct  timespec  timeOfDay ;

    clock_gettime(CLOCK_REALTIME, &timeOfDay);
    currentGMT.tv_sec = timeOfDay.tv_sec;
    currentGMT.tv_usec = timeOfDay.tv_nsec / 1000;
#else
    gettimeofday(&currentGMT, NULL);
#endif

    return(currentGMT);

}


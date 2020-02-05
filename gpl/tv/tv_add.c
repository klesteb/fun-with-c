
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

struct  timeval  tv_add (

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
 * Function: tv_add.c
 * Version : 1.0
 * Created : 11-Dec-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function tv_add() returns the sum of two TIMEVALs.
 *
 *    Invocation:
 *
 *        result = tv_add(time1, time2);
 *
 *    where
 *
 *        <time1>             - I
 *            Is the first operand, a time represented by a UNIX TIMEVAL
 *            structure.
 *
 *        <time2>             - I
 *            Is the second operand, a time represented by a UNIX TIMEVAL
 *            structure.
 *
 *        <result>            - O
 *            Returns, in a UNIX TIMEVAL structure, the sum of TIME1 and
 *            TIME2.
 *
 * Modification History
 *
 * Variables Used
 */

    struct timeval result;

/*
 * Main part of function.
 */

    /* Add the two times together.                                      */

    result.tv_sec = time1.tv_sec + time2.tv_sec;
    result.tv_usec = time1.tv_usec + time2.tv_usec;

    if (result.tv_usec > 1000000) {                     /* Carry?       */

        result.tv_sec++;  
        result.tv_usec = result.tv_usec - 1000000;

    }

    return(result);

}


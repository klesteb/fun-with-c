
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 2000 by Kevin L. Esteb                                     */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include "que_priv.h"

/*----------------------------------------------------------------------*/

/**/

int que_empty(

#    if __STDC__
    queue *q)
#    else
    q)

    queue *q;
#    endif

{
/*
 * Function: que_empty.c
 * Version : 1.0
 * Created : 02-Jan-1993
 * Author  : Peter Yard
 *
 * Description
 *
 *    Function que_empty() tests if queue has anything in it. 
 *
 *    Invocation:
 *
 *        status = que_empty(q);
 *
 *    where
 *
 *        <q>                 - I
 *            The queue data struture.
 *
 *        <status>            - O
 *            The funtion returns QUE_OK if there is any data, otherwise 
 *            QUE_ERROR.
 *
 * Modification History
 *
 *    10-May-2002 K.Esteb
 *        Reformated the file.
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    return(q->size == 0);

} 


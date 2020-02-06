
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 1998 by Kevin L. Esteb                                     */
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

int que_end(

#    if __STDC__
    queue *q)
#    else
    q)

    queue *q;
#    endif

{
/*
 * Function: que_end.c
 * Version : 1.0
 * Created : 02-Jan-1993
 * Author  : Peter Yard
 *
 * Description
 *
 *    Function que_end() will do a boolean test to see if the cursor is at 
 *    the tail of queue.
 *
 *    Invocation:
 *
 *        status = que_end(q);
 *
 *    where
 *
 *        <q>                 - I
 *            The queue data struture.
 *
 *        <status>            - O
 *            This function will return QUE_OK if at the end otherwise 
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

    return(q->cursor == q->tail);

} 


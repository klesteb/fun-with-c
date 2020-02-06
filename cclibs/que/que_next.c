
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

void *que_next(

#    if __STDC__
    queue *q)
#    else
    q)

    queue *q;
#    endif

{
/*
 * Function: que_next.c
 * Version : 1.0
 * Created : 02-Jan-1993
 * Author  : Peter Yard
 *
 * Description
 *
 *    Function que_next() will move to the next element in the queue 
 *    without popping. This uses the cursor for the current position. 
 *    que_next() only moves in the direction from the head of the queue 
 *    to the tail. 
 *
 *    Invocation
 *
 *        data = que_next(q);
 *
 *    where
 *
 *        <q>                   - I
 *            Is the data structure returned for que_init().
 *
 *        <data>                - O
 *            The data item at the current cursor postion.
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

    if (q->cursor->next == NULL) return(NULL); 

    q->cursor = (node *)q->cursor->next; 

    return(q->cursor->data);

} 


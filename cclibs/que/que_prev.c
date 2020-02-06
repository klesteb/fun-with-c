
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

void *que_prev(

#    if __STDC__
    queue *q)
#    else
    q)

    queue *q;
#    endif

{
/*
 * Function: que_prev.c
 * Version : 1.0
 * Created : 02-Jan-1993
 * Author  : Peter Yard
 *
 * Description
 *
 *    Function que_prev() will move to the previous element in the queue 
 *    without popping. This uses the cursor for the current position. 
 *
 *    Invocation
 *
 *        data = que_prev(q);
 *
 *    where
 *
 *        <q>                   - I
 *            Is the data structure returned for que_init().
 *
 *        <data>                - O
 *            The data item at the current cursor postion or NULL if the
 *            queue is empty.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    if (q->cursor->prev == NULL) return(NULL); 

    q->cursor = (node *)q->cursor->prev; 

    return(q->cursor->data);

} 


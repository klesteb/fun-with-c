
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

void *que_first(

#    if __STDC__
    queue *q)
#    else
    q)

    queue *q;
#    endif

{
/*
 * Function: que_first.c
 * Version : 1.0
 * Created : 02-Jan-1993
 * Author  : Peter Yard
 *
 * Description
 *
 *    Function que_first() will position queue cursor to the first element 
 *    (head) of the queue. 
 *
 *    Invocation
 *
 *        data = que_first(q);
 *
 *    where
 *
 *        <q>                 - I
 *            The queue data struture.
 *
 *        <data>              - O
 *            The data held at that location or NULL.
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

    if (que_empty(q)) return(NULL); 

    q->cursor = q->head; 

    return(q->cursor->data);

} 


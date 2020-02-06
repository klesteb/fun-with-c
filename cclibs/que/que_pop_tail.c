
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

void *que_pop_tail(

#    if __STDC__
    queue *q)
#    else
    q)

    queue *q;
#    endif

{
/*
 * Function: que_pop_tail.c
 * Version : 1.0
 * Created : 02-Jan-1993
 * Author  : Peter Yard
 *
 * Description
 *
 *    Function que_pop_tail() will remove and return the data element at 
 *    the tail of the queue. 
 * 
 *    Invocation
 *
 *        data = que_pop_tail(q);
 *
 *    where
 *
 *        <q>                 - I
 *            Is the data structure returned for que_init().
 *
 *        <data>              - O
 *            Is the data item at the current cursor position or NULL
 *            if the queue is empty.
 *
 * Modification History
 *
 * Variables Used
 */

    datanode *p; 
    void     *d; 

/*
 * Main part of function.
 */

    if (que_empty(q)) return(NULL); 

    d = q->tail->data; 
    p = q->tail->prev; 

    free(q->tail); 
    q->size--; 

    if (q->size == 0) { 

        q->head = q->tail = q->cursor = NULL; 

    } else { 

        q->tail = (node *)p; 
        q->tail->next = NULL; 
        q->cursor = q->tail; 

    } 

    q->sorted = FALSE; 

    return(d);

} 


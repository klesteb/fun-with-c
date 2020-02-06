
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

void *que_delete(

#    if __STDC__
    queue *q)
#    else
    q)

    queue *q;
#    endif

{
/*/*
 * Function: que_delete.c
 * Version : 1.0
 * Created : 02-Jan-1993
 * Author  : Peter Yard
 *
 * Description
 *
 *    Function que_delete() will delete the current queue element as pointed 
 *    to by the cursor. 
 * 
 *    WARNING! It is the responsibility of the caller to free any memory. 
 *    The QUE routines cannot distinguish between pointers to literals and 
 *    malloced memory. 
 *
 *    Invocation:
 *
 *        data = que_delete(q);
 *
 *    where
 *
 *        <q>                 - I
 *            The queue data struture.
 *
 *        <data>              - O
 *            The data held at that location in the queue.
 *
 * Modification History
 *
 *    10-May-2002 K.Esteb
 *        Reformated the file.
 *
 * Variables Used
 */

    void     *d; 
    datanode *n, *p, *c; 

/*
 * Main part of function.
 */

    if (q->cursor == NULL) return(NULL); 
    if (q->cursor == q->head) return(que_pop_head(q)); 
    if (q->cursor == q->tail) return(que_pop_tail(q)); 

    n = q->cursor->next; 
    p = q->cursor->prev; 
    d = q->cursor->data; 
    c = q->cursor;

    if (p != NULL) { 

        p->next = n;
        n->prev = p;
        q->cursor = p;

    } else {

        n->prev = p;
        p->next = n;
        q->cursor = n;

    }

    free(c); 

    q->size--; 
    q->sorted = FALSE; 

    return(d);

} 


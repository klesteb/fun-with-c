
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

int que_insert(

#    if __STDC__
    queue *q, void *d)
#    else
    q, d)

    queue *q;
    void *d;
#    endif

{
/*
 * Function: que_insert.c
 * Version : 1.0
 * Created : 02-Jan-1993
 * Author  : Peter Yard
 *
 * Description
 *
 *    Function que_insert() will insert a data element into the queue at the
 *    current cursor position.
 * 
 *    Invocation
 *
 *        status = que_insert(q, d);
 *
 *    where
 *
 *        <q>                   - I/O
 *            The queue data structure returned from que_init().
 *
 *        <d>                   - I
 *            The data to be inserted into the queue.
 *
 *        <status>              - O
 *            This function will return QUE_OK if successful, otherwise 
 *            QUE_ERROR.
 *
 * Modification History
 *
 *    10-May-2002 K.Esteb
 *        Reformated the file.
 *
 * Variables Used
 */

    node     *c;
    node     *n; 
    datanode *x;

/*
 * Main part of function.
 */

    if (q->cursor == NULL) return(QUE_ERROR);
    if (q->cursor == q->head) return(que_push_head(q, d));
    if (q->cursor == q->tail) return(que_push_tail(q, d));

    if ((x = malloc(sizeof(datanode))) != NULL) {

        c = q->cursor;
        n = q->cursor->next;

        x->next = n;
        x->prev = c;
        c->next = x;
        n->prev = x;
        x->data = d;

        q->size++;
        q->cursor = x;
        q->sorted = QUE_ERROR;

        return(QUE_OK);

    }

    return(QUE_ERROR);

} 



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

int que_push_tail(

#    if __STDC__
    queue *q, void *d)
#    else
    q, d)

    queue *q;
    void *d;
#    endif

{
/*
 * Function: que_push_tail.c
 * Version : 1.0
 * Created : 02-Jan-1993
 * Author  : Peter Yard
 *
 * Description
 *
 *    Function que_push_tail() will put a data pointer at the tail of 
 *    the queue.
 * 
 *    Invocation
 *
 *        status = que_push_tail(q, d);
 *
 *    where
 *
 *        <q>                 - I
 *            Is the data structure returned from que_init().
 *
 *        <d>                 - I
 *            Is the data item to be pushed onto the queue.
 *
 *        <status>            - O
 *            This function will return QUE_OK if success else QUE_ERROR
 *            if unable to push the data. 
 *
 * Modification History
 *
 *    10-May-2002 K.Esteb
 *        Reformated the file.
 *
 * Variables Used
 */

    node     *p;
    datanode *n; 

/*
 * Main part of function.
 */

    if (q->tail->data != NULL) {

        if ((n = malloc(sizeof(datanode))) != NULL) {

            p = q->tail;
            q->tail = (node *)n; 
            q->tail->next = NULL; 
            p->next = q->tail;
            q->tail->prev = p;

        } else return(QUE_ERROR);

    }

    q->tail->data = d; 
    q->size++; 
    q->cursor = q->tail; 
    q->sorted = QUE_ERROR;

    return(QUE_OK);

} 


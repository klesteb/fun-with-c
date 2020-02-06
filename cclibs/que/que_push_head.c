
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

int que_push_head(

#    if __STDC__
    queue *q, void *d)
#    else
    q, d)

    queue *q;
    void *d;
#    endif

{
/*
 * Function: que_push_head.c
 * Version : 1.0
 * Created : 02-Jan-1993
 * Author  : Peter Yard
 *
 * Description
 *
 *    Function que_push_head() will put a data pointer at the head of 
 *    the queue.
 * 
 *    Invocation
 *
 *        status = que_push_head(q, d);
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

    node     *n;
    datanode *p;

/*
 * Main part of function.
 */

    if (q->head->data != NULL) {

        if ((n = malloc(sizeof(datanode))) != NULL) {

            p = q->head;
            q->head = (node *)n; 
            q->head->prev = NULL; 
            p->prev = q->head;
            q->head->next = p;

        } else return(QUE_ERROR);

    }

    q->head->data = d; 
    q->size++;
    q->cursor = q->head; 
    q->sorted = QUE_ERROR;
    q->head->data = d; 

    return(QUE_OK);

} 



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

int que_util_debug;               /* Global debug switch (1/0 = yes/no).*/

/*----------------------------------------------------------------------*/

/**/ 

int que_init(

#    if __STDC__
    queue *q)
#    else
    q)

    queue *q;
#    endif

{
/*
 * Function: que_init.c
 * Version : 1.0
 * Created : 02-Jan-1993
 * Author  : Peter Yard
 *
 * Description
 *
 *    Function que_init() will initialise the queue object and pointers. 
 * 
 *    Invocation:
 *
 *        status = que_init(q);
 *
 *    where
 *
 *        <q>                 - I
 *            The queue data struture.
 *
 *        <status>            - O
 *            This function will return QUE_OK if init successful else 
 *            QUE_ERROR.
 *
 * Modification History
 *
 *    10-May-2002 K.Esteb
 *        Reformated the file.
 *
 * Variables Used
 */

    node *x;

/*
 * Main part of function.
 */

    if ((x = malloc(sizeof(datanode))) != NULL) {

        q->head = (node *)x;
        q->tail = (node *)x;

        q->head->next = q->tail;
        q->head->prev = NULL;
        q->head->data = NULL;
        q->tail->prev = q->head;
        q->tail->next = NULL;
        q->tail->data = NULL;

        q->size = 0; 
        q->cursor = q->tail; 
        q->sorted = QUE_ERROR; 

        return(QUE_OK);

    }

    return(QUE_ERROR);

} 


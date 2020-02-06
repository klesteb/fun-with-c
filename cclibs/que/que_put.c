
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

int que_put(queue *q, void *data) { 
/*
 * Function: que_put.c
 * Version : 1.0
 * Created : 02-Jan-1993
 * Author  : Peter Yard
 *
 * Description
 *
 *    Function que_put() will replace the current pointer to data with 
 *    a new pointer to other data. 
 * 
 *    Invocation
 *
 *        status = que_put(q, d);
 *
 *    where
 *
 *        <q>                  - I
 *            Is the data structure returned from que_init();
 *
 *        <d>                  - I
 *            Is the data item to replace the current data item.
 *
 *        <status>             - O
 *            This function will return QUE_OK if successful, QUE_ERROR
 *            if cursor at NULL 
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

    if (q->cursor == NULL) return(QUE_ERROR); 

    q->cursor->data = data; 

    return(QUE_OK);

} 



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

int que_find(

#    if __STDC__
    queue *q, void *data, comp_proc *Comp)
#    else
    q, data, Comp)

    queue *q;
    void *data;
    comp_proc *Comp;
#    endif

{
/*
 * Function: que_find.c
 * Version : 1.0
 * Created : 02-Jan-1993
 * Author  : Peter Yard
 *
 * Description
 *
 *    Function que_find() will do a linear search of queue for match 
 *    with key in data.
 *
 *    Useful for small queues that are constantly changing and would 
 *    otherwise need constant sorting with the que_seek() function. For 
 *    description of Comp see que_sort(). Queue cursor left on position 
 *    found item else at end. 
 * 
 *    Invocation:
 *
 *        status = que_find(q, data, routine);
 *
 *    where
 *
 *        <q>                 - I
 *            The queue data struture.
 *
 *        <data>              - I
 *            The data item to match against.
 *
 *        <routine>           - I
 *            The routine to do the comparision with.
 *
 *        <status>            - O
 *            This routine will return QUE_OK if found, QUE_ERROR if not 
 *            in queue. 
 *
 * Modification History
 *
 *    10-May-2002 K.Esteb
 *        Reformated the file.
 *
 * Variables Used
 */

    void *d; 

/*
 * Main part of function.
 */

    d = que_first(q); 

    do { 

        if (Comp(d, data) == 0) return(QUE_OK); 
        d = que_next(q); 

    } while (!que_end(q)); 

    if (Comp(d, data) == 0) return(QUE_OK); 

    return(QUE_ERROR); 

} 


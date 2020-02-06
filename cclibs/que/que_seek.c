
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

static int que_bsearch(

#    if __STDC__
    queue *q, void *key, comp_proc *Comp)
#    else
    q, key Comp)

    queue *q;
    void *key;
    comp_proc *Comp;
#    endif

{
/*
 * Function: que_bsearch.c
 * Version : 1.0
 * Created : 02-Jan-2002
 * Author  : Peter Yard
 *
 * Description
 *
 *    Function que_bsearch() will do a binary search of queue index 
 *    for node containing the key. See que_sort() for description of 'Comp' 
 *    function. 
 * 
 *    Invocation
 *
 *        status = que_bsearch(q, key, routine);
 *
 *    where
 *
 *        <q>                 - I
 *            Is the data structure returned from que_init();
 *
 *        <key>               - I
 *            Is the key to search for.
 *
 *        <routine>           - I
 *            The comparison routine.
 *
 *        <status>            - O
 *            Integer index into array of node pointers, or -1 if not found. 
 *
 * Modification History
 *
 *    10-May-2002 K.Esteb
 *        Reformated the file.
 *
 * Variables Used
 */

    int low;
    int mid;
    int hi;
    int val; 

/*
 * Main part of function.
 */

    low = 0; 
    hi = q->size - 1; 

    while (low <= hi) { 

        mid = (low + hi) / 2; 
        val = Comp(key, q->index[mid]); 

        if (val < 0) { 

            hi = mid - 1; 

        } else if (val > 0) { 

            low = mid + 1; 

        } else return(mid);                /* Success                   */ 

    } 

    /* Not Found */ 

    return(-1); 

} 

/**/ 

void *que_seek(

#    if __STDC__
    queue *q, void *data, comp_proc *Comp)
#    else
    q, key Comp)

    queue *q;
    void *data;
    comp_proc *Comp;
#    endif

{
/*
 * Function: que_seek.c
 * Version : 1.0
 * Created : 02-Jan-1993
 * Author  : Peter Yard
 *
 * Description
 *
 *    Function que_seek() will use the index to locate data according to 
 *    key in 'data' 
 * 
 *    Invocation
 *
 *        data = que_seek(q, d, routine);
 *
 *    where
 *
 *        <q>                 - I
 *            Is the data structure returned from que_init();
 *
 *        <key>               - I
 *            Is the key to search for.
 *
 *        <routine>           - I
 *            The comparison routine.
 *
 *        <data>              - O
 *            This function returns a pointer to data or NULL if could not 
 *            find it or could not sort queue. 
 *
 * Modification History
 *
 *    10-May-2002 K.Esteb
 *        Reformated the file.
 *
 * Variables Used
 */

    int idx; 

/*
 * Main part of function.
 */

    if (!q->sorted) { 

        if (!que_sort(q, Comp)) return NULL;

    } 

    idx = que_bsearch(q, data, Comp); 

    if (idx < 0) return NULL; 

    q->cursor = q->posn_index[idx]; 

    return(q->index[idx]); 

} 


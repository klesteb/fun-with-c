
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

static void QuickSort(

#    if __STDC__
    void *list[], int low, int high, comp_proc *Comp) 
#    else
    list, low, high, Comp)

    void *list[];
    int low;
    int high;
    comp_proc *Comp;
#    endif

{
/*
 * Function: QuickSort
 * Version : 1.0
 * Created : 02-Jan-1993
 * Author  : Peter Yard
 *
 * Description
 *
 * Modification History
 *
 * Variables Used
 */

    int  flag = 1, i, j; 
    void *key, *temp; 

/*
 * Main part of function.
 */

    if (low < high) { 

        i = low; 
        j = high + 1; 
        key = list[low]; 

        while (flag) { 

            i++; 

            while (Comp(list[i], key) < 0) 
                i++; 

            j--; 

            while (Comp(list[j], key) > 0) 
                j--; 

            if (i < j) { 

                temp = list[i]; 
                list[i] = list[j]; 
                list[j] = temp; 

            } else  flag = 0; 

        } 

        temp = list[low]; 
        list[low] = list[j]; 
        list[j] = temp; 

        QuickSort(list, low, j-1, Comp); 
        QuickSort(list, j+1, high, Comp); 

    } 

} 

/**/ 

int que_sort(

#    if __STDC__
    queue *q, comp_proc *Comp)
#    else
    q, Comp)

    queue *q;
    comp_proc *Comp;
#    endif

{

/*
 * Function: que_sort.c
 * Version : 1.0
 * Created : 02-Jan-1993
 * Author  : Peter Yard
 *
 * Description
 *
 *    Function que_sort() will sort the queue and allow index style access. 
 *    The Comp function supplied by caller must return:
 * 
 *        -1 if data1  < data2 
 *         0 if data1 == data2 
 *        +1 if data1  > data2 
 * 
 *    For Comp(data1, data2). If the queue is already sorted it frees the 
 *    memory of the old index and starts again. 
 * 
 *    Invocation
 *
 *        status = que_sort(q, routine);
 *
 *    where
 *
 *        <q>                 - I
 *            Is the data struture returned from que_init().
 *
 *        <routine>           - I
 *            Is the comparison routine.
 *
 *        <status>            - O
 *            This function returns QUE_OK if sort succeeded. QUE_ERROR if 
 *            an error occurred. 
 * 
 * Modification History
 *
 *    10-May-2002 K.Esteb
 *        Reformated the file.
 *
 * Variables Used
 */

    int         i; 
    void        *d; 
    datanode    *dn; 

/*
 * Main part of function.
 */

    /* if already sorted free memory for tag array                      */ 

    if (q->sorted) { 

        free(q->index); 
        free(q->posn_index); 
        q->sorted = QUE_ERROR;

    } 

    /* Now allocate memory of array, array of pointers                  */ 

    q->index = malloc(q->size * sizeof(q->cursor->data)); 
    if (q->index == NULL) return(QUE_ERROR); 

    q->posn_index = malloc(q->size * sizeof(q->cursor)); 

    if (q->posn_index == NULL) { 

        free(q->index); 

        return(QUE_ERROR); 

    } 

    /* Walk queue putting pointers into array                           */ 

    d = que_first(q); 

    for (i = 0; i < q->size; i++) { 

        q->index[i] = d; 
        q->posn_index[i] = q->cursor; 
        d = que_next(q);

    } 

    /* Now sort the index */ 

    QuickSort(q->index, 0, q->size - 1, Comp); 

    /* Rearrange the actual queue into correct order */ 

    dn = q->head; 
    i = 0; 

    while (dn != NULL) { 

        dn->data = q->index[i++]; 
        dn = dn->next;

    } 

    /* Re-position to original element */ 

    if (d != NULL) { 

        que_find(q, d, Comp);

    } else  que_first(q); 

    q->sorted = QUE_OK; 

    return(QUE_OK); 

} 


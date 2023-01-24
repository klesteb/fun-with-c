
/**************************************************************** 
 * 
 *  File : deque.c 
 * 
 *  Author: Peter Yard [1993.01.02] -- 02 Jan 1993 
 * 
 *  Disclaimer: This code is released to the public domain. 
 * 
 *  Description: 
 * 
 *      Generic double ended queue (Deque pronounced DEK) for handling 
 *      any data types, with sorting. 
 * 
 *      By use of various functions in this module the caller 
 *      can create stacks, queues, lists, doubly linked lists, 
 *      sorted lists, indexed lists.  All lists are dynamic. 
 * 
 *      It is the responsibility of the caller to malloc and free 
 *      memory for insertion into the queue. A pointer to the object 
 *      is used so that not only can any data be used but various kinds 
 *      of data can be pushed on the same queue if one so wished e.g. 
 *      various length string literals mixed with pointers to structures 
 *      or integers etc. 
 * 
 *  Enhancements: 
 * 
 *      A future improvement would be the option of multiple "cursors" 
 *      so that multiple locations could occur in the one queue to allow 
 *      placemarkers and additional flexibility.  Perhaps even use queue 
 *      itself to have a list of cursors. 
 *
 *      The orignal can be found on Bob Stout's snippet collection 
 *      at http://www.snippets.org.
 * 
 * Modifications:
 * 
 *      16-Nov-1998 K.Esteb
 *          This code did not work correctly under VMS or Linux (memory access 
 *          violations). This was fixed by actually allocating memory to hold 
 *          the structures in que_push_tail() and que_push_head(). Apparently 
 *          it worked fine under MS-DOS - Hmmm.... 
 *
 *          I also changed the calling convention to reflect how I like 
 *          things. All statically allocated memory has been pushed into the 
 *          queue data structure. This should make the code reentrant and 
 *          thread safe. 
 *
 *       03-Nov-1999 K.Esteb
 *          Fixed several functions that were not working correctly.
 *
 *          que_init()      now allocates memory for the initial head/tail 
 *                          nodes.
 *          que_push_tail() will now populate the initial head/tail nodes 
 *                          before creating new nodes.
 *          que_push_head() will now populate the initial head/tail nodes 
 *                          before creating new nodes. It will also correctly
 *                          place new nodes at the beginning of the queue.
 *          que_delete()    will now delete a node from the list properly.
 *          que_insert()    will now insert a new node after the current one.
 *
 *          This changes were all instigated when it was discoverd that the
 *          library was not maintaining the list pointers properly.
 *
 *       30-Nov-2001 K.Esteb
 *          Fixed a small problem.
 *
 *          que_push_tail() will now place the first data item into the
 *                          correct node. que_pop_tail() now finds all of 
 *                          data items.
 *
 * Usage: 
 * 
 *          /x init queue x/ 
 *          queue  q; 
 *          que_init(&q); 
 * 
 *      To create a stack : 
 * 
 *          que_push_head(&q, &mydata1); /x push x/ 
 *          que_push_head(&q, &mydata2); 
 *          ..... 
 *          data_ptr = que_pop_head(&q); /x pop x/ 
 *          ..... 
 *          data_ptr = que_first(&q);   /x top of stack x/ 
 * 
 *      To create a FIFO: 
 * 
 *          que_push_head(&q, &mydata1); 
 *          ..... 
 *          data_ptr = que_pop_tail(&q); 
 * 
 *      To create a double list: 
 * 
 *          data_ptr = que_first(&q); 
 *          .... 
 *          data_ptr = que_next(&q); 
 *          data_ptr = que_last(&q); 
 *          if (que_empty(&q)) .... 
 *          ..... 
 *          data_ptr = que_previous(&q); 
 * 
 * 
 *      To create a sorted list: 
 * 
 *          que_push_head(&q, &mydata1); /x push x/ 
 *          que_push_head(&q, &mydata2); 
 *          ..... 
 *          if (!que_sort(&q, MyFunction)) 
 *              .. error .. 
 * 
 *          /x fill in key field of mydata1. 
 *           * NB: que_find does linear search 
 *           x/ 
 * 
 *          if (que_find(&q, &mydata1, MyFunction)) 
 *          { 
 *              /x found it, queue cursor now at correct record x/ 
 *              /x can retrieve with x/ 
 *              data_ptr = que_get(&q); 
 * 
 *              /x alter data , write back with x/ 
 *              que_put(&q, data_ptr); 
 *          } 
 * 
 *          /x Search with binary search x/ 
 *          if (que_seek(&q, &mydata, MyFunction)) 
 *              /x etc x/ 
 * 
 * 
 ****************************************************************/ 

#include <stdlib.h> 
#include "deque.h"

static void QuickSort(void *list[], int low, int high, 
                      int (*Comp)(const void *, const void *)); 

static int  que_bsearch(queue *q, void *key, 
                      int (*Comp)(const void *, const void *)); 

/* The index: a pointer to pointers */ 

/* static  void        **index; 
 * static  datanode    **posn_index; 
 */

/**/ 

/*** 
 * 
 ** function    : que_init 
 * 
 ** purpose     : Initialise queue object and pointers. 
 * 
 ** parameters  : 'queue' pointer. 
 * 
 ** returns     : TRUE if init successful else FALSE 
 * 
 ** comments    : 
 ***/ 

int que_init(queue  *q) { 

    node *x, *y;

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
        q->sorted = FALSE; 

        return(TRUE);

    }

    return(FALSE);

} 

/**/ 

/*** 
 * 
 ** function    : que_start 
 * 
 ** purpose     : tests if cursor is at head of queue 
 * 
 ** parameters  : 'queue' pointer. 
 * 
 ** returns     : boolean - TRUE is at head else FALSE 
 * 
 ** comments    : 
 * 
 ***/ 

int que_start(queue *q) { 

    return(q->cursor == q->head);

} 

/**/ 

/*** 
 * 
 ** function    : que_end 
 * 
 ** purpose     : boolean test if cursor at tail of queue 
 * 
 ** parameters  : 'queue' pointer to test. 
 * 
 ** returns     : TRUE or FALSE 
 * 
 ** comments    : 
 * 
 ***/ 

int que_end(queue *q) { 

    return(q->cursor == q->tail);

} 

/**/ 

/*** 
 * 
 ** function    : que_empty 
 * 
 ** purpose     : test if queue has nothing in it. 
 * 
 ** parameters  : 'queue' pointer 
 * 
 ** returns     : TRUE if empty queue, else FALSE 
 * 
 ** comments    : 
 * 
 ***/ 

int que_empty(queue *q) { 

    return(q->size == 0);

} 

/**/ 

/*** 
 * 
 ** function    : que_size 
 * 
 ** purpose     : return the number of elements in the queue 
 * 
 ** parameters  : queue pointer 
 * 
 ** returns     : number of elements 
 * 
 ** comments    : 
 * 
 ***/ 

int que_size(queue *q) { 

    return(q->size);

} 

/**/ 

/*** 
 * 
 ** function    : que_first 
 * 
 ** purpose     : position queue cursor to first element (head) of queue. 
 * 
 ** parameters  : 'queue' pointer 
 * 
 ** returns     : pointer to data at head. If queue is empty returns NULL 
 * 
 ** comments    : 
 * 
 ***/ 

void *que_first(queue *q) { 

    if (que_empty(q)) return(NULL); 

    q->cursor = q->head; 

    return(q->cursor->data);

} 

/**/ 

/*** 
 * 
 ** function    : que_last 
 * 
 ** purpose     : locate cursor at tail of queue. 
 * 
 ** parameters  : 'queue' pointer 
 * 
 ** returns     : pointer to data at tail , if queue empty returns NULL 
 * 
 ** comments    : 
 * 
 ***/ 

void *que_last(queue *q) { 

    if (que_empty(q)) return(NULL); 

    q->cursor = q->tail; 

    return(q->cursor->data);

} 

/**/ 

/*** 
 * 
 ** function    : que_push_head 
 * 
 ** purpose     : put a data pointer at the head of the queue 
 * 
 ** parameters  : 'queue' pointer, void pointer to the data. 
 * 
 ** returns     : TRUE if success else FALSE if unable to push data. 
 * 
 ** comments    : 
 * 
 ***/ 

int que_push_head(queue *q, void *d) { 

    node     *n;
    datanode *p;

    if (q->head->data != NULL) {

        if ((n = malloc(sizeof(datanode))) != NULL) {

            p = q->head;
            q->head = (node *)n; 
            q->head->prev = NULL; 
            p->prev = q->head;
            q->head->next = p;

        } else return(FALSE);

    }

    q->head->data = d; 
    q->size++;
    q->cursor = q->head; 
    q->sorted = FALSE;
    q->head->data = d; 

    return(TRUE);

} 

/**/ 

/*** 
 * 
 ** function    : que_push_tail 
 * 
 ** purpose     : put a data element pointer at the tail of the queue 
 * 
 ** parameters  : queue pointer, pointer to the data 
 * 
 ** returns     : TRUE if data pushed, FALSE if data not inserted. 
 * 
 ** comments    : 
 * 
 ***/ 

int que_push_tail(queue *q, void *d) { 

    node        *p;
    datanode    *n; 

    if (q->tail->data != NULL) {

        if ((n = malloc(sizeof(datanode))) != NULL) {

            p = q->tail;
            q->tail = (node *)n; 
            q->tail->next = NULL; 
            p->next = q->tail;
            q->tail->prev = p;

        } else return(FALSE);

    }

    q->tail->data = d; 
    q->size++; 
    q->cursor = q->tail; 
    q->sorted = FALSE;

    return(TRUE);

} 

/**/ 

/*** 
 * 
 ** function    : que_pop_head 
 * 
 ** purpose     : remove and return the top element at the head of the 
 *                queue. 
 * 
 ** parameters  : queue pointer 
 * 
 ** returns     : pointer to data element or NULL if queue is empty. 
 * 
 ** comments    : 
 * 
 ***/ 

void *que_pop_head(queue *q) { 

    datanode    *n; 
    void        *d; 

    if (que_empty(q)) return(NULL); 

    d = q->head->data; 
    n = q->head->next; 

    free(q->head); 
    q->size--; 

    if (q->size == 0) { 

        q->head = q->tail = q->cursor = NULL; 

    } else { 

        q->head = (node *)n; 
        q->head->prev = NULL; 
        q->cursor = q->head;

    } 

    q->sorted = FALSE; 

    return(d); 

} 

/**/ 

/*** 
 * 
 ** function    : que_pop_tail 
 * 
 ** purpose     : remove element from tail of queue and return data. 
 * 
 ** parameters  : queue pointer 
 * 
 ** returns     : pointer to data element that was at tail. NULL if queue 
 *                empty. 
 * 
 ** comments    : 
 * 
 ***/ 

void *que_pop_tail(queue *q) { 

    datanode    *p; 
    void        *d; 

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

/**/ 

/*** 
 * 
 ** function    : que_next 
 * 
 ** purpose     : Move to the next element in the queue without popping 
 * 
 ** parameters  : queue pointer. 
 * 
 ** returns     : pointer to data element of new element or NULL if end 
 *                of the queue. 
 * 
 ** comments    : This uses the cursor for the current position. que_next 
 *                only moves in the direction from the head of the queue 
 *                to the tail. 
 ***/ 

void *que_next(queue *q) { 

    if (q->cursor->next == NULL) return(NULL); 

    q->cursor = (node *)q->cursor->next; 

    return(q->cursor->data);

} 

/**/ 

/*** 
 * 
 ** function    : que_prev 
 * 
 ** purpose     : Opposite of que_next. Move to next element closer to the 
 *                head of the queue. 
 * 
 ** parameters  : pointer to queue 
 * 
 ** returns     : pointer to data of new element else NULL if queue empty 
 * 
 ** comments    : Makes cursor move towards the head of the queue. 
 * 
 ***/ 

void *que_prev(queue *q) { 

    if (q->cursor->prev == NULL) return(NULL); 

    q->cursor = (node *)q->cursor->prev; 

    return(q->cursor->data);

} 

/**/ 

/*** 
 * 
 ** function    : que_delete 
 * 
 ** purpose     : Delete the current queue element as pointed to by 
 *                the cursor. 
 * 
 ** parameters  : queue pointer 
 * 
 ** returns     : pointer to data element. 
 * 
 ** comments    : WARNING! It is the responsibility of the caller to 
 *                free any memory. Queue cannot distinguish between 
 *                pointers to literals and malloced memory. 
 * 
 ***/ 

void *que_delete(queue *q) { 

    void        *d; 
    datanode    *n, *p, *c; 

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

/**/ 

/*** 
 * 
 ** function    : que_get 
 * 
 ** purpose     : get the pointer to the data at the cursor location 
 * 
 ** parameters  : queue pointer 
 * 
 ** returns     : data element pointer 
 * 
 ** comments    : 
 * 
 ***/ 

void *que_get(queue *q) { 

    if (q->cursor == NULL) return(NULL); 

    return(q->cursor->data);

} 

/**/ 

/*** 
 * 
 ** function    : que_put 
 * 
 ** purpose     : replace pointer to data with new pointer to data. 
 * 
 ** parameters  : queue pointer, data pointer 
 * 
 ** returns     : boolean- TRUE if successful, FALSE if cursor at NULL 
 * 
 ** comments    : 
 * 
 ***/ 

int que_put(queue *q, void *data) { 

    if (q->cursor == NULL) return(FALSE); 

    q->cursor->data = data; 

    return(TRUE);

} 

/**/ 

/*** 
 * 
 ** function    : que_find 
 * 
 ** purpose     : Linear search of queue for match with key in *data 
 * 
 ** parameters  : queue pointer q, data pointer with data containing key 
 *                comparison function here called Comp. 
 * 
 ** returns     : TRUE if found , FALSE if not in queue. 
 * 
 ** comments    : Useful for small queues that are constantly changing 
 *                and would otherwise need constant sorting with the 
 *                que_seek function. 
 *                For description of Comp see que_sort. 
 *                Queue cursor left on position found item else at end. 
 * 
 ***/ 

int que_find(queue *q, void *data, int (*Comp)(const void *, const void *)) { 

    void *d; 

    d = que_first(q); 

    do { 

        if (Comp(d, data) == 0) return(TRUE); 
        d = que_next(q); 

    } while (!que_end(q)); 

    if (Comp(d, data) == 0) return(TRUE); 

    return(FALSE); 

} 

/**/ 

/*========  Sorted Queue and Index functions   ========= */ 

static void QuickSort(void *list[], int low, int high, 
                      int (*Comp)(const void *, const void *)) { 

    int     flag = 1, i, j; 
    void    *key, *temp; 

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

/*** 
 * 
 ** function    : que_sort 
 * 
 ** purpose     : sort the queue and allow index style access. 
 * 
 ** parameters  : queue pointer, comparison function compatible with 
 *                with 'qsort'. 
 * 
 ** returns     : TRUE if sort succeeded. FALSE if error occurred. 
 * 
 ** comments    : Comp function supplied by caller must return 
 *                  -1 if data1  < data2 
 *                   0 if data1 == data2 
 *                  +1 if data1  > data2 
 * 
 *                    for Comp(data1, data2) 
 * 
 * 
 *                If queue is already sorted it frees the memory of the 
 *                old index and starts again. 
 * 
 ***/ 

int que_sort(queue *q, int (*Comp)(const void *, const void *)) { 

    int         i; 
    void        *d; 
    datanode    *dn; 

    /* if already sorted free memory for tag array                         */ 

    if (q->sorted) { 

        free(q->index); 
        free(q->posn_index); 
        q->sorted = FALSE;

    } 

    /* Now allocate memory of array, array of pointers */ 

    q->index = malloc(q->size * sizeof(q->cursor->data)); 
    if (q->index == NULL) return(FALSE); 

    q->posn_index = malloc(q->size * sizeof(q->cursor)); 

    if (q->posn_index == NULL) { 

        free(q->index); 

        return(FALSE); 

    } 

    /* Walk queue putting pointers into array */ 

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

    q->sorted = TRUE; 

    return(TRUE); 

} 

/**/ 

/*** 
 * 
 ** function    : que_bsearch 
 * 
 ** purpose     : binary search of queue index for node containing key 
 * 
 ** parameters  : queue pointer 'q', data pointer of key 'key', 
 *                  Comp comparison function. 
 * 
 ** returns     : integer index into array of node pointers, 
 *                or -1 if not found. 
 * 
 ** comments    : see que_sort for description of 'Comp' function. 
 * 
 ***/ 

static int que_bsearch(queue *q, void *key, 
                      int (*Comp)(const void *, const void*)) { 

    int low, mid, hi, val; 

    low = 0; 
    hi = q->size - 1; 

    while (low <= hi) { 

        mid = (low + hi) / 2; 
        val = Comp(key, q->index[mid]); 

        if (val < 0) { 

            hi = mid - 1; 

        } else if (val > 0) { 

            low = mid + 1; 

        } else return(mid);                /* Success                         */ 

    } 

    /* Not Found */ 

    return(-1); 

} 

/**/ 

/*** 
 * 
 ** function    : que_seek 
 * 
 ** purpose     : use index to locate data according to key in 'data' 
 * 
 ** parameters  : queue pointer 'q', data pointer 'data', Comp comparison 
 *                function. 
 * 
 ** returns     : pointer to data or NULL if could not find it or could 
 *                not sort queue. 
 * 
 ** comments    : see que_sort for description of 'Comp' function. 
 * 
 ***/ 

void *que_seek(queue *q, void *data, int (*Comp)(const void *, const void *)) { 

    int idx; 

    if (!q->sorted) { 

        if (!que_sort(q, Comp)) return NULL;

    } 

    idx = que_bsearch(q, data, Comp); 

    if (idx < 0) return NULL; 

    q->cursor = q->posn_index[idx]; 

    return(q->index[idx]); 

} 

/**/ 

/*** 
 * 
 ** function    : que_insert 
 * 
 ** purpose     : Insert an element into a queue 
 * 
 ** parameters  : queue pointer 'q', data pointer 'data', Comp comparison 
 *                function. 
 * 
 ** returns     : pointer to data or NULL if could not find it or could 
 *                not sort queue. 
 * 
 ** comments    : see que_sort for description of 'Comp' function. 
 *                WARNING! This code can be very slow since each new 
 *                element means a new que_sort.  Should only be used for 
 *                the insertion of the odd element ,not the piecemeal 
 *                building of an entire queue. 
 ***/ 

int que_insert(queue *q, void *d) { 

    node        *p; 
    node        *c;
    node        *n; 
    datanode    *x;

    if (q->cursor == NULL) return(FALSE);
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
        q->sorted = FALSE;

        return(TRUE);

    }

    return(FALSE);

} 


/* +++Date last modified: 05-Jul-1997 */

/*
 *  File : DEQUE.H
 *
 *  Peter Yard  02 Jan 1993.
 */

#ifndef DEQUEUE__H
#define DEQUEUE__H

/*#include "sniptype.h"*/                     /* For True_, False_    */
#ifndef TRUE
#define TRUE  1
#define FALSE 0
#endif

typedef struct nodeptr datanode;

typedef struct nodeptr {
      void        *data ;
      datanode    *prev, *next ;
} node ;

typedef struct {
      node        *head, *tail, *cursor;
      int         size, sorted, item_deleted;
      void        **index; 
      datanode    **posn_index; 
} queue;

typedef  struct {
      void        *dataptr;
      node        *loc ;
} index_elt ;


int    que_init(queue  *q);
int    que_empty(queue *q);
int    que_size(queue *q);
int    que_start(queue *q);
int    que_end(queue *q);
int    que_push_head(queue *q, void *d);
int    que_push_tail(queue *q, void *d);
void  *que_first(queue *q);
void  *que_last(queue *q);
void  *que_pop_head(queue *q);
void  *que_pop_tail(queue *q);
void  *que_next(queue *q);
void  *que_prev(queue *q);
void  *que_delete(queue *q);
void  *que_get(queue *q);
int    que_put(queue *q, void *data);
int    que_insert(queue *q, void *data);
int    que_sort(queue *q, int (*Comp)(const void *, const void *));
int    que_find(queue *q, void *data, int (*Comp)(const void *, const void *));
void  *que_seek(queue *q, void *data, int (*Comp)(const void *, const void *));

#endif /* DEQUEUE__H */


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                                                           */
/* TITLE:       que_util.h                                                   */
/*                                                                           */
/* FACILITY:    Global variables and structures for the QUE package.         */
/*                                                                           */
/* ABSTRACT:    These are the declarations of the data structures and        */
/*              routines to implement the QUE utilities.                     */
/*                                                                           */
/* ENVIRONMENT: VMS/AIX/Linux                                                */
/*                                                                           */
/* Version      Author                                           Date        */
/* -------      ---------------------------------------------    ----------- */
/* 1-000        Kevin Esteb                                      23-Apr-2002 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef QUE_UTIL_H
#define QUE_UTIL_H 1

#ifdef __cplusplus
    extern  "C" {
#endif

#include  "gpl/ansi_setup.h"        /* ANSI or non-ANSI C?              */

/*----------------------------------------------------------------------*/
/*    QUE Structures and Definitions.                                   */
/*----------------------------------------------------------------------*/

typedef struct nodeptr datanode;

typedef struct nodeptr {
    void     *data;
    datanode *prev;
    datanode *next;
} node;

typedef struct {
    node     *head;
    node     *tail;
    node     *cursor;
    int       size;
    int       sorted;
    int       item_deleted;
    void     **index; 
    datanode **posn_index; 
} queue;

typedef  struct {
    void  *dataptr;
    node  *loc;
} index_elt;

/*----------------------------------------------------------------------*/
/*    Miscellaneous declarations.                                       */
/*----------------------------------------------------------------------*/

typedef int (comp_proc) P_((void *a, void *b));

#define QUE_OK     0
#define QUE_ERROR -1

/*----------------------------------------------------------------------*/
/*    Public functions.                                                 */
/*----------------------------------------------------------------------*/

extern int    que_init P_((queue  *q));

extern int    que_empty P_((queue *q));

extern int    que_size P_((queue *q));

extern int    que_start P_((queue *q));

extern int    que_end P_((queue *q));

extern int    que_push_head P_((queue *q, void *d));

extern int    que_push_tail P_((queue *q, void *d));

extern void  *que_first P_((queue *q));

extern void  *que_last P_((queue *q));

extern void  *que_pop_head P_((queue *q));

extern void  *que_pop_tail P_((queue *q));

extern void  *que_next P_((queue *q));

extern void  *que_prev P_((queue *q));

extern void  *que_delete P_((queue *q));

extern void  *que_get P_((queue *q));

extern int    que_put P_((queue *q, void *d));

extern int    que_insert P_((queue *q, void *d));

extern int    que_sort P_((queue *q, comp_proc *Comp));

extern int    que_find P_((queue *q, void *data, comp_proc *Comp));

extern void  *que_seek P_((queue *q, void *data, comp_proc *Comp));

#ifdef __cplusplus
    }
#endif

#endif


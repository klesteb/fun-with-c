
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2019 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#ifndef _WORKER_H
#define _WORKER_H

#include "objects/object.h"
#include "include/item_list.h"

/*----------------------------------------------------------------*/
/* klass defination                                               */
/*----------------------------------------------------------------*/

typedef struct _worker_s worker_t;

struct _worker_s {
    object_t parent_klass;
    int   (*ctor)(object_t *, item_list_t *);
    int   (*dtor)(object_t *);
    int   (*_compare)(worker_t *, worker_t *);
    int   (*_override)(worker_t *, item_list_t *);
    int   (*_match)(worker_t *, int);
    void *(*_perform)(worker_t *, void *, int);
    int type;
};

/*-------------------------------------------------------------*/
/* constants                                                   */
/*-------------------------------------------------------------*/

#define WORKER_OBJECT(x) ((worker_t *)(x))

#define WORKER_K_TYPE 1

#define WORKER_M_DESTRUCTOR 1
#define WORKER_M_PERFORM    2
#define WORKER_M_MATCH      3

/*-------------------------------------------------------------*/
/* interface                                                   */
/*-------------------------------------------------------------*/

extern worker_t *worker_create(item_list_t *);
extern int worker_destroy(worker_t *);
extern int worker_match(worker_t *, int);
extern int worker_compare(worker_t *, worker_t *);
extern void *worker_perform(worker_t *, void *, int);
extern int worker_override(worker_t *, item_list_t *);

#endif



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

#ifndef _WORKBENCH_H
#define _WORKBENCH_H

#include "events.h"
#include "object.h"
#include "que_util.h"
#include "item_list.h"

/*-------------------------------------------------------------*/
/* klass defination                                            */
/*-------------------------------------------------------------*/

typedef struct _workbench_s workbench_t;

struct _workbench_s {
    object_t parent_klass;
    int (*ctor)(object_t *, item_list_t *);
    int (*dtor)(object_t *);
    int (*_compare)(workbench_t *, workbench_t *);
    int (*_override)(workbench_t *, item_list_t *);
    int (*_loop)(workbench_t *);
    int (*_event)(workbench_t *, event_t *);
    queue events;
    queue containers;
};

/*-------------------------------------------------------------*/
/* constants                                                   */
/*-------------------------------------------------------------*/

#define WORKBENCH(x) ((workbench_t *)(x))

#define WORKBENCH_M_DESTRUCTOR 1

/*-------------------------------------------------------------*/
/* interface                                                   */
/*-------------------------------------------------------------*/

extern workbench_t *workbench_create(item_list_t *);
extern int workbench_loop(workbench_t *);
extern int workbench_destroy(workbench_t *);
extern int workbench_compare(workbench_t *, workbench_t *);
extern int workbench_override(workbench_t *, item_list_t *);

#endif

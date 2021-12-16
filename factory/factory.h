
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

#ifndef _FACTORY_H
#define _FACTORY_H

#include "cclibs/que_util.h"
#include "worker.h"

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

typedef struct _factory_s factory_t;

struct _factory_s {
    object_t parent_klass;
    int   (*ctor)(object_t *, item_list_t *);
    int   (*dtor)(object_t *);
    int   (*_add)(factory_t *, worker_t *);
    int   (*_remove)(factory_t *, worker_t *);
    int   (*_override)(factory_t *, item_list_t *);
    void *(*_dispatch)(factory_t *, int, void *, int);
    queue workers;
};

/*-------------------------------------------------------------*/
/* constants                                                   */
/*-------------------------------------------------------------*/

#define FACTORY_OBJECT(x) ((factory_t *)(x))

/*-------------------------------------------------------------*/
/* interface                                                   */
/*-------------------------------------------------------------*/

extern factory_t *factory_create();
extern int factory_destroy(factory_t *);
extern int factory_add(factory_t *, worker_t *);
extern int factory_remove(factory_t *, worker_t *);
extern int factory_override(factory_t *, item_list_t *);
extern void *factory_dispatch(factory_t *, int, void *, int);

#endif


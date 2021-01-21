
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2021 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that this copyright notice appears in all copies. The author    */
/*  makes no representations about the suitability of this software for      */
/*  any purpose. It is provided "as is" without express or implied warranty. */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef _HANDLER_H
#define _HANDLER_H

#include "room.h"
#include "files.h"
#include "tracer.h"
#include "object.h"
#include "item_list.h"

/*-------------------------------------------------------------*/
/* klass defination                                            */
/*-------------------------------------------------------------*/

typedef struct _handler_s handler_t;

struct _handler_s {
    object_t parent_klass;
    int (*ctor)(object_t *, item_list_t *);
    int (*dtor)(object_t *);
    int (*_compare)(handler_t *, handler_t *);
    int (*_override)(handler_t *, item_list_t *);
    int (*_init)(handler_t *);
    int (*_remove)(handler_t *);
    int (*_detach)(handler_t *);
    int (*_attach)(handler_t *, room_base_t *);
    int (*_handle)(handler_t *, void **);

    int base;
    int timeout;
    int retries;
    char *path;
    void *handle;
    files_t *db;
    tracer_t *trace;
};

/*-------------------------------------------------------------*/
/* klass constants                                             */
/*-------------------------------------------------------------*/

#define HANDLER(x) ((handler_t *)(x))

#define HANDLER_K_TRACE   1
#define HANDLER_K_DB      2
#define HANDLER_K_PATH    3
#define HANDLER_K_RETRIES 4
#define HANDLER_K_TIMEOUT 5
#define HANDLER_K_BASE    6

#define HANDLER_M_DESTRUCTOR 1
#define HANDLER_M_INIT       2
#define HANDLER_M_ATTACH     3
#define HANDLER_M_DETACH     4
#define HANDLER_M_REMOVE     5
#define HANDLER_M_HANDLE     6

/*-------------------------------------------------------------*/
/* klass interface                                             */
/*-------------------------------------------------------------*/

extern handler_t *handler_create(files_t *, char *, int, int, int, tracer_t *);
extern int handler_destroy(handler_t *);
extern int handler_compare(handler_t *, handler_t *);
extern int handler_override(handler_t *, item_list_t *);
extern char *handler_version(handler_t *);

extern int handler_init(handler_t *);
extern int handler_detach(handler_t *);
extern int handler_remove(handler_t *);
extern int handler_handle(handler_t *, void **);
extern int handler_attach(handler_t *, room_base_t *);

#endif


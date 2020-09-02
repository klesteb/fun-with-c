
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2020 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#ifndef _TRACE_H
#define _TRACE_H

#include "object.h"
#include "errors.h"
#include "que_util.h"
#include "item_list.h"
#include "error_trace.h"

/*-------------------------------------------------------------*/
/* klass defination                                            */
/*-------------------------------------------------------------*/

typedef struct _trace_s trace_t;

struct _trace_s {
    object_t parent_klass;
    int (*ctor)(object_t *, item_list_t *);
    int (*dtor)(object_t *);
    int (*_compare)(trace_t *, trace_t *);
    int (*_override)(trace_t *, item_list_t *);
    int (*_add)(trace_t *, error_trace_t *);
    int (*_dump)(trace_t *, int (*output)(char *));
    errors_t *errs;
    queue errors;
};

/*-------------------------------------------------------------*/
/* constants                                                   */
/*-------------------------------------------------------------*/

#define TRACE(x) ((trace_t *)(x))

#define TRACE_M_DESTRUCTOR 1

/*-------------------------------------------------------------*/
/* interface                                                   */
/*-------------------------------------------------------------*/

extern trace_t *trace_create(item_list_t *);
extern int trace_destroy(trace_t *);
extern int trace_compare(trace_t *, trace_t *);
extern int trace_override(trace_t *, item_list_t *);
extern int trace_add(trace_t *, error_trace_t *);
extern int trace_dump(trace_t *, int (*output)(char *));

#endif



/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2021 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#ifndef _EVENT_H_
#define _EVENT_H_

#include "object.h"
#include "nix_util.h"
#include "que_util.h"
#include "item_list.h"

/*----------------------------------------------------------------*/
/* constants                                                      */
/*----------------------------------------------------------------*/

#define EV_INPUT  1
#define EV_WORKER 2
#define EV_TIMER  3

/*----------------------------------------------------------------*/
/* data structures                                                */
/*----------------------------------------------------------------*/

typedef struct _event_handler_s {
    int type;
    NxInputId input_id;
    NxWorkProcId worker_id;
    NxIntervalId timer_id;
} event_handler_t;

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

typedef struct _event_s event_t;

struct _event_s {
    object_t parent_klass;
    int (*ctor)(object_t *, item_list_t *);
    int (*dtor)(object_t *);
    int (*_compare)(event_t *, event_t *);
    int (*_override)(event_t *, item_list_t *);

    int (*_loop)(event_t *);
    int (*_break)(event_t *);
    int (*_register_input)(event_t *, int , int (*input)(void *), void *);
    int (*_register_worker)(event_t *, int , int (*input)(void *), void *);
    int (*_register_timer)(event_t *, int, double, int (*input)(void *), void *);

    int broken;
    queue handlers;
};

/*-------------------------------------------------------------*/
/* klass constants                                             */
/*-------------------------------------------------------------*/

#define EVENT(x) ((event_t *)(x))

#define EVENT_M_DESTRUCTOR 1

/*----------------------------------------------------------------*/
/* interface                                                      */
/*----------------------------------------------------------------*/

extern event_t *event_create(void);
extern int event_destroy(event_t *);
extern int event_compare(event_t *, event_t *);
extern int event_override(event_t *, item_list_t *);
extern char *event_version(event_t *);

extern int event_loop(event_t *);
extern int event_break(event_t *);
extern int event_register_input(event_t *, int, int (*input)(void *), void *);
extern int event_register_worker(event_t *, int, int (*input)(void *), void *);
extern int event_register_timer(event_t *, int, double, int (*input)(void *), void *);

#endif


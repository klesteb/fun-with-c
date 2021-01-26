
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

#ifndef _EVENTS_H_
#define _EVENTS_H_

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

typedef struct _events_handler_s {
    int type;
    NxInputId input_id;
    NxWorkProcId worker_id;
    NxIntervalId timer_id;
} events_handler_t;


/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

typedef struct _events_s events_t;

struct _events_s {
    object_t parent_klass;
    int (*ctor)(object_t *, item_list_t *);
    int (*dtor)(object_t *);
    int (*_compare)(events_t *, events_t *);
    int (*_override)(events_t *, item_list_t *);

    int (*_loop)(events_t *);
    int (*_register_input)(events_t *, int , int (*input)(void *), void *);
    int (*_register_worker)(events_t *, int , int (*input)(void *), void *);
    int (*_register_timer)(events_t *, int, double, int (*input)(void *), void *);
    
    queue handlers;
};

/*-------------------------------------------------------------*/
/* klass constants                                             */
/*-------------------------------------------------------------*/

#define EVENTS(x) ((events_t *)(x))

#define EVENTS_M_DESTRUCTOR 1

/*----------------------------------------------------------------*/
/* interface                                                      */
/*----------------------------------------------------------------*/

extern events_t *events_create();
extern int events_destroy(events_t *);
extern int events_compare(events_t *, events_t *);
extern int events_override(events_t *, item_list_t *);
extern char *events_version(events_t *);

extern int events_loop(events_t *);
extern int events_register_input(events_t *, int, int (*input)(void *), void *);
extern int events_register_worker(events_t *, int, int (*input)(void *), void *);
extern int events_register_timer(events_t *, int, double, int (*input)(void *), void *);

#endif


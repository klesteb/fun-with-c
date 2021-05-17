
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

#ifndef _WORKBENCH_H
#define _WORKBENCH_H

#include <panel.h>

#include "events.h"
#include "object.h"
#include "window.h"
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
    int (*_read_stdin)(workbench_t *);
    int (*_init_terminal)(workbench_t *);
    int (*_dispatch)(workbench_t *, int *);
    int (*_set_menu)(workbench_t *, window_t *);
    int (*_get_focus)(workbench_t *, window_t *);
    int (*_set_focus)(workbench_t *, window_t *);
    int (*_queue_event)(workbench_t *, events_t *);

    int (*_draw)(workbench_t *);
    int (*_erase)(workbench_t *);
    int (*_add)(workbench_t *, window_t *);
    int (*_event)(workbench_t *, events_t *);
    int (*_remove)(workbench_t *, window_t *);

    int panels;
    window_t *main;
    PANEL *panel;
    WINDOW *messages;
    queue events;
};

/*-------------------------------------------------------------*/
/* klass constants                                             */
/*-------------------------------------------------------------*/

#define WORKBENCH(x) ((workbench_t *)(x))

#define WORKBENCH_M_DESTROY       1
#define WORKBENCH_M_DESTRUCTOR    1
#define WORKBENCH_M_ADD           2
#define WORKBENCH_M_DRAW          3
#define WORKBENCH_M_EVENT         4
#define WORKBENCH_M_ERASE         5
#define WORKBENCH_M_REMOVE        6
#define WORKBENCH_M_SET_FOCUS     7
#define WORKBENCH_M_GET_FOCUS     8
#define WORKBENCH_M_INIT_TERMINAL 9
#define WORKBENCH_M_READ_STDIN    10
#define WORKBENCH_M_DISPATCH      11

/*-------------------------------------------------------------*/
/* klass interface                                             */
/*-------------------------------------------------------------*/

extern workbench_t *workbench_create(item_list_t *);
extern int workbench_destroy(workbench_t *);
extern int workbench_compare(workbench_t *, workbench_t *);
extern int workbench_override(workbench_t *, item_list_t *);

extern int workbench_capture(workbench_t *);
extern int workbench_dispatch(workbench_t *, int *);
extern int workbench_set_menu(workbench_t *, window_t *);
extern int workbench_get_focus(workbench_t *, window_t *);
extern int workbench_set_focus(workbench_t *, window_t *);
extern int workbench_inject_event(workbench_t *, events_t *);

extern int workbench_refresh(workbench_t *);
extern int workbench_add(workbench_t *, window_t *);
extern int workbench_remove(workbench_t *, window_t *);
extern int workbench_remove_async(workbench_t *, window_t *);

#endif


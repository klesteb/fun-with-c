
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
    int (*_draw)(workbench_t *);
    int (*_loop)(workbench_t *);
    int (*_refresh)(workbench_t *);
    int (*_event)(workbench_t *, event_t *);
    int (*_get_focus)(workbench_t *, window_t *);
    int (*_set_focus)(workbench_t *, window_t *);
    int (*_add_window)(workbench_t *, window_t *);
    int (*_inject_event)(workbench_t *, event_t *);
    int (*_remove_window)(workbench_t *, window_t *);
    int panels;
    PANEL *panel;
    queue events;
};

/*-------------------------------------------------------------*/
/* constants                                                   */
/*-------------------------------------------------------------*/

#define WORKBENCH(x) ((workbench_t *)(x))

#define WORKBENCH_M_DRAW             1
#define WORKBENCH_M_EVENT            2
#define WORKBENCH_M_REFRESH          3
#define WORKBENCH_M_DESTROY          4
#define WORKBENCH_M_DESTRUCTOR       4
#define WORKBENCH_M_ADD_WINDOW       5
#define WORKBENCH_M_REMOVE_WINDOW    6
#define WORKBENCH_M_SET_FOCUS        7
#define WORKBENCH_M_GET_FOCUS        8
#define WORKBENCH_M_LOOP             9
#define WORKBENCH_M_INJECT_EVENT     10

/*-------------------------------------------------------------*/
/* interface                                                   */
/*-------------------------------------------------------------*/

extern workbench_t *workbench_create(item_list_t *);
extern int workbench_loop(workbench_t *);
extern int workbench_destroy(workbench_t *);
extern int workbench_get_focus(workbench_t *, window_t *);
extern int workbench_set_focus(workbench_t *, window_t *);
extern int workbench_compare(workbench_t *, workbench_t *);
extern int workbench_add_window(workbench_t *, window_t *);
extern int workbench_override(workbench_t *, item_list_t *);
extern int workbench_inject_event(workbench_t *, event_t *);
extern int workbench_remove_window(workbench_t *, window_t *);

#endif


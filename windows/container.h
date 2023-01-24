
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

#ifndef _CONTAINER_H
#define _CONTAINER_H

#include <ncurses.h>

#include "object.h"
#include "component.h"
#include "item_list.h"
#include "que_util.h"

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

typedef struct _container_s container_t;

struct _container_s {
    object_t parent_klass;
    int (*ctor)(object_t *, item_list_t *);
    int (*dtor)(object_t *);
    int (*_draw)(container_t *);
    int (*_erase)(container_t *);
    int (*_refresh)(container_t *);
    int (*_event)(container_t *, events_t *);
    int (*_compare)(container_t *, container_t *);
    int (*_override)(container_t *, item_list_t *);
    int (*_add_component)(container_t *, component_t *);
    int (*_remove_component)(container_t *, component_t *);
    int row;
    int col;
    int type;
    int width;
    int height;
    void *focus;
    WINDOW *area;
    queue components;
    void *data;
};

/*----------------------------------------------------------------*/
/* constants                                                      */
/*----------------------------------------------------------------*/

#define CONTAINER(x) ((container_t *)(x))

#define CONTAINER_K_WIDTH  1
#define CONTAINER_K_HEIGHT 2
#define CONTAINER_K_ROW    3
#define CONTAINER_K_COL    4

#define CONTAINER_M_DRAW             1
#define CONTAINER_M_EVENT            2
#define CONTAINER_M_ERASE            3
#define CONTAINER_M_DESTROY          4
#define CONTAINER_M_REFRESH          5
#define CONTAINER_M_ADD_COMPONENT    6
#define CONTAINER_M_REMOVE_COMPONENT 7

#define CONTAINER_T_HORIZONTAL 1
#define CONTAINER_T_VERTICAL   2
#define CONTAINER_T_SCROLLABLE 3
#define CONTAINER_T_FORM       4
#define CONTAINER_T_MENU       5

/*----------------------------------------------------------------*/
/* interface                                                      */
/*----------------------------------------------------------------*/

extern container_t *container_create(int, int, int, int);
extern int container_draw(container_t *);
extern int container_erase(container_t *);
extern int container_refresh(container_t *);
extern int container_destroy(container_t *);
extern int container_event(container_t *, events_t *);
extern int container_compare(container_t *, container_t *);
extern int container_override(container_t *, item_list_t *);
extern int container_add_component(container_t *, component_t *);
extern int container_remove_component(container_t *, component_t *);

#endif


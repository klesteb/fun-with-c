
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

#ifndef _CONTAINER_H
#define _CONTAINER_H

#include <ncurses.h>

#include "window.h"

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

typedef struct _container_s container_t;

struct _container_s {
    widget_t parent_klass;
    int (*ctor)(object_t *, item_list_t *);
    int (*dtor)(object_t *);
    int (*_compare)(container_t *, container_t *);
    int (*_override)(container_t *, item_list_t *);

    int type;
    WINDOW *area;
    window_t *window;
};

/*----------------------------------------------------------------*/
/* klass constants                                                */
/*----------------------------------------------------------------*/

#define CONTAINER(x) ((container_t *)(x))

#define CONTAINER_K_WINDOW 6

#define CONTAINER_T_HORIZONTAL 1
#define CONTAINER_T_VERTICAL   2
#define CONTAINER_T_SCROLLABLE 3
#define CONTAINER_T_FORM       4
#define CONTAINER_T_MENU       5

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

extern container_t *container_create(window_t *, int, int, int, int);
extern int container_destroy(container_t *);
extern int container_compare(container_t *, container_t *);

#define container_draw(self) widget_draw(WIDGET(self))
#define container_erase(self) widget_erase(WIDGET(self))
#define container_refresh(self) widget_refresh(WIDGET(self))
#define container_event(self, event) widget_event(WIDGET(self), event);
#define container_add(self, thing) widget_add(WIDGET(self), (void *)thing)
#define container_override(self, items) widget_override(WIDGET(self), items)
#define container_get_theme(self, theme) widget_get_theme(WIDGET(self), theme)
#define container_set_theme(self, theme) widget_set_theme(WIDGET(self), theme)
#define container_remove(self, thing) widget_remove(WIDGET(self), (void *)thing)
#define container_get_coordinates(self, coordinates) widget_get_coordinates(WIDGET(self), coordinates)
#define container_set_coordinates(self, coordinates) widget_set_coordinates(WIDGET(self), coordinates)

#endif


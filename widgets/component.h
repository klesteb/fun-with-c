
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

#ifndef _COMPONENT_H
#define _COMPONENT_H

#include <ncurses.h>

#include "widgets/widget.h"
#include "widgets/window.h"

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

typedef struct _component_s component_t;

struct _component_s {
    widget_t parent_klass;
    int (*ctor)(object_t *, item_list_t *);
    int (*dtor)(object_t *);
    int (*_compare)(component_t *, component_t *);
    int (*_override)(component_t *, item_list_t *);

    int tab;
    int padding;
    int data_size;
    void *data;
    window_t *parent;
    WINDOW *area;
};

/*----------------------------------------------------------------*/
/* klass constants                                                */
/*----------------------------------------------------------------*/

#define COMPONENT(x) ((component_t *)(x))

#define COMPONENT_K_WINDOW  5
#define COMPONENT_K_TAB     6
#define COMPONENT_K_DATA    7
#define COMPONENT_K_PADDING 8

#define KEY_IILEGAL (KEY_MAX - 1)

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

extern component_t *component_create(window_t *, int, int, int, int, int, int, void *, int);
extern int component_destroy(component_t *);
extern int component_compare(component_t *, component_t *); 
extern int component_set_data(component_t *, void *);
extern int component_get_data(component_t *, void *, int);
extern int component_set_tab(component_t *, int);
extern int component_get_tab(component_t *, int *);

#define component_draw(self) widget_draw(WIDGET(self))
#define component_erase(self) widget_erase(WIDGET(self))
#define component_refresh(self) widget_refresh(WIDGET(self))
#define component_event(self, event) widget_event(WIDGET(self), event)
#define component_add(self, thing) widget_add(WIDGET(self), (void *)thing)
#define component_override(self, items) widget_override(WIDGET(self), items)
#define component_get_theme(self, theme) widget_get_theme(WIDGET(self), theme)
#define component_set_theme(self, theme) widget_set_theme(WIDGET(self), theme)
#define component_get_coordinates(self, coordinates) widget_get_coordinates(WIDGET(self), coordinates)
#define component_set_coordinates(self, coordinates) widget_set_coordinates(WIDGET(self), coordinates)
#define component_remove(self, thing) widget_remove(WIDGET(self), (void *)thing)

#endif


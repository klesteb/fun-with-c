
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

#ifndef _BELL_H
#define _BELL_H

#include <ncurses.h>

#include "widget.h"
#include "window.h"

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

typedef struct _bell_s bell_t;

struct _bell_s {
    widget_t parent_klass;
    int (*ctor)(object_t *, item_list_t *);
    int (*dtor)(object_t *);
    int (*_compare)(bell_t *, bell_t *);
    int (*_override)(bell_t *, item_list_t *);

    int times;
};

/*----------------------------------------------------------------*/
/* klass constants                                                */
/*----------------------------------------------------------------*/

#define BELL(x) ((bell_t *)(x))

#define BELL_K_TIMES 10

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

extern bell_t *bell_create(window_t *, int);
extern int bell_destroy(bell_t *);
extern int bell_compare(bell_t *, bell_t *); 

#define bell_draw(self) widget_draw(WIDGET(self))
#define bell_erase(self) widget_erase(WIDGET(self))
#define bell_refresh(self) widget_refresh(WIDGET(self))
#define bell_event(self, event) widget_event(WIDGET(self), event);
#define bell_add(self, thing) widget_add(WIDGET(self), (void *)thing)
#define bell_override(self, items); widget_override(WIDGET(self), items)
#define bell_get_theme(self, theme) widget_get_theme(WIDGET(self), theme)
#define bell_set_theme(self, theme) widget_set_theme(WIDGET(self), theme)
#define bell_get_coordinates(self, coordinates) widget_get_coordinates(WIDGET(self), coordinates)
#define bell_set_coordinates(self, coordinates) widget_set_coordinates(WIDGET(self), coordinates)
#define bell_remove(self, thing) widget_remove(WIDGET(self), (void *)thing)

#endif


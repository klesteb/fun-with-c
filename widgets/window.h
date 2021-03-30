
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

#ifndef _WINDOW_H
#define _WINDOW_H

#include <ncurses.h>

#include "widget.h"

/*-------------------------------------------------------------*/
/* klass defination                                            */
/*-------------------------------------------------------------*/

typedef struct _window_s window_t;

struct _window_s {
    widget_t parent_klass;
    int (*ctor)(object_t *, item_list_t *);
    int (*dtor)(object_t *);
    int (*_compare)(window_t *, window_t *);
    int (*_override)(window_t *, item_list_t *);
    int (*_box)(window_t *);

    int tab;
    int tabs;
    int boxed;
    char *title;
    WINDOW *outer;
    WINDOW *inner;
};

/*-------------------------------------------------------------*/
/* klass constants                                             */
/*-------------------------------------------------------------*/

#define WINDOW(x) ((window_t *)(x))

#define WINDOW_K_TITLE 3
#define WINDOW_K_BOXED 4

/*-------------------------------------------------------------*/
/* klass interface                                             */
/*-------------------------------------------------------------*/

extern window_t *window_create(char *, int, int, int, int, int);
extern int window_destroy(window_t *);
extern int window_compare(window_t *, window_t *);
extern int window_set_tab(window_t *, int);
extern int window_get_tab(window_t *, int *);

#define window_draw(self) widget_draw(WIDGET(self))
#define window_erase(self) widget_erase(WIDGET(self))
#define window_refresh(self) widget_refresh(WIDGET(self))
#define window_event(self, event) widget_event(WIDGET(self), event);
#define window_add(self, thing) widget_add(WIDGET(self), (void *)thing)
#define window_override(self, items) widget_override(WIDGET(self), items)
#define window_get_theme(self, theme) widget_get_theme(WIDGET(self), theme)
#define window_set_theme(self, theme) widget_set_theme(WIDGET(self), theme)
#define window_remove(self, thing) widget_remove(WIDGET(self), (void *)thing)
#define window_get_padding(self, padding) widget_get_padding(WIDGET(self), padding)
#define window_set_padding(self, padding) widget_set_padding(WIDGET(self), padding)
#define window_get_coordinates(self, coordinates) widget_get_coordinates(WIDGET(self), coordinates)
#define window_set_coordinates(self, coordinates) widget_set_coordinates(WIDGET(self), coordinates)

#endif


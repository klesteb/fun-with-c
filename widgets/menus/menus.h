
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

#ifndef _MENUS_H
#define _MENUS_H

#include <ncurses.h>
#include <menu.h>

#include "widget.h"
#include "menus_list.h"
#include "menus_priv.h"

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

typedef struct _menus_s menus_t;

struct _menus_s {
    widget_t parent_klass;
    int (*ctor)(object_t *, item_list_t *);
    int (*dtor)(object_t *);
    int (*_compare)(menus_t *, menus_t *);
    int (*_override)(menus_t *, item_list_t *);
    int (*_show_description)(menus_t *, int (*inject)(events_t *));

    int items_count;
    char *title;
    ITEM *focus;
    ITEM **items;
    WINDOW *outer;
    WINDOW *inner;
    menus_data_t *data;
};

/*----------------------------------------------------------------*/
/* klass constants                                                */
/*----------------------------------------------------------------*/

#define MENUS(x) ((menus_t *)(x))

#define MENUS_K_TITLE 4
#define MENUS_K_LIST  5

#define MENUS_M_SHOW_DESCRIPTION 8

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

extern menus_t *menus_create(char *, int, int, int, int, menus_list_t *);
extern int menus_destroy(menus_t *);
extern int menus_compare(menus_t *, menus_t *);
extern int menus_override(menus_t *, item_list_t *);

#define menus_draw(self) widget_draw(WIDGET(self))
#define menus_erase(self) widget_erase(WIDGET(self))
#define menus_refresh(self) widget_refresh(WIDGET(self))
#define menus_event(self, event) widget_event(WIDGET(self), event);
#define menus_add(self, thing) widget_add(WIDGET(self), (void *)thing)
#define menus_get_theme(self, theme) widget_get_theme(WIDGET(self), theme)
#define menus_set_theme(self, theme) widget_set_theme(WIDGET(self), theme)
#define menus_remove(self, thing) widget_remove(WIDGET(self), (void *)thing)
#define menus_get_coordinates(self, coordinates) widget_get_coordinates(WIDGET(self), coordinates)
#define menus_set_coordinates(self, coordinates) widget_set_coordinates(WIDGET(self), coordinates)

#endif


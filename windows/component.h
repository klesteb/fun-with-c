
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

#ifndef _COMPONENT_H
#define _COMPONENT_H

#include <ncurses.h>

#include "object.h"
#include "events.h"
#include "item_list.h"

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

typedef struct _component_s component_t;

struct _component_s {
    object_t parent_klass;
    int (*ctor)(object_t *, item_list_t *);
    int (*dtor)(object_t *);
    int (*_draw)(component_t *);
    int (*_erase)(component_t *);
    int (*_refresh)(component_t *);
    int (*_event)(component_t *, event_t *);
    int (*_compare)(component_t *, component_t *);
    int (*_override)(component_t *, item_list_t *);
    int bg;
    int fg;
    int col;
    int row;
    int size;
    int type;
    int focus;
    int width;
    int height;
    int padding;
    int attribute;
    void *data;
    WINDOW *area;
};

/*----------------------------------------------------------------*/
/* constants                                                      */
/*----------------------------------------------------------------*/

#define COMPONENT(x) ((component_t *)(x))

#define COMPONENT_K_WIDTH      1
#define COMPONENT_K_HEIGHT     2
#define COMPONENT_K_DATA       3
#define COMPONENT_K_FOREGROUND 4
#define COMPONENT_K_BACKGROUND 5

#define COMPONENT_M_DRAW    1
#define COMPONENT_M_EVENT   2
#define COMPONENT_M_ERASE   3
#define COMPONENT_M_REFRESH 4
#define COMPONENT_M_DESTROY 5

#define COMPONENT_T_TEXT          1
#define COMPONENT_T_HLINE         2
#define COMPONENT_T_VLINE         3
#define COMPONENT_T_LABEL         5
#define COMPONENT_T_MESSAGE       4
#define COMPONENT_T_TEXTAREA      5
#define COMPONENT_T_INPUT         6
#define COMPONENT_T_FORM_FIELD    7
#define COMPONENT_T_FORM_LABEL    8
#define COMPONENT_T_FORM_HEADER   9
#define COMPONENT_T_FORM_TEXTAREA 10
#define COMPONENT_T_FORM_HLINE    11
#define COMPONENT_T_MENU          12
#define COMPONENT_T_MENU_ITEM     13
#define COMPONENT_T_PROMPT        14
#define COMPONENT_T_INKEY         15

#define KEY_IILEGAL (KEY_MAX - 1)

/*----------------------------------------------------------------*/
/* interface                                                      */
/*----------------------------------------------------------------*/

extern component_t *component_create(int, int, void *, int);
extern int component_draw(component_t *);
extern int component_erase(component_t *);
extern int component_destroy(component_t *);
extern int component_refresh(component_t *);
extern int component_event(component_t *, event_t *);
extern int component_set_padding(component_t *, int);
extern int component_set_data(component_t *, void *);
extern int component_get_padding(component_t *, int *);
extern int component_set_area(component_t *, WINDOW *);
extern int component_get_area(component_t *, WINDOW *);
extern int component_set_attribute(component_t *, int);
extern int component_get_attribute(component_t *, int *);
extern int component_set_colors(component_t *, int, int);
extern int component_get_data(component_t *, void *, int);
extern int component_set_position(component_t *, int, int);
extern int component_compare(component_t *, component_t *); 
extern int component_override(component_t *, item_list_t *);
extern int component_get_colors(component_t *, int *, int *);
extern int component_get_position(component_t *, int *, int *);

#endif


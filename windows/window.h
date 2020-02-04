
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

#ifndef _WINDOW_H
#define _WINDOW_H

#include <ncurses.h>

#include "object.h"
#include "container.h"
#include "item_list.h"
#include "que_util.h"
#include "events.h"

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

typedef struct _window_s window_t;

struct _window_s {
    object_t parent_klass;
    int (*ctor)(object_t *, item_list_t *);
    int (*dtor)(object_t *);
    int (*_draw)(window_t *);
    int (*_erase)(window_t *);
    int (*_refresh)(window_t *);
    int (*_event)(window_t *, event_t *);
    int (*_compare)(window_t *, window_t *);
    int (*_override)(window_t *, item_list_t *);
    int (*_add_container)(window_t *, container_t *);
    int (*_remove_container)(window_t *, container_t *);
    int fg;
    int bg;
    int row;
    int col;
    int boxed;
    int width;
    int height;
    char *title;
    int attribute;
    WINDOW *outer;
    WINDOW *inner;
    queue containers;
};

/*----------------------------------------------------------------*/
/* constants                                                      */
/*----------------------------------------------------------------*/

#define WINDOW(x) ((window_t *)(x))

#define WINDOW_K_WIDTH  1
#define WINDOW_K_HEIGHT 2
#define WINDOW_K_ROW    3
#define WINDOW_K_COL    4

/*----------------------------------------------------------------*/
/* interface                                                      */
/*----------------------------------------------------------------*/

extern window_t *window_create(int, int, int, int);
extern int window_draw(window_t *);
extern int window_erase(window_t *);
extern int window_clear(window_t *);
extern int window_destroy(window_t *);
extern int window_refresh(window_t *);
extern int window_box(window_t *, char *);
extern int window_event(window_t *, event_t *);
extern int window_compare(window_t *, window_t *);
extern int window_set_colors(window_t *, int, int);
extern int window_override(window_t *, item_list_t *);
extern int window_get_colors(window_t *, int *, int *);
extern int window_add_container(window_t *, container_t *);
extern int window_output(window_t *, int, int, char *, ...);
extern int window_remove_container(window_t *, container_t *);

#endif


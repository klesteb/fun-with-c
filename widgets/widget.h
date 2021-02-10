
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

#ifndef _WIDGET_H
#define _WIDGET_H

#include "theme.h"
#include "events.h"
#include "object.h"
#include "que_util.h"
#include "item_list.h"
#include "coordinates.h"

/*-------------------------------------------------------------*/
/* klass defination                                            */
/*-------------------------------------------------------------*/

typedef struct _widget_s widget_t;

struct _widget_s {
    object_t parent_klass;
    int (*ctor)(object_t *, item_list_t *);
    int (*dtor)(object_t *);
    int (*_compare)(widget_t *, widget_t *);
    int (*_override)(widget_t *, item_list_t *);
    int (*_draw)(widget_t *);
    int (*_erase)(widget_t *);
    int (*_add)(widget_t *, void *);
    int (*_remove)(widget_t *, void *);
    int (*_event)(widget_t *, events_t *);

    coordinates_t *coordinates;
    theme_t *theme;
    queue things;
};

/*-------------------------------------------------------------*/
/* klass constants                                             */
/*-------------------------------------------------------------*/

#define WIDGET(x) ((widget_t *)(x))

#define WIDGET_K_COORDINATES 1
#define WIDGET_K_THEME       2

#define WIDGET_M_DESTRUCTOR 1
#define WIDGET_M_ADD        2
#define WIDGET_M_DRAW       3
#define WIDGET_M_ERASE      4
#define WIDGET_M_EVENT      5
#define WIDGET_M_REMOVE     6

/*-------------------------------------------------------------*/
/* klass interface                                             */
/*-------------------------------------------------------------*/

extern widget_t *widget_create(item_list_t *);
extern int widget_destroy(widget_t *);
extern int widget_compare(widget_t *, widget_t *);
extern int widget_override(widget_t *, item_list_t *);

extern int widget_draw(widget_t *);
extern int widget_erase(widget_t *);
extern int widget_refresh(widget_t *);
extern int widget_add(widget_t *, void *);
extern int widget_remove(widget_t *, void *);
extern int widget_event(widget_t *, events_t *);
extern int widget_get_theme(widget_t *, theme_t *);
extern int widget_set_theme(widget_t *, theme_t *);
extern int widget_get_coordinates(widget_t *, coordinates_t *);
extern int widget_set_coordinates(widget_t *, coordinates_t *);

#endif


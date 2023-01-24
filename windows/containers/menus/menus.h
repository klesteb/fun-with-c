
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2020 by Kevin L. Esteb                       */
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

#include "container.h"

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

typedef struct _menus_s menus_t;

struct _menus_s {
    container_t parent_klass;
    int (*ctor)(object_t *, item_list_t *);
    int (*dtor)(object_t *);
    int (*_compare)(menus_t *, menus_t *);
    int (*_override)(menus_t *, item_list_t *);
    int (*_show_description)(container_t *);
};

/*----------------------------------------------------------------*/
/* constants                                                      */
/*----------------------------------------------------------------*/

#define MENUS(x) ((menus_t *)(x))

#define MENUS_M_SHOW_DESCRIPTION 8

/*----------------------------------------------------------------*/
/* interface                                                      */
/*----------------------------------------------------------------*/

extern menus_t *menus_create(int, int, int, int);
extern int menus_draw(menus_t *);
extern int menus_erase(menus_t *);
extern int menus_refresh(menus_t *);
extern int menus_destroy(menus_t *);
extern int menus_show_description(menus_t *);
extern int menus_event(menus_t *, events_t *);
extern int menus_compare(menus_t *, menus_t *);
extern int menus_override(menus_t *, item_list_t *);
extern int menus_add_component(menus_t *, component_t *);
extern int menus_remove_component(menus_t *, component_t *);

#endif



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

#include <menu.h>

#include "component.h"

/*----------------------------------------------------------------*/
/*                                                                */
/*----------------------------------------------------------------*/

typedef struct _item_data_s {
    int index;
    char *label;
    char *description;
    ITEM *item;
} item_data_t;

/*----------------------------------------------------------------*/
/* constants                                                      */
/*----------------------------------------------------------------*/

/*----------------------------------------------------------------*/
/* interface                                                      */
/*----------------------------------------------------------------*/

extern component_t *menu_item_create(item_data_t *, int);

#endif



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

#include "widgets/window.h"
#include "widgets/component.h"

/*----------------------------------------------------------------*/

/* data structure to define a menu item */

typedef struct _menus_list_s {
    char *label;
    char *description;
    void *data;
    int type;
    int data_size;
    int (*callback)(void *, int, error_trace_t *);
} menus_list_t;

/* macro to easily set up the structure */

#define SET_MENU(x,a,b,c,d,e,f) {\
    (x).label=strdup((a));\
    (x).description=strdup((b));\
    (x).type=(c);\
    (x).data=(d);\
    (x).data_size=(e);\
    (x).callback=(f);\
}

/* menu item types */

#define MENUS_T_ITEM      1
#define MENUS_T_SEPERATOR 2

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

extern component_t *menus_create(window_t *, int, int, int, int, int, menus_list_t *items, int size);

#endif


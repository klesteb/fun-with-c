
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

#ifndef _MENUS_PRIV_H
#define _MENUS_PRIV_H

#include <menu.h>

#include "include/error_trace.h"

#include "widgets/widget.h"

/*----------------------------------------------------------------*/
/* private data structure                                         */
/*----------------------------------------------------------------*/

typedef struct _menus_data_s {
    WINDOW *subwin;
    MENU *menu;
    ITEM **items;
    ITEM *focus;
    Menu_Options options;
    char *mark;
    int row;
    int col;
    int items_count;
    int (*show_description)(widget_t *);
    int (*callback)(const char *, error_trace_t *);
} menus_data_t;

typedef struct _userptr_data_s {
        int (*callback)(void *, int, error_trace_t *);
        int data_size;
        void *data;
} userptr_data_t;

#endif


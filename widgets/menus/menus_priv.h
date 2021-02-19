
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

#ifndef _MENU_PRIV_H
#define _MENU_PRIV_H

#include <menu.h>

/*----------------------------------------------------------------*/
/* private data                                                   */
/*----------------------------------------------------------------*/

typedef struct _menus_data_s {
    MENU *menu;
    ITEM **items;
    Menu_Options options;
    char *mark;
    int row;
    int col;
    int (*callback)(widget_t *);
} menus_data_t;

typedef struct _userptr_data_s {
        int (*callback)(void *, int);
        int data_size;
        void *data;
} userptr_data_t;

#endif


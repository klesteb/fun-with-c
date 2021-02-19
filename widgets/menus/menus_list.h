
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

#ifndef _MENUS_LIST_H
#define _MENUS_LIST_H

#include "data_types.h"

typedef struct _menus_list_s {
    char *label;
    char *description;
    void *data;
    int data_size;
    int (*callback)(void *, int);
} menus_list_t;

/* macro to easily set up the structure */

#define SET_MENU(x,a,b,c,d,e) {\
    (x).label=(a);\
    (x).description(b);\
    (x).data=(c);\
    (x).data_size=(d);\
    (x).callback=(e);\
}

#endif



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

#ifndef _ITEM_LIST_H
#define _ITEM_LIST_H

#include "data_types.h"

typedef struct _item_list_s {
    UWORD buffer_length;
    UWORD item_code;
    void *buffer_address;
    void *return_address;
} item_list_t;

#define SET_ITEM(x,a,b,c,d) {(x).item_code=(a);\
                             (x).buffer_address=(b);\
                             (x).buffer_length=(c);\
                             (x).return_address=(d);}
#endif


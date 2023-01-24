
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

#ifndef _INKEY_H
#define _INKEY_H

#include "component.h"

/*----------------------------------------------------------------*/
/* data structures                                                */
/*----------------------------------------------------------------*/

typedef struct _inkey_func_s {
    int keycode;
    int (*callback)(void *);
    void *data;
} inkey_funcs_t;

/*----------------------------------------------------------------*/
/* interface                                                      */
/*----------------------------------------------------------------*/

extern component_t *inkey_create(inkey_funcs_t *);

#endif


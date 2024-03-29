
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

#ifndef _LIST_MENU_H
#define _LIST_MENU_H

#include "include/error_trace.h"

#include "widgets/window.h"
#include "widgets/components/menus/menus.h"

/*----------------------------------------------------------------*/
/* interface                                                      */
/*----------------------------------------------------------------*/

extern component_t *list_menu_create(window_t *, int, int, int, int, int, int (*display)(const char *, error_trace_t *), menus_list_t *, int);

#endif


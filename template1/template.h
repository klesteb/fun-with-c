
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

#ifndef _TEMPLATE_H
#define _TEMPLATE_H

#include "object.h"
#include "item_list.h"
#include "template_klass.h"

/*-------------------------------------------------------------*/
/* interface                                                   */
/*-------------------------------------------------------------*/

extern template_t *template_create(item_list_t *);
extern int template_destroy(template_t *);
extern int template_compare(template_t *, template_t *);
extern int template_override(template_t *, item_list_t *);
extern char *template_version(template_t *);

#endif


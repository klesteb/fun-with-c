
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

#ifndef _FORMS_H
#define _FORMS_H

#include <form.h>

#include "component.h"

/*----------------------------------------------------------------*/
/*                                                                */
/*----------------------------------------------------------------*/

typedef struct _field_data_s {
    int col;
    int row;
    int width;
    int height;
    int required;
    char *value;
    FIELD *field;
} field_data_t;

/*----------------------------------------------------------------*/
/* constants                                                      */
/*----------------------------------------------------------------*/

/*----------------------------------------------------------------*/
/* interface                                                      */
/*----------------------------------------------------------------*/

extern component_t *form_field_create(field_data_t *, int);
extern component_t *form_label_create(field_data_t *, int);
extern component_t *form_header_create(field_data_t *, int);
extern component_t *form_textarea_create(field_data_t *, int);
extern component_t *form_seperator_create(field_data_t *, int);

extern int form_field_results(component_t *, void *, int);

#endif


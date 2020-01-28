
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

#include <ncurses.h>
#include <form.h>

#include "component.h"
#include "item_list.h"
#include "colors.h"
#include "forms.h"

require_klass(COMPONENT_KLASS);

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

extern int _form_field_dtor(object_t *);

int _form_label_draw(component_t *self) {

    int width = 0;
    int stat = OK;
    int count = 0;
    char *value = NULL;
    field_data_t *data = NULL;

    if (self->data != NULL) {

        data = (field_data_t *)self->data;

        if ((value = calloc(1, data->width + 1))) {

            width = data->width - 1;
            count = (width < strlen(data->value)) ? width : strlen(data->value);

            if (self->padding) {

                memset(value, '.', width);

            } else {

                memset(value, ' ', width);

            }

            strcat(value, ":");
            memcpy(value, data->value, count);

            set_field_back(data->field, self->bg);
            set_field_fore(data->field, self->fg);

            field_opts_off(data->field, O_ACTIVE);
            field_opts_off(data->field, O_EDIT);

            set_field_just(data->field, JUSTIFY_LEFT);
            set_field_buffer(data->field, 0, value);

            free(value);

        }

        self->data = (void *)data;

    }

    return stat;

}

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

component_t *form_label_create(field_data_t *data) {

    FIELD *temp = NULL;
    item_list_t items[3];
    component_t *self = NULL;

    if ((self = component_create(data->height, data->width, NULL, 0))) {

        self->type = COMPONENT_T_FORM_LABEL;

        temp = new_field(data->height, data->width, data->row, data->col, 0, 0);

        if (temp != NULL) {

            data->field = temp;
            self->data = (void *)data;
            self->size = sizeof(field_data_t);

        }

        SET_ITEM(items[0], COMPONENT_M_DESTROY, &_form_field_dtor, 0, NULL);
        SET_ITEM(items[1], COMPONENT_M_DRAW, &_form_label_draw, 0, NULL);
        SET_ITEM(items[2], 0, 0, 0, 0);

        component_override(self, items);
        
    }

    return self;

}



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

#include "component.h"
#include "item_list.h"
#include "colors.h"

require_klass(COMPONENT_KLASS);

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

int _label_draw(component_t *self) {

    int stat = ERR;
    char *value = NULL;

    if ((value = calloc(1, self->width + 1))) {

        int width = self->width - 1;
        int count = (self->size > self->width) ? width : self->size;

        if (self->padding) {

            memset(value, '.', width);

        } else {

            memset(value, ' ', width);

        }

        strcat(value, ":");
        memcpy(value, self->data, count);

        wattron(self->area, self->attribute);
        wcoloron(self->area, self->fg, self->bg);
        mvwaddstr(self->area, self->row, self->col, value);
        wstandend(self->area);

        free(value);

        stat = OK;

    }

    return stat;

}

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

component_t *label_create(int width, char *value, int size) {

    item_list_t items[2];
    component_t *label = NULL;

    if ((label = component_create(1, width, (void *)value, size))) {

        label->type = COMPONENT_T_LABEL;

        SET_ITEM(items[0], COMPONENT_M_DRAW, &_label_draw, 0, NULL);
        SET_ITEM(items[1], 0, 0, 0, 0);

        component_override(label, items);

    }

    return label;

}


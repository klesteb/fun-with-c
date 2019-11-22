
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

#include "object.h"
#include "container.h"
#include "component.h"
#include "item_list.h"

require_klass(CONTAINER_KLASS);

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

int _vertical_draw(container_t *self) {

    int row = 0;
    int col = 0;
    int stat = OK;
    component_t *temp = NULL;

    for (temp = que_first(&self->components);
         temp != NULL;
         temp = que_next(&self->components)) {

        component_set_area(temp, self->area);
        component_set_metrics(temp, row, col);

        if ((stat = component_draw(temp)) != OK) {

            break;

        }

        row++;

    }

    return stat;

}

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

container_t *vertical_create(int row, int col, int height, int width) {

    item_list_t items[2];
    container_t *vertical = NULL;

    if ((vertical = container_create(row, col, height, width))) {

        vertical->type = CONTAINER_T_VERTICAL;

        SET_ITEM(items[0], CONTAINER_M_DRAW, &_vertical_draw, 0, NULL);
        SET_ITEM(items[1], 0, 0, 0, 0);

        container_override(vertical, items);

    }

    return vertical;

}


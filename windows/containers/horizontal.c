
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

int _horizontal_draw(container_t *self) {

    int row = 0;
    int col = 0;
    int stat = OK;
    component_t *temp = NULL;

    for (temp = que_first(&self->components);
         temp != NULL;
         temp = que_next(&self->components)) {

        if ((col + temp->width) > self->width) {

            row++;
            col = 1;

        }

        component_set_area(temp, self->area);
        component_set_metrics(temp, row, col);

        if ((stat = component_draw(temp)) != OK) {

            break;

        }

        col += (temp->width + 1);

    }

    return stat;

}

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

container_t *horizontal_create(int row, int col, int height, int width) {

    item_list_t items[2];
    container_t *horizontal = NULL;

    if ((horizontal = container_create(row, col, height, width))) {

        horizontal->type = CONTAINER_T_HORIZONTAL;

        SET_ITEM(items[0], CONTAINER_M_DRAW, &_horizontal_draw, 0, NULL);
        SET_ITEM(items[1], 0, 0, 0, 0);

        container_override(horizontal, items);

    }

    return horizontal;

}


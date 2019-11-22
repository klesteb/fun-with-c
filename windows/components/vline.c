
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

int _vline_draw(component_t *self) {

    int stat = OK;

    wattron(self->area, self->attribute);
    wattron(self->area, setcolor(self->fg, self->bg));

    mvwvline(self->area, self->row, self->col, ACS_VLINE, self->height);

    wstandend(self->area);

    return stat;

}

int _vline_refresh(component_t *self) {

    int stat = ERR;

    if ((stat == component_erase(self)) == OK) {

        stat = component_draw(self);

    }

    return stat;

}

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

component_t *vline_create(int height) {

    item_list_t items[3];
    component_t *vline = NULL;

    if ((vline = component_create(1, height, NULL, 0))) {

        vline->type = COMPONENT_T_VLINE;

        SET_ITEM(items[0], COMPONENT_M_DRAW, &_vline_draw, 0, NULL);
        SET_ITEM(items[1], COMPONENT_M_REFRESH, &_vline_refresh, 0, NULL);
        SET_ITEM(items[2], 0, 0, 0, 0);

        component_override(vline, items);

    }

    return vline;

}


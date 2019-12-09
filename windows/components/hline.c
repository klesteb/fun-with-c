
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

int _hline_draw(component_t *self) {

    int stat = OK;

    wattron(self->area, self->attribute);
    wsetcolor(self->area, self->fg, self->bg);
    mvwhline(self->area, self->row, self->col, ACS_HLINE, self->width);
    wstandend(self->area);

    return stat;

}

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

component_t *hline_create(int width) {

    item_list_t items[2];
    component_t *hline = NULL;

    if ((hline = component_create(1, width, NULL, 0))) {

        hline->type = COMPONENT_T_HLINE;

        SET_ITEM(items[0], COMPONENT_M_DRAW, &_hline_draw, 0, NULL);
        SET_ITEM(items[1], 0, 0, 0, 0);

        component_override(hline, items);

    }

    return hline;

}


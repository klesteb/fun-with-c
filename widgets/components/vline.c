
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

#include <ncurses.h>
#include <errno.h>

#include "when.h"
#include "colors.h"
#include "widget.h"
#include "component.h"
#include "item_list.h"

require_klass(COMPONENT_KLASS);

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

int _vline_draw(widget_t *widget) {

    int stat = OK;
    component_t *self = COMPONENT(widget);

    when_error_in {

        errno = 0;
        stat = wattron(self->window->inner, widget->theme->attribute);
        check_status(stat, OK, errno);

        errno = 0;
        stat = wcoloron(self->window->inner, 
                        widget->theme->foreground, widget->theme->background);
        check_status(stat, OK, errno);

        errno = 0;
        stat = mvwvline(self->window->inner, 
                        widget->coordinates->startx, 
                        widget->coordinates->starty,
                        ACS_VLINE, widget->coordinates->height);
        check_status(stat, OK, errno);

        errno = 0;
        stat = wstandend(self->window->inner);
        check_status(stat, OK, errno);

        exit_when;

    } use {

        stat = ERR;
        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

component_t *vline_create(window_t *window, int startx, int starty, int height) {

    int tab = 0;
    int width = 1;
    int padding = FALSE;
    item_list_t items[2];
    component_t *vline = NULL;

    if ((vline = component_create(window, startx, starty, height, width, tab, padding, NULL, 0))) {

        SET_ITEM(items[0], WIDGET_M_DRAW, _vline_draw, 0, NULL);
        SET_ITEM(items[1], 0, 0, 0, 0);

        component_override(vline, items);

    }

    return vline;

}


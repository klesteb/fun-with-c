
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
#include "component.h"
#include "item_list.h"
#include "error_codes.h"

require_klass(COMPONENT_KLASS);

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

int _text_draw(widget_t *widget) {

    int len = 0;
    int stat = OK;
    char *value = NULL;
    component_t *self = COMPONENT(widget);

    when_error_in {

        errno = 0;
        if ((value = calloc(1, widget->coordinates->width)) == NULL) {

            cause_error(errno);

        }

        len = (self->data_size < (widget->coordinates->width - 1))
            ? self->data_size
            : widget->coordinates->width - 1;

        memcpy(value, (char *)self->data, len);

        stat = wattron(self->window->inner, widget->theme->attribute);
        check_status(stat, OK, E_INVOPS);

        stat = wcoloron(self->window->inner, 
                        widget->theme->foreground, widget->theme->background);
        check_status(stat, OK, E_INVOPS);

        stat = mvwaddstr(self->window->inner, 
                         widget->coordinates->startx, 
                         widget->coordinates->starty, value);
        check_status(stat, OK, E_INVOPS);

        stat = wstandend(self->window->inner);
        check_status(stat, OK, E_INVOPS);

        free(value);

        exit_when;

    } use {

        stat = ERR;
        object_set_error2(widget, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

component_t *text_create(window_t *window, int startx, int starty, int width, char *value, int size) {

    int tab = 0;
    int height = 1;
    int padding = FALSE;
    item_list_t items[2];
    component_t *text = NULL;

    if ((text = component_create(window, startx, starty, height, width, tab, padding, (void *)value, size))) {

        SET_ITEM(items[0], WIDGET_M_DRAW, _text_draw, 0, NULL);
        SET_ITEM(items[1], 0, 0, 0, 0);

        component_override(text, items);

    }

    return text;

}


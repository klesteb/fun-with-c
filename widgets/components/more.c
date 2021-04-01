
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
#include <menu.h>
#include <errno.h>

#include "when.h"
#include "more.h"
#include "colors.h"
#include "que_util.h"
#include "component.h"
#include "item_list.h"
#include "misc/misc.h"
#include "error_codes.h"
#include "error_trace.h"

require_klass(COMPONENT_KLASS);

/*----------------------------------------------------------------*/
/* private methods                                                */
/*----------------------------------------------------------------*/

typedef struct _more_data_s {
    int rows;
    int cols;
    int last_line;
    int current_pad_row;
    int visible_pad_rows;
    WINDOW *pad;
} more_data_t;

/*----------------------------------------------------------------*/
/* private methods                                                */
/*----------------------------------------------------------------*/

int _more_data_create(component_t *more, char *text, int text_size) {

    queue lines;
    int stat = OK;
    int vrows = 0;
    int vcols = 0;
    char *line = NULL;
    more_data_t *data = NULL;

    when_error_in {

        getmaxyx(more->area, vrows, vcols);

        errno = 0;
        data = calloc(1, sizeof(more_data_t));
        if (data == NULL) cause_error(errno);

        stat = que_init(&lines);
        check_status(stat, QUE_OK, E_INVOPS);

        stat = wordwrap(text, vcols, &lines);
        check_status(stat, OK, E_INVOPS);

        data->rows = vrows;
        data->cols = vcols;
        data->current_pad_row = 0;
        data->visible_pad_rows = vrows - 4;
        data->last_line = que_size(&lines);

        data->pad = newpad(data->last_line, vcols);
        if (data->pad == NULL) cause_error(E_INVOPS);

        while ((line = que_pop_head(&lines))) {

            wprintw(data->pad, "%s", line);
            free(line);

        }

        more->data = (void *)data;

        exit_when;

    } use {

        stat = ERR;
        object_set_error2(more, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

int _more_dtor(object_t *object) {

    int stat = OK;
    more_data_t *data = NULL;
    component_t *self = COMPONENT(object);

    /* free local resources here */

    if (self->data != NULL) {

        data = (more_data_t *)self->data;

        werase(data->pad);
        delwin(data->pad);

        free(data);
        self->data = NULL;

    }

    if (self->area != NULL) {

        werase(self->area);
        delwin(self->area);

        self->area = NULL;

    }

    /* walk the chain, freeing as we go */

    object_demote(object, object_t);
    object_destroy(object);

    return stat;

}

int _more_add(widget_t *widget, void *data) {

    return OK;

}

int _more_draw(widget_t *widget) {

    int stat = OK;
    more_data_t *data = NULL;
    component_t *self = COMPONENT(widget);

    when_error_in {

        if (self->data != NULL) {

            data = (more_data_t *)self->data;

            stat = prefresh(data->pad, data->current_pad_row, 0, 2, 0, data->rows - 3 , data->cols - 1);
            check_status(stat, OK, E_INVOPS);

        }

        exit_when;

    } use {

        stat = ERR;
        object_set_error2(widget, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int _more_erase(widget_t *widget) {

    int stat = OK;
    component_t *self = COMPONENT(widget);

    when_error_in {

        if (self->area) {

            stat = werase(self->area);
            check_status(stat, OK, E_INVOPS);

        }

        exit_when;

    } use {

        stat = ERR;
        object_set_error2(widget, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int _more_event(widget_t *widget, events_t *event) {

    int stat = OK;
    more_data_t *data = NULL;
    component_t *self = COMPONENT(widget);

    when_error_in {

        if (self->data != NULL) {

            data = (more_data_t *)self->data;

            if (event->type == EVENT_K_KEYBOARD) {

                KEVENT *key = (KEVENT *)event->data;
            
                switch(key->keycode) {
                    case KEY_NPAGE: // page down
                        data->current_pad_row += data->visible_pad_rows;
                        if (data->current_pad_row + data->visible_pad_rows > data->last_line) {
                            data->current_pad_row = data->last_line - data->visible_pad_rows;
                        }
                        break;
                    case KEY_PPAGE: // page up
                        data->current_pad_row -= data->visible_pad_rows;
                        if (data->current_pad_row < 0) {
                            data->current_pad_row = 0;
                        }
                        break;
                    case KEY_UP:
                        data->current_pad_row--;
                        if (data->current_pad_row < 0) {
                            data->current_pad_row = 0;
                        }
                        break;
                    case KEY_DOWN:
                        data->current_pad_row++;
                        if (data->current_pad_row + data->visible_pad_rows > data->last_line) {
                            data->current_pad_row = data->last_line - data->visible_pad_rows;
                        }
                        break;
                }
                    
                if (data->current_pad_row + data->visible_pad_rows > data->last_line) {

                    data->current_pad_row = data->last_line - data->visible_pad_rows;

                } else if (data->current_pad_row < 0) {

                    data->current_pad_row = 0;

                }
            }
            
            COMPONENT(widget)->data = data;

        }

        exit_when;

    } use {

        stat = ERR;
        object_set_error2(widget, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int _more_remove(widget_t *widget, void *thing) {

    return OK;

}

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

component_t *more_create(window_t *window, int startx, int starty, int height, int width, int tab, char *text, int text_size) {

    int stat = OK;
    int padding = FALSE;
    item_list_t items[7];
    component_t *more = NULL;

    when_error_in {

        more = component_create(window, startx, starty, height, width, tab, padding, NULL, 0);
        check_creation(more);

        stat = _more_data_create(more, text, text_size);
        check_return(stat, more);

        SET_ITEM(items[0], WIDGET_M_ADD, _more_add, 0, NULL);
        SET_ITEM(items[1], WIDGET_M_DRAW, _more_draw, 0, NULL);
        SET_ITEM(items[2], WIDGET_M_EVENT, _more_event, 0, NULL);
        SET_ITEM(items[3], WIDGET_M_ERASE, _more_erase, 0, NULL);
        SET_ITEM(items[4], WIDGET_M_DESTROY, _more_dtor, 0, NULL);
        SET_ITEM(items[5], WIDGET_M_REMOVE, _more_remove, 0, NULL);
        SET_ITEM(items[6], 0, 0, 0, 0);

        stat = component_override(more, items);
        check_return(stat, more);

        exit_when;

    } use {

        object_set_error2(more, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return more;

}


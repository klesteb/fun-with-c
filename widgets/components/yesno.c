
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

#include "include/when.h"
#include "include/item_list.h"
#include "include/error_codes.h"

#include "widgets/colors.h"
#include "widgets/component.h"

require_klass(COMPONENT_KLASS);

/*----------------------------------------------------------------*/
/* private data                                                   */
/*----------------------------------------------------------------*/

typedef struct _yesno_data_s {
    int (*callback)(int, error_trace_t *);
    char *label;
} yesno_data_t;

/*----------------------------------------------------------------*/
/* private methods                                                */
/*----------------------------------------------------------------*/

static int _yesno_data_create(component_t *self, char *label, int (*callback)(int, error_trace_t *)) {

    int stat = OK;
    yesno_data_t *data = NULL;

    when_error_in {

        errno = 0;
        data = calloc(1, sizeof(yesno_data_t));
        if (data == NULL) cause_error(errno);

        data->callback = callback;
        data->label = strdup(label);

        self->data = (void *)data;

        exit_when;

    } use {

        stat = ERR;
        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

int _yesno_dtor(object_t *object) {

    int stat = OK;
    yesno_data_t *data = NULL;
    component_t *self = COMPONENT(object);

    /* free local resources here */

    if (self->data != NULL) {

        data = (yesno_data_t *)self->data;

        free(data->label);
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

int _yesno_event(widget_t *widget, events_t *event) {

    int stat = OK;
    error_trace_t error;
    yesno_data_t *data = NULL;
    component_t *self = COMPONENT(widget);

    when_error_in {

        if (self->data != NULL) {

            data = (yesno_data_t *)self->data;
            
            if (event->type == EVENT_K_KEYBOARD) {

                KEVENT *key = (KEVENT *)event->data;

                switch(key->keycode) {
                    case KEY_F(7): { 
                        if (data->callback != NULL) {
                            stat = (*data->callback)(TRUE, &error);
                            check_status2(stat, OK, error);
                        }
                        break;
                    }
                    case KEY_F(8): {
                        if (data->callback != NULL) {
                            stat = (*data->callback)(FALSE, &error);
                            check_status2(stat, OK, error);
                        }
                        break;
                    }
                }

            }

        }

        exit_when;

    } use {

        stat = ERR;
        object_set_error2(widget, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int _yesno_draw(widget_t *widget) {

    int len = 0;
    int stat = OK;
    yesno_data_t *data = NULL;
    component_t *self = COMPONENT(widget);

    when_error_in {

        if (self->data != NULL) {

            data = (yesno_data_t *)self->data;
            len = strlen(data->label);

            stat = wattron(self->area, widget->theme->attribute);
            check_status(stat, OK, E_INVOPS);

            stat = wcoloron(self->area, 
                            widget->theme->foreground, widget->theme->background);
            check_status(stat, OK, E_INVOPS);

            stat = mvwaddnstr(self->area, 0, 0, data->label, len);
            check_status(stat, OK, E_INVOPS);

            stat = wstandend(self->area);
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

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

component_t *yesno_create(window_t *window, int startx, int starty, int height, int width, int tab, char *label, int (*callback)(int, error_trace_t *)) {

    int stat = OK;
    int padding = FALSE;
    item_list_t items[4];
    component_t *yesno = NULL;

    when_error_in {

        yesno = component_create(window, startx, starty, height, width, tab, padding, NULL, 0);
        check_creation(yesno);

        stat = _yesno_data_create(yesno, label, callback);
        check_return(stat, yesno);

        SET_ITEM(items[0], WIDGET_M_DRAW, _yesno_draw, 0, NULL);
        SET_ITEM(items[1], WIDGET_M_EVENT, _yesno_event, 0, NULL);
        SET_ITEM(items[2], WIDGET_M_DESTROY, _yesno_dtor, 0, NULL);
        SET_ITEM(items[3], 0, 0, 0, 0);

        stat = component_override(yesno, items);
        check_return(stat, yesno);

        exit_when;

    } use {

        object_set_error2(yesno, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return yesno;

}


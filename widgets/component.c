
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
#include "container.h"
#include "component.h"
#include "error_codes.h"

require_klass(WIDGET_KLASS);

/*----------------------------------------------------------------*/
/* klass defination                                               */
/*----------------------------------------------------------------*/

int _component_ctor(object_t *object, item_list_t *);
int _component_dtor(object_t *);
int _component_compare(component_t *, component_t *);

int _component_draw(widget_t *);
int _component_erase(widget_t *);
int _component_add(widget_t *, void *);
int _component_remove(widget_t *, void *);
int _component_event(widget_t *, events_t *);

declare_klass(COMPONENT_KLASS) {
    .size = KLASS_SIZE(component_t),
    .name = KLASS_NAME(component_t),
    .ctor = _component_ctor,
    .dtor = _component_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

component_t *component_create(window_t *window, int startx, int starty, int height, int width, int tab, void *data, int size) {

    int stat = ERR;
    item_list_t items[5];
    coordinates_t coords;
    component_t *self = NULL;

    coords.startx = startx;
    coords.starty = starty;
    coords.height = height;
    coords.width  = width;

    SET_ITEM(items[0], WIDGET_K_COORDINATES, &coords, sizeof(coordinates_t), NULL);
    SET_ITEM(items[1], COMPONENT_K_WINDOW, window, 0, NULL);
    SET_ITEM(items[2], COMPONENT_K_TAB, &tab, sizeof(int), NULL);

    if (data != NULL) {

        SET_ITEM(items[3], COMPONENT_K_DATA, data, size, NULL);
        SET_ITEM(items[4], 0, 0, 0, 0);

    } else {

        SET_ITEM(items[3], 0, 0, 0, 0);
        SET_ITEM(items[4], 0, 0, 0, 0);

    }

    self = (component_t *)object_create(COMPONENT_KLASS, items, &stat);

    return self;

}

int component_destroy(component_t *self) {

    int stat = OK;

    when_error {

        if (self != NULL) {

            if (object_assert(self, component_t)) {

                stat = self->dtor(OBJECT(self));
                check_status(stat, OK, E_INVOPS);

            } else {

                cause_error(E_INVOBJ);

            }

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int component_compare(component_t *us, component_t *them) { 

    int stat = OK;

    when_error {

        if ((us != NULL) && (them != NULL)) {

            stat = us->_compare(us, them);
            check_status(stat, OK, E_NOTSAME);

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(us, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int component_set_data(component_t *self, void *data) {

    int stat = OK;

    when_error {

        if ((self != NULL) && (data != NULL)) {

            self->data = data;

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int component_get_data(component_t *self, void *data, int size) {

    int stat = OK;

    when_error {

        if ((self != NULL) && (data != NULL)) {

            errno = 0;
            if ((memcpy(data, self->data, size) == NULL)) {

                cause_error(errno);

            }

        } else {

            cause_error(E_INVPARM);

        }

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

int _component_ctor(object_t *object, item_list_t *items) {

    int tab = 0;
    int stat = ERR;
    void *value = NULL;
    int value_size = 0;
    window_t *window = NULL;
    component_t *self = NULL;

    if (object != NULL) {

        stat = OK;

        when_error_in {

            /* initilize the base klass here */

            stat = WIDGET_KLASS->ctor(object, items);
            check_return(stat, object);

            WIDGET(object)->_add = _component_add;
            WIDGET(object)->_draw = _component_draw;
            WIDGET(object)->_erase = _component_erase;
            WIDGET(object)->_event = _component_event;
            WIDGET(object)->_remove = _component_remove;

            /* capture our items */

            if (items != NULL) {

                int x;
                for (x = 0;; x++) {

                    if ((items[x].buffer_length == 0) &&
                        (items[x].item_code == 0)) break;

                    switch(items[x].item_code) {
                        case COMPONENT_K_DATA: {
                            value_size = items[x].buffer_length;
                            value = items[x].buffer_address;
                            break;
                        }
                        case COMPONENT_K_WINDOW: {
                            window = items[x].buffer_address;
                            break;
                        }
                        case COMPONENT_K_TAB: {
                            memcpy(&tab, 
                                   items[x].buffer_address,
                                   items[x].buffer_length);
                            break;
                        }
                    }

                }

            }

            /* initialize our derived klass here */

            self = COMPONENT(object);

            /* assign our methods here */

            self->ctor = _component_ctor;
            self->dtor = _component_dtor;
            self->_compare = _component_compare;

            /* initialize internal variables here */

            self->tab = tab;
            self->data = value;
            self->window = window;
            self->data_size = value_size;

            exit_when;

        } use {

            stat = ERR;
            object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
            clear_error();

        } end_when;

    }

    return stat;

}

int _component_dtor(object_t *object) {

    int stat = OK;
    widget_t *widget = WIDGET(object);

    /* free local resources here */

    widget->_erase(widget);

    /* walk the chain, freeing as we go */

    object_demote(object, widget_t);
    widget_destroy(WIDGET(object));

    return stat;

}

int _component_compare(component_t *us, component_t *them) {

    int stat = ERR;

    if (((object_compare(OBJECT(us), OBJECT(them))) == OK) &&
        ((widget_compare(WIDGET(us), WIDGET(them)) == OK) &&
         (us->ctor == them->ctor) &&
         (us->dtor == them->dtor) &&
         (us->data == them->data) &&
         (us->tab == them->tab) &&
         (us->window == them->window) &&
         (us->data_size == them->data_size))) {

        stat = OK;

    }

    return stat;

}

int _component_add(widget_t *widget, void *thing) {

    /* not applicable */

    return OK;

}

int _component_draw(widget_t *widget) {

    /* must be overridden in a derived klass */

    return OK;

}

int _component_erase(widget_t *widget) {

    int stat = OK;
    component_t *self = COMPONENT(widget);

    when_error_in {

        if (self->window != NULL) {

            errno = 0;
            stat = wmove(self->window->inner, widget->coordinates->startx, widget->coordinates->starty);
            check_status(stat, OK, errno);

            errno = 0;
            stat = wclrtoeol(self->window->inner);
            check_status(stat, OK, errno);

        }

        exit_when;

    } use {

        stat = ERR;
        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int _component_event(widget_t *widget, events_t *event) {

    /* must be overridden in a derived klass */

    return OK;

}

int _component_remove(widget_t *widget, void *thing) {

    /* not applicable */

    return OK;

}

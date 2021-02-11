
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
#include "events.h"
#include "widget.h"
#include "object.h"
#include "container.h"
#include "component.h"
#include "item_list.h"
#include "coordinates.h"
#include "error_codes.h"

require_klass(WIDGET_KLASS);

/*----------------------------------------------------------------*/
/* klass defination                                               */
/*----------------------------------------------------------------*/

int _container_ctor(object_t *object, item_list_t *);
int _container_dtor(object_t *);
int _container_compare(container_t *, container_t *);
int _container_override(container_t *, item_list_t *);

int _container_draw(widget_t *);
int _container_erase(widget_t *);
int _container_remove(widget_t *, void *);
int _container_event(widget_t *, events_t *);

declare_klass(CONTAINER_KLASS) {
    .size = KLASS_SIZE(container_t),
    .name = KLASS_NAME(container_t),
    .ctor = _container_ctor,
    .dtor = _container_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

container_t *container_create(window_t *window, int startx, int starty, int height, int width) {

    int stat = ERR;
    coordinates_t coords;
    item_list_t items[3];
    container_t *self = NULL;

    coords.startx = startx;
    coords.starty = starty;
    coords.height = height;
    coords.width  = width;

    SET_ITEM(items[0], WIDGET_K_COORDINATES, &coords, sizeof(coordinates_t), NULL);
    SET_ITEM(items[1], CONTAINER_K_WINDOW, window, 0, NULL);
    SET_ITEM(items[2], 0, 0, 0, 0);

    self = (container_t *)object_create(CONTAINER_KLASS, items, &stat);

    return self;

}

int container_destroy(container_t *self) {

    int stat = OK;

    when_error {

        if (self != NULL) {

            if (object_assert(self, container_t)) {

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

int container_compare(container_t *us, container_t *them) { 

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

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

int _container_ctor(object_t *object, item_list_t *items) {

    int stat = OK;
    int startx = 0;
    int starty = 0;
    int height = 0;
    int width  = 0;
    window_t *window = NULL;
    container_t *self = NULL;

    if (object != NULL) {

        when_error_in {

            /* initilize the base klass here */

            stat = WIDGET_KLASS->ctor(object, items);
            check_return(stat, object);

            WIDGET(object)->_draw = _container_draw;
            WIDGET(object)->_erase = _container_erase;
            WIDGET(object)->_event = _container_event;
            WIDGET(object)->_remove = _container_remove;

            startx = WIDGET(object)->coordinates->startx;
            starty = WIDGET(object)->coordinates->starty;
            height = WIDGET(object)->coordinates->height;
            width  = WIDGET(object)->coordinates->width;

            /* capture our items */

            if (items != NULL) {

                int x;
                for (x = 0;; x++) {

                    if ((items[x].buffer_length == 0) &&
                        (items[x].item_code == 0)) break;

                    switch(items[x].item_code) {
                        case CONTAINER_K_WINDOW: {
                            window = items[x].buffer_address;
                            break;
                        }
                    }

                }

            }

            /* initialize our derived klass here */

            self = CONTAINER(object);

            /* assign our methods here */

            self->ctor = _container_ctor;
            self->dtor = _container_dtor;
            self->_compare = _container_compare;

            /* initialize internal variables here */

            self->type = 0;
            self->window = window;

            if (self->window != NULL) {

                errno = 0;
                if ((self->area = derwin(window->inner, height, width, startx, starty)) == NULL) {

                    cause_error(errno);

                }

            }

            exit_when;

        } use {

            stat = ERR;
            object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
            clear_error();

        } end_when;

    }

    return stat;

}

int _container_dtor(object_t *object) {

    int stat = OK;
    component_t *component = NULL;
    widget_t *widget = WIDGET(object);
    container_t *self = CONTAINER(object);

    /* free local resources here */

    while ((component = que_pop_head(&widget->things))) {

        component_destroy(component);

    }

    que_init(&widget->things);

    if (self->area != NULL) {

        werase(self->area);
        delwin(self->area);

    }

    /* walk the chain, freeing as we go */

    object_demote(object, widget_t);
    widget_destroy(widget);

    return stat;

}

int _container_compare(container_t *us, container_t *them) {

    int stat = ERR;

    if (((widget_compare(WIDGET(us), WIDGET(them))) == OK) &&
        (us->ctor == them->ctor) &&
        (us->dtor == them->dtor) &&
        (us->_compare == them->_compare) &&
        (us->_override == them->_override)) {

        stat = OK;

    }

    return stat;

}

int _container_draw(widget_t *widget) {

    /* must be overridden in a derived klass */

    return OK;

}

int _container_erase(widget_t *widget) {

    int stat = OK;
    component_t *component = NULL;
    container_t *self = CONTAINER(widget);

    when_error_in {

        if (self->area != NULL) {

            for (component = que_last(&widget->things);
                 component != NULL;
                 component = que_prev(&widget->things)) {

                stat = component_erase(component);
                check_return(stat, component);

            }

            errno = 0;
            stat = wclear(self->area);
            check_status(stat, OK, errno);

            errno = 0;
            stat = wnoutrefresh(self->area);
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

int _container_event(widget_t *widget, events_t *event) {

    /* must be overridden in a derived klass */

    return OK;

}

int _container_remove(widget_t *widget, void *thing) {

    int stat = OK;
    component_t *temp = NULL;
    component_t *self = COMPONENT(widget);
    component_t *component = (component_t *)thing;

    when_error_in {

        for (temp = que_first(&widget->things);
             temp != NULL;
             temp = que_next(&widget->things)) {

            if ((component_compare(component, temp) == OK)) {

                component_t *junk = que_delete(&widget->things);
                stat = component_destroy(junk);
                check_return(stat, junk);
                break;

            }

        }

        if (que_empty(&widget->things)) {

            stat = que_init(&widget->things);
            check_status(stat, QUE_OK, E_INVOPS);

        }

        exit_when;

    } use {

        stat = ERR;
        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}


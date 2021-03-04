
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

#include "bell.h"
#include "when.h"
#include "window.h"
#include "error_codes.h"

require_klass(WIDGET_KLASS);

/*----------------------------------------------------------------*/
/* klass defination                                               */
/*----------------------------------------------------------------*/

int _bell_ctor(object_t *object, item_list_t *);
int _bell_dtor(object_t *);
int _bell_compare(bell_t *, bell_t *);

int _bell_draw(widget_t *);
int _bell_erase(widget_t *);
int _bell_add(widget_t *, void *);
int _bell_remove(widget_t *, void *);
int _bell_event(widget_t *, events_t *);

declare_klass(BELL_KLASS) {
    .size = KLASS_SIZE(bell_t),
    .name = KLASS_NAME(bell_t),
    .ctor = _bell_ctor,
    .dtor = _bell_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

bell_t *bell_create(window_t *window, int times) {

    int stat = ERR;
    bell_t *self = NULL;
    item_list_t items[2];

    SET_ITEM(items[0], BELL_K_TIMES, &times, sizeof(int), NULL);
    SET_ITEM(items[1], 0, 0, 0, 0);

    self = (bell_t *)object_create(BELL_KLASS, items, &stat);

    return self;

}

int bell_destroy(bell_t *self) {

    int stat = OK;

    when_error {

        if (self != NULL) {

            if (object_assert(self, bell_t)) {

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

int bell_compare(bell_t *us, bell_t *them) { 

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

int _bell_ctor(object_t *object, item_list_t *items) {

    int times = 1;
    int stat = ERR;
    bell_t *self = NULL;

    if (object != NULL) {

        stat = OK;

        when_error_in {

            /* initilize the base klass here */

            stat = WIDGET_KLASS->ctor(object, items);
            check_return(stat, object);

            WIDGET(object)->_add = _bell_add;
            WIDGET(object)->_draw = _bell_draw;
            WIDGET(object)->_erase = _bell_erase;
            WIDGET(object)->_event = _bell_event;
            WIDGET(object)->_remove = _bell_remove;

            /* capture our items */

            if (items != NULL) {

                int x;
                for (x = 0;; x++) {

                    if ((items[x].buffer_length == 0) &&
                        (items[x].item_code == 0)) break;

                    switch(items[x].item_code) {
                        case BELL_K_TIMES: {
                            memcpy(&times, 
                                   items[x].buffer_address,
                                   items[x].buffer_length);
                            break;
                        }
                    }

                }

            }

            /* initialize our derived klass here */

            self = BELL(object);

            /* assign our methods here */

            self->ctor = _bell_ctor;
            self->dtor = _bell_dtor;
            self->_compare = _bell_compare;

            /* initialize internal variables here */

            self->times = times;

            exit_when;

        } use {

            stat = ERR;
            object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
            clear_error();

        } end_when;

    }

    return stat;

}

int _bell_dtor(object_t *object) {

    int stat = OK;
    widget_t *widget = WIDGET(object);

    /* free local resources here */


    /* walk the chain, freeing as we go */

    object_demote(object, widget_t);
    widget_destroy(widget);

    return stat;

}

int _bell_compare(bell_t *us, bell_t *them) {

    int stat = ERR;

    if (((widget_compare(WIDGET(us), WIDGET(them)) == OK) &&
         (us->ctor == them->ctor) &&
         (us->dtor == them->dtor) &&
         (us->_compare == them->_compare) &&
         (us->times == them->times))) {

        stat = OK;

    }

    return stat;

}

int _bell_add(widget_t *widget, void *thing) {

    /* not applicable */

    return OK;

}

int _bell_draw(widget_t *widget) {

    int stat = OK;
    bell_t *self = BELL(widget);

    when_error_in {

        int x;
        for (x = 0; x < self->times; x++) {

            stat = beep();
            check_status(stat, OK, E_INVOPS);

        }

        exit_when;

    } use {

        stat = ERR;
        object_set_error2(widget, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return OK;

}

int _bell_erase(widget_t *widget) {

    /* not applicable */

    return OK;

}

int _bell_event(widget_t *widget, events_t *event) {

    /* not applicable */

    return OK;

}

int _bell_remove(widget_t *widget, void *thing) {

    /* not applicable */

    return OK;

}


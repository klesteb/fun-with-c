
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

#include <stdio.h>
#include <ncurses.h>
#include <panel.h>

#include "when.h"
#include "colors.h"
#include "events.h"
#include "object.h"
#include "nix_util.h"
#include "que_util.h"
#include "container.h"
#include "workbench.h"
#include "error_codes.h"

#define TIMEOUT 1.0

require_klass(OBJECT_KLASS);

/*----------------------------------------------------------------*/
/* klass methods                                                  */
/*----------------------------------------------------------------*/

int _workbench_ctor(object_t *, item_list_t *);
int _workbench_dtor(object_t *);

int _workbench_loop(workbench_t *);
int _workbench_event(workbench_t *, event_t *);
int _workbench_compare(workbench_t *, workbench_t *);
int _workbench_override(workbench_t *, item_list_t *);

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

declare_klass(WORKBENCH_KLASS) {
    .size = KLASS_SIZE(workbench_t),
    .name = KLASS_NAME(workbench_t),
    .ctor = _workbench_ctor,
    .dtor = _workbench_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

workbench_t *workbench_create(item_list_t *items) {

    int stat = ERR;
    workbench_t *self = NULL;

    self = (workbench_t *)object_create(WORKBENCH_KLASS, items, &stat);

    return self;

}

int workbench_destroy(workbench_t *self) {

    int stat = OK;

    when_error {

        if (self != NULL) {

            if (object_assert(self, workbench_t)) {

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

        object_set_error2(OBJECT(self), trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int workbench_override(workbench_t *self, item_list_t *items) {

    int stat = OK;

    when_error {
        
        if (self != NULL) {

            stat = self->_override(self, items);
            check_status(stat, OK, E_INVOPS);

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(OBJECT(self), trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int workbench_compare(workbench_t *us, workbench_t *them) {

    int stat = OK;

    when_error {

        if (us != NULL) {

            if (object_assert(them, workbench_t)) {

                stat = us->_compare(us, them);
                check_status(stat, OK, E_NOTSAME);

            } else {

                cause_error(E_INVOBJ);

            }

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(OBJECT(us), trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int workbench_loop(workbench_t *self) {

    int stat = OK;

    when_error {

        if (self != NULL) {

            stat = self->_loop(self);
            check_status(stat, OK, E_INVOPS);

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(OBJECT(self), trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

/*----------------------------------------------------------------*/
/* private methods                                                */
/*----------------------------------------------------------------*/

static int _event_handler(NxAppContext context, NxIntervalId id, void *data) {

    int stat = OK;
    event_t *event = NULL;
    workbench_t *self = (workbench_t *)data;

    while ((event = que_pop_head(&self->events))) {

        stat = self->_event(self, event);
        if (stat != OK) break;

    }

    if (que_empty(&self->events)) {

        que_init(&self->events);

    }

    NxAddTimeOut(NULL, TIMEOUT, _event_handler, (void *)self);

    return stat;

}

static int _read_stdin(NxAppContext context, NxInputId id, int source, void *data) {

    int ch;
    int stat = OK;
    workbench_t *self = (workbench_t *)data;

    while ((ch = getch()) != ERR) {

        event_t *event = calloc(1, sizeof(event_t));
        KEVENT *kevent = calloc(1, sizeof(KEVENT));
        kevent->keycode = ch;

        event->type = EVENT_K_KEYBOARD;
        event->data = (void *)kevent;

        que_push_tail(&self->events, event);

        doupdate();

    }

    return stat;

}

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

int _workbench_ctor(object_t *object, item_list_t *items) {

    int stat = ERR;
    workbench_t *self = NULL;

    if (object != NULL) {

        /* capture our items */

        if (items != NULL) {

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                /* switch(items[x].item_code) { */
                /*     case WORKBENCH_K_TYPE: { */
                /*         memcpy(&type,  */
                /*                items[x].buffer_address,  */
                /*                items[x].buffer_length); */
                /*         break; */
                /*     } */
                /* } */

            }

        }

        /* initilize our base klass here */


        /* initialize our derived klass here */

        self = WORKBENCH(object);

        /* assign our methods here */

        self->ctor = _workbench_ctor;
        self->dtor = _workbench_dtor;
        self->_compare = _workbench_compare;
        self->_override = _workbench_override;
        self->_loop = _workbench_loop;
        self->_event = _workbench_event;

        /* initialize internal variables here */

        que_init(&self->events);

        initscr();
        cbreak();
        noecho();
        nodelay(stdscr, TRUE);
        keypad(stdscr, TRUE);

        if (has_colors() == FALSE) {

            endwin();
            printf("Your terminal does not support color\n");
            exit(1);

        }

        init_colorpairs();
        erase();
        refresh();
        curs_set(0);

        stat = OK;

    }

    return stat;

}

int _workbench_dtor(object_t *object) {

    int stat = OK;
    event_t *event = NULL;

    /* free local resources here */

    while ((event = que_pop_head(&self->events))) {

        free(event->data);
        free(event);

    }

    if (que_empty(&self->events)) {

        que_init(&self->events);

    }
    
    endwin();

    /* walk the chain, freeing as we go */

    object_demote(object, object_t);
    object_destroy(object);

    return stat;

}

int _workbench_override(workbench_t *self, item_list_t *items) {

    int stat = -1;

    if (items != NULL) {

        int x;
        for (x = 0;; x++) {

            if ((items[x].buffer_length == 0) &&
                (items[x].item_code == 0)) break;

            switch(items[x].item_code) {
                case WORKBENCH_M_DESTRUCTOR: {
                    self->dtor = items[x].buffer_address;
                    stat = 0;
                    break;
                }
            }

        }

    }

    return stat;

}

int _workbench_compare(workbench_t *self, workbench_t *other) {

    int stat = ERR;

    if ((object_compare(OBJECT(self), OBJECT(other)) == 0) &&
        (self->ctor == other->ctor) &&
        (self->dtor == other->dtor) &&
        (self->_compare == other->_compare) &&
        (self->_override == other->_override) &&
        (self->_event == other->_event) &&
        (self->_loop == other->_loop)) {

        stat = OK;

    }

    return stat;

}

int _workbench_event(workbench_t *self, event_t *event) {

    int stat = ERR;
    container_t *container = NULL;

    for (container = que_first(&self->containers);
         container != NULL;
         container = que_next(&self->containers)) {

        stat = container_event(container, event);
        if (stat != OK) break;

        wnoutrefresh(container->area);

    }

    return stat;

}

int _workbench_loop(workbench_t *self) {

    int stat = OK;

    NxAddInput(NULL, fileno(stdin), NxInputReadMask, _read_stdin, (void *)self);
    NxAddTimeOut(NULL, TIMEOUT, _event_handler, (void *)self);

    stat = NxMainLoop(NULL);

    return stat;

}


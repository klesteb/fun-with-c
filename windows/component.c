
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
#include <errno.h>

#include "when.h"
#include "colors.h"
#include "events.h"
#include "object.h"
#include "component.h"
#include "item_list.h"
#include "error_codes.h"

require_klass(OBJECT_KLASS);

/*----------------------------------------------------------------*/
/* klass defination                                               */
/*----------------------------------------------------------------*/

int _component_ctor(object_t *object, item_list_t *);
int _component_dtor(object_t *);
int _component_draw(component_t *);
int _component_erase(component_t *);
int _component_refresh(component_t *);
int _component_event(component_t *, events_t *);
int _component_compare(component_t *, component_t *);
int _component_override(component_t *, item_list_t *);

declare_klass(COMPONENT_KLASS) {
    .size = KLASS_SIZE(component_t),
    .name = KLASS_NAME(component_t),
    .ctor = _component_ctor,
    .dtor = _component_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

component_t *component_create(int height, int width, void *data, int size) {

    int stat = ERR;
    item_list_t items[4];
    component_t *self = NULL;

    SET_ITEM(items[0], COMPONENT_K_WIDTH, &width, sizeof(int), NULL);
    SET_ITEM(items[1], COMPONENT_K_HEIGHT, &height, sizeof(int), NULL);

    if (data != NULL) {

        SET_ITEM(items[2], COMPONENT_K_DATA, data, size, NULL);
        SET_ITEM(items[3], 0, 0, 0, 0);

    } else {

        SET_ITEM(items[2], 0, 0, 0, 0);
        SET_ITEM(items[3], 0, 0, 0, 0);

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

int component_override(component_t *self, item_list_t *items) {

    int stat = OK;

    when_error {

        if ((self != NULL) && (items != NULL)) {

            stat = self->_override(self, items);
            check_status(stat, OK, E_INVOPS);

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

int component_erase(component_t *self) {

    int stat = OK;

    when_error {

        if (self != NULL) {

            stat = self->_erase(self);
            check_status(stat, OK, E_INVOPS);

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

int component_draw(component_t *self) {

    int stat = OK;

    when_error {
        
        if (self != NULL) {

            stat = self->_draw(self);
            check_status(stat, OK, E_INVOPS);

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

int component_event(component_t *self, events_t *event) {

    int stat = OK;

    when_error {

        if ((self != NULL) && (event != NULL)) {

            stat = self->_event(self, event);
            check_status(stat, OK, E_INVOPS);

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

int component_refresh(component_t *self) {

    int stat = OK;

    when_error {
        
        if (self != NULL) {

            stat = self->_refresh(self);
            check_status(stat, OK, E_INVOPS);

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

int component_set_position(component_t *self, int row, int col) {

    int stat = OK;

    when_error {

        if (self != NULL) {

            self->row = row;
            self->col = col;

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

int component_get_position(component_t *self, int *row, int *col) {

    int stat = OK;

    when_error {

        if ((self != NULL) && (row != NULL) && (col != NULL)) {

            *row = self->row;
            *col = self->col;

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

int component_set_colors(component_t *self, int fg, int bg) {
    
    int stat = OK;

    when_error {

        if ((self != NULL) && (fg <= BWHITE) && (bg <= WHITE)) {

            self->fg = fg;
            self->bg = bg;

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

int component_get_colors(component_t *self, int *fg, int *bg) {

    int stat = OK;

    when_error {

        if ((self != NULL) && (fg != NULL) && (bg != NULL)) {

            *fg = self->fg;
            *bg = self->bg;

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

int component_set_padding(component_t *self, int padding) {

    int stat = OK;

    when_error {

        if (self != NULL) {

            self->padding = padding;

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

int component_get_padding(component_t *self, int *padding) {

    int stat = OK;

    when_error {

        if ((self != NULL) && (padding != NULL)) {

            *padding = self->padding;

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

int component_set_attribute(component_t *self, int attr) {

    int stat = OK;

    when_error {

        if (self != NULL) {

            self->attribute = attr;

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

int component_get_attribute(component_t *self, int *attr) {

    int stat = OK;

    when_error {

        if ((self != NULL) && (attr != NULL)) {

            *attr = self->attribute;

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

int component_set_area(component_t *self, WINDOW *area) {

    int stat = OK;

    when_error {

        if ((self != NULL) && (area != NULL)) {

            self->area = area;

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

int component_get_area(component_t *self, WINDOW *area) {

    int stat = OK;

    when_error {

        if ((self != NULL) && (area != NULL)) {

            area = self->area;

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

    int col = 0;
    int row = 0;
    int stat = ERR;
    int height = 1;
    int width = 32;
    int value_size = 0;
    int fg = WHITE;
    int bg = BLACK;
    void *value = NULL;
    component_t *self = NULL;

    if (object != NULL) {

        /* capture our items */

        if (items != NULL) {

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                switch(items[x].item_code) {
                    case COMPONENT_K_HEIGHT: {
                        memcpy(&height, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case COMPONENT_K_WIDTH: {
                        memcpy(&width, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case COMPONENT_K_FOREGROUND: {
                        memcpy(&fg, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case COMPONENT_K_BACKGROUND: {
                        memcpy(&bg, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case COMPONENT_K_DATA: {
                        value_size = items[x].buffer_length;
                        value = items[x].buffer_address;
                        break;
                    }
                }

            }

        }

        /* initilize our base klass here */

        object_set_error1(object, 0);

        /* initialize our derived klass here */

        self = COMPONENT(object);

        /* assign our methods here */

        self->ctor = _component_ctor;
        self->dtor = _component_dtor;

        self->_draw = _component_draw;
        self->_erase = _component_erase;
        self->_event = _component_event;
        self->_compare = _component_compare;
        self->_refresh = _component_refresh;
        self->_override = _component_override;

        /* initialize internal variables here */

        self->bg = bg;
        self->fg = fg;
        self->col = col;
        self->row = row;
        self->padding = 0;
        self->data = value;
        self->attribute = 0;
        self->focus = FALSE;
        self->width = width;
        self->height = height;
        self->size = value_size;

        stat = OK;

    }

    return stat;

}

int _component_dtor(object_t *object) {

    int stat = OK;
    component_t *self = COMPONENT(object);

    /* free local resources here */

    self->_erase(self);

    /* walk the chain, freeing as we go */

    object_demote(object, object_t);
    object_destroy(object);

    return stat;

}

int _component_compare(component_t *us, component_t *them) {

    int stat = ERR;

    if (((object_compare(OBJECT(us), OBJECT(them))) == 0) &&
        (us->ctor == them->ctor) &&
        (us->dtor == them->dtor) &&
        (us->_draw == them->_draw) &&
        (us->_erase == them->_erase) &&
        (us->_event == them->_event) &&
        (us->_refresh == them->_refresh) &&
        (us->_compare == them->_compare) &&
        (us->_override == them->_override) &&
        (us->bg == them->bg) &&
        (us->fg == them->fg) &&
        (us->col == them->col) &&
        (us->row == them->row) &&
        (us->padding == them->padding) &&
        (us->attribute == them->attribute) &&
        (us->focus == them->focus) &&
        (us->data == them->data) &&
        (us->width == them->width) &&
        (us->height == them->height) &&
        (us->area == them->area) &&
        (us->size == them->size)) {

        stat = OK;

    }

    return stat;

}

int _component_override(component_t *self, item_list_t *items) {

    int x;
    int stat = ERR;

    for (x = 0;; x++) {

        if ((items[x].buffer_length == 0) &&
            (items[x].item_code == 0)) break;

        switch(items[x].item_code) {
            case COMPONENT_M_DRAW: {
                self->_draw = items[x].buffer_address;
                stat = OK;
                break;
            }
            case COMPONENT_M_EVENT: {
                self->_event = items[x].buffer_address;
                stat = OK;
                break;
            }
            case COMPONENT_M_ERASE: {
                self->_erase = items[x].buffer_address;
                stat = OK;
                break;
            }
            case COMPONENT_M_REFRESH: {
                self->_refresh = items[x].buffer_address;
                stat = OK;
                break;
            }
            case COMPONENT_M_DESTROY: {
                self->dtor = items[x].buffer_address;
                stat = OK;
                break;
            }
        }

    }

    return stat;

}

int _component_erase(component_t *self) {

    int stat = ERR;

    if (self->area != NULL) {

        wmove(self->area, self->row, self->col);
        wclrtoeol(self->area);

    }

    return stat;

}

int _component_refresh(component_t *self) {

    int stat = ERR;

    if ((stat == self->_erase(self)) == OK) {

        stat = self->_draw(self);

    }

    return stat;

}

int _component_draw(component_t *self) {

    /* must be overridden in a derived klass */

    return OK;

}

int _component_event(component_t *self, events_t *event) {

    /* must be overridden in a derived klass */

    return OK;

}


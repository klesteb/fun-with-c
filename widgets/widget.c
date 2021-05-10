
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

#include <string.h>
#include <errno.h>

#include "when.h"
#include "colors.h"
#include "widget.h"
#include "misc/misc.h"
#include "error_codes.h"

require_klass(OBJECT_KLASS);

/*----------------------------------------------------------------*/
/* klass methods                                                  */
/*----------------------------------------------------------------*/

int _widget_ctor(object_t *, item_list_t *);
int _widget_dtor(object_t *);

int _widget_compare(widget_t *, widget_t *);
int _widget_override(widget_t *, item_list_t *);
int _widget_draw(widget_t *);
int _widget_erase(widget_t *);
int _widget_add(widget_t *, void *);
int _widget_remove(widget_t *, void *);
int _widget_event(widget_t *, events_t *);

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

declare_klass(WIDGET_KLASS) {
    .size = KLASS_SIZE(widget_t),
    .name = KLASS_NAME(widget_t),
    .ctor = _widget_ctor,
    .dtor = _widget_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

widget_t *widget_create(item_list_t *items) {

    int stat = ERR;
    widget_t *self = NULL;

    self = (widget_t *)object_create(WIDGET_KLASS, items, &stat);
    
    return self;

}

int widget_destroy(widget_t *self) {

    int stat = OK;

    when_error_in {

        if (self != NULL) {

            stat = self->dtor(OBJECT(self));
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

int widget_override(widget_t *self, item_list_t *items) {

    int stat = OK;

    when_error_in {

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

int widget_compare(widget_t *us, widget_t *them) {

    int stat = OK;

    when_error_in {

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

int widget_add(widget_t *self, void *thing) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (thing == NULL)) {

            cause_error(E_INVPARM);

        }
        
        stat = self->_add(self, thing);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int widget_draw(widget_t *self) {

    int stat = OK;

    when_error_in {

        if (self != NULL) {

            stat = self->_draw(self);
            check_return(stat, self);

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

int widget_erase(widget_t *self) {

    int stat = OK;

    when_error_in {

        if (self != NULL) {

            stat = self->_erase(self);
            check_return(stat, self);

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

int widget_event(widget_t *self, events_t *event) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (event == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_event(self, event);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int widget_refresh(widget_t *self) {

    int stat = OK;

    when_error_in {

        if (self == NULL) {

            cause_error(E_INVPARM);

        }

        stat = self->_erase(self);
        check_return(stat, self);

        stat = self->_draw(self);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int widget_remove(widget_t *self, void *thing) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (thing == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_remove(self, thing);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int widget_set_theme(widget_t *self, theme_t *theme) {

    int stat = OK;

    when_error_in {

        if ((self != NULL) && (theme != NULL)) {

            self->theme->attribute  = theme->attribute;
            self->theme->background = theme->background;
            self->theme->foreground = theme->foreground;

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

int widget_get_theme(widget_t *self, theme_t *theme) {

    int stat = OK;

    when_error_in {

        if ((self != NULL) && (theme != NULL)) {

            (*theme).attribute  = self->theme->attribute;
            (*theme).background = self->theme->background;
            (*theme).foreground = self->theme->foreground;

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

int widget_set_coordinates(widget_t *self, coordinates_t *coordinates) {

    int stat = OK;

    when_error_in {

        if ((self != NULL) && (coordinates != NULL)) {

            self->coordinates->width  = coordinates->width;
            self->coordinates->height = coordinates->height;
            self->coordinates->startx = coordinates->startx;
            self->coordinates->starty = coordinates->starty;

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

int widget_get_coordinates(widget_t *self, coordinates_t *coordinates) {

    int stat = OK;

    when_error_in {

        if ((self != NULL) && (coordinates != NULL)) {

            (*coordinates).width  = self->coordinates->width;
            (*coordinates).height = self->coordinates->height;
            (*coordinates).startx = self->coordinates->startx;
            (*coordinates).starty = self->coordinates->starty;

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

int _widget_ctor(object_t *object, item_list_t *items) {

    int stat = OK;
    widget_t *self = NULL;
    theme_t *theme = NULL;
    coordinates_t *coordinates = NULL;

    if (object != NULL) {

        when_error_in {
            
            /* default theme */

            errno = 0;
            if ((theme = calloc(1, sizeof(theme_t))) != NULL) {

                theme->attribute = A_NORMAL;
                theme->background = BLACK;
                theme->foreground = WHITE;

            } else {

                cause_error(errno);
                
            }

            /* default coordinates */

            errno = 0;
            if ((coordinates = calloc(1, sizeof(coordinates_t))) != NULL) {

                coordinates->width  = 1;
                coordinates->height = 1;
                coordinates->startx = 0;
                coordinates->starty = 0;

            } else {

                cause_error(errno);

            }
            
            /* capture our items */

            if (items != NULL) {

                int x;
                for (x = 0;; x++) {

                    if ((items[x].buffer_length == 0) &&
                        (items[x].item_code == 0)) break;

                    switch(items[x].item_code) {
                        case WIDGET_K_THEME: {
                            memcpy(theme, 
                                   items[x].buffer_address, 
                                   items[x].buffer_length);
                            break;
                        }
                        case WIDGET_K_COORDINATES: {
                            memcpy(coordinates, 
                                   items[x].buffer_address, 
                                   items[x].buffer_length);
                            break;
                        }
                    }

                }

            }

            /* initialize our base klass here */

            object_set_error1(object, OK);

            /* initialize our derived klass here */

            self = WIDGET(object);

            /* assign our methods here */

            self->ctor = _widget_ctor;
            self->dtor = _widget_dtor;
            self->_compare = _widget_compare;
            self->_override = _widget_override;

            self->_add = widget_add;
            self->_draw = _widget_draw;
            self->_erase = _widget_erase;
            self->_event = _widget_event;
            self->_remove = _widget_remove;

            /* initialize internal variables here */

            self->theme = theme;
            self->coordinates = coordinates;

            stat = que_init(&self->things);
            check_status(stat, QUE_OK, E_INVOPS);

            exit_when;

        } use {

            stat = ERR;
            object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
            clear_error();

        } end_when;

    }

    return stat;

}

int _widget_dtor(object_t *object) {

    int stat = OK;
    widget_t *widget = WIDGET(object);
    
    /* free local resources here */

    free(widget->theme);
    free(widget->coordinates);

    /* walk the chain, freeing as we go */

    object_demote(object, object_t);
    object_destroy(object);

    return stat;

}

int _widget_override(widget_t *self, item_list_t *items) {

    int stat = ERR;

    if (items != NULL) {

        int x;
        for (x = 0;; x++) {

            if ((items[x].buffer_length == 0) &&
                (items[x].item_code == 0)) break;

            switch(items[x].item_code) {
                case WIDGET_M_DESTROY: {
                    self->dtor = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case WIDGET_M_ADD: {
                    self->_add = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case WIDGET_M_DRAW: {
                    self->_draw = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case WIDGET_M_ERASE: {
                    self->_erase = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case WIDGET_M_EVENT: {
                    self->_event = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case WIDGET_M_REMOVE: {
                    self->_remove = items[x].buffer_address;
                    stat = OK;
                    break;
                }
            }

        }

    }

    return stat;

}

int _widget_compare(widget_t *self, widget_t *other) {

    int stat = ERR;

    if ((object_compare(OBJECT(self), OBJECT(other)) == OK) &&
        (self->ctor == other->ctor) &&
        (self->dtor == other->dtor) &&
        (self->_compare == other->_compare) &&
        (self->_override == other->_override) &&
        (self->_add == other->_add) &&
        (self->_draw == other->_draw) &&
        (self->_erase == other->_erase) &&
        (self->_event == other->_event) &&
        (self->_remove == other->_remove) &&
        (self->theme->attribute == self->theme->attribute) &&
        (self->theme->background == self->theme->background) &&
        (self->theme->foreground == self->theme->foreground) &&
        (self->coordinates->width == other->coordinates->width) &&
        (self->coordinates->height == other->coordinates->height) &&
        (self->coordinates->startx == other->coordinates->startx) &&
        (self->coordinates->starty == other->coordinates->starty) ) {

        stat = OK;

    }

    return stat;

}

int _widget_add(widget_t *self, void *data) {

    int stat = OK;

    when_error_in {

        stat = que_push_head(&self->things, data);
        check_status(stat, QUE_OK, E_NOQUEUE);

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int _widget_draw(widget_t *self) {

    int x;
    int stat = OK;
    int fg = self->theme->foreground;
    int bg = self->theme->background;
    int attr = self->theme->attribute;
    int col = self->coordinates->startx;
    int row = self->coordinates->starty;
    char *blank = string(self->coordinates->width, '.');

    when_error_in {

        for (x = 0; x < self->coordinates->height; x++) {

            stat = move(row + x, col);
            check_status(stat, OK, E_INVOPS);

            stat = attron(attr);
            check_status(stat, OK, E_INVOPS);

            stat = coloron(fg, bg);
            check_status(stat, OK, E_INVOPS);

            stat = printw("%s\n", blank);
            check_status(stat, OK, E_INVOPS);

        }

        standend();
        refresh();
        free(blank);

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int _widget_erase(widget_t *self) {

    int x;
    int stat = OK;
    int fg = self->theme->foreground;
    int bg = self->theme->background;
    int attr = self->theme->attribute;
    int col = self->coordinates->startx;
    int row = self->coordinates->starty;
    char *blank = spaces(self->coordinates->width);

    when_error_in {

        for (x = 0; x < self->coordinates->height; x++) {

            stat = move(row + x, col);
            check_status(stat, OK, E_INVOPS);

            stat = attron(attr);
            check_status(stat, OK, E_INVOPS);

            stat = coloron(fg, bg);
            check_status(stat, OK, E_INVOPS);

            stat = printw("%s\n", blank);
            check_status(stat, OK, E_INVOPS);

        }

        standend();
        refresh();
        free(blank);

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int _widget_remove(widget_t *self, void *data) {

    int stat = OK;
    widget_t *temp = NULL;

    when_error_in {

        for (temp = que_first(&self->things);
             temp != NULL;
             temp = que_next(&self->things)) {

            if (memcmp(temp, data, sizeof(widget_t)) == 0) {

                widget_t *junk = que_delete(&self->things);
                free(junk);
                break;

            }

        }

        if (que_empty(&self->things)) {

            stat = que_init(&self->things);
            check_status(stat, QUE_OK, E_INVOPS);

        }

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return OK;

}

int _widget_event(widget_t *self, events_t *event) {

    return OK;

}


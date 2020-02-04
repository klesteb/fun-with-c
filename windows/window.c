
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
#include <stdarg.h>
#include <errno.h>

#include "when.h"
#include "object.h"
#include "window.h"
#include "events.h"
#include "colors.h"
#include "container.h"
#include "item_list.h"
#include "error_codes.h"

require_klass(OBJECT_KLASS);

/*----------------------------------------------------------------*/
/* klass defination                                               */
/*----------------------------------------------------------------*/

static int _box_window(window_t *);

int _window_ctor(object_t *object, item_list_t *);
int _window_dtor(object_t *);
int _window_draw(window_t *);
int _window_erase(window_t *);
int _window_refresh(window_t *);
int _window_event(window_t *, event_t *);
int _window_compare(window_t *, window_t *);
int _window_override(window_t *, item_list_t *);
int _window_add_container(window_t *, container_t *);
int _window_remove_container(window_t *, container_t *);

declare_klass(WINDOW_KLASS) {
    .size = KLASS_SIZE(window_t),
    .name = KLASS_NAME(window_t),
    .ctor = _window_ctor,
    .dtor = _window_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

window_t *window_create(int row, int col, int height, int width) {

    int stat = ERR;
    item_list_t items[5];
    window_t *self = NULL;

    SET_ITEM(items[0], WINDOW_K_WIDTH, &width, sizeof(int), NULL);
    SET_ITEM(items[1], WINDOW_K_HEIGHT, &height, sizeof(int), NULL);
    SET_ITEM(items[2], WINDOW_K_ROW, &row, sizeof(int), NULL);
    SET_ITEM(items[3], WINDOW_K_COL, &col, sizeof(int), NULL);
    SET_ITEM(items[4], 0, 0, 0, 0);

    self = (window_t *)object_create(WINDOW_KLASS, items, &stat);

    return self;

}

int window_destroy(window_t *self) {

    int stat = OK;

    when_error {

        if (self != NULL) {

            if (object_assert(self, window_t)) {

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

int window_compare(window_t *us, window_t *them) { 

    int stat = OK;

    when_error {

        if ((us != NULL) && (them != NULL)) {

            if (object_assert(them, window_t)) {

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

        object_set_error2(us, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int window_override(window_t *self, item_list_t *items) {

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

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int window_draw(window_t *self) {

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

int window_erase(window_t *self) {

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

int window_refresh(window_t *self) {

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

int window_event(window_t *self, event_t *event) {

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

int window_add_container(window_t *self, container_t *container) {

    int stat = OK;

    when_error {

        if ((self != NULL) && (container != NULL)) {

            stat = self->_add_container(self, container);
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

int window_remove_container(window_t *self, container_t *container) {

    int stat = OK;

    when_error {

        if ((self != NULL) && (container != NULL)) {

            stat = self->_remove_container(self, container);
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

int window_box(window_t *self, char *title) {

    int stat = OK;

    when_error {
        
        if (self != NULL) {

            if (self->title != NULL) free(self->title);

            self->boxed = 1;
            self->title = strdup(title);

            stat = _box_window(self);
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

int window_get_colors(window_t *self, int *fg, int *bg) {

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

int window_set_colors(window_t *self, int fg, int bg) {

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

int window_clear(window_t *self) {

    int stat = OK;

    when_error {

        if (self != NULL) {

            stat = werase(self->inner);
            check_status(stat, OK, E_INVOPS);
            stat = wrefresh(self->inner);
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

int window_output(window_t *self, int row, int col, char *fmt, ...) {

    va_list aptr;
    int stat = OK;
    char buffer[2048];

    when_error {
        
        if ((self != NULL) && (row >= 0) && (col >= 0) && (fmt != NULL)) {

            va_start(aptr, fmt);
            vsnprintf(buffer, 2047, fmt, aptr);
            va_end(aptr);

            wattrset(self->inner, self->attribute); 
            wcolorset(self->inner, self->fg, self->bg);
            stat = mvwprintw(self->inner, row, col, buffer);
            wstandend(self->inner);

            wnoutrefresh(self->inner);

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

int _window_ctor(object_t *object, item_list_t *items) {

    int col = 0;
    int row = 0;
    int boxed = 0;
    int stat = ERR;
    int height = 1;
    int width = 32;
    int attribute = 0;
    char *title = NULL;
    int fg = WHITE;
    int bg = BLACK;
    window_t *self = NULL;

    if (object != NULL) {

        /* capture our items */

        if (items != NULL) {

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                switch(items[x].item_code) {
                    case WINDOW_K_ROW: {
                        memcpy(&row, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case WINDOW_K_COL: {
                        memcpy(&col, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case WINDOW_K_HEIGHT: {
                        memcpy(&height, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case WINDOW_K_WIDTH: {
                        memcpy(&width, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                }

            }

        }

        /* initilize our base klass here */

        object_set_error(object, OK);

        /* initialize our derived klass here */

        self = WINDOW(object);

        /* assign our methods here */

        self->ctor = _window_ctor;
        self->dtor = _window_dtor;

        self->_draw = _window_draw;
        self->_erase = _window_erase;
        self->_event = _window_event;
        self->_refresh = _window_refresh;
        self->_compare = _window_compare;
        self->_override = _window_override;
        self->_add_container = _window_add_container;
        self->_remove_container = _window_remove_container;

        /* initialize internal variables here */

        self->bg = bg;
        self->fg = fg;
        self->col = col;
        self->row = row;
        self->boxed = boxed;
        self->title = title;
        self->width = width;
        self->height = height;
        self->attribute = attribute;

        que_init(&self->containers);

        errno = 0;
        if ((self->outer = newwin(height + 2, width + 2, row, col)) == NULL) {
            
            object_set_error(self, errno);
            goto fini;
            
        }

        errno = 0;
        if ((self->inner = derwin(self->outer, height, width, 1, 1)) == NULL) {

            object_set_error(self, errno);
            goto fini;
            
        }

        stat = OK;
        
    }

    fini:
    return stat;

}

int _window_dtor(object_t *object) {

    int stat = OK;
    window_t *self = NULL;
    container_t *container = NULL;

    /* free local resources here */

    self = WINDOW(object);

    while ((container = que_pop_head(&self->containers))) {

        container_destroy(container);

    }

    if (self->inner != NULL) {

        werase(self->inner);
        delwin(self->inner);

    }

    if (self->outer != NULL) {

        werase(self->outer);
        delwin(self->outer);

    }

    if (self->title != NULL) free(self->title);

    /* walk the chain, freeing as we go */

    object_demote(object, object_t);
    object_destroy(object);

    return stat;

}

int _window_compare(window_t *us, window_t *them) {
    
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
        (us->_add_container == them->_add_container) &&
        (us->_remove_container == them->_remove_container) &&
        (us->bg == them->bg) &&
        (us->fg == them->fg) &&
        (us->col == them->col) &&
        (us->row == them->row) &&
        (us->boxed == them->boxed) &&
        (us->inner == them->inner) &&
        (us->outer == them->outer) &&
        (us->title == them->title) &&
        (us->width == them->width) &&
        (us->height == them->height) &&
        (us->attribute == them->attribute)) {

        stat = OK;

    }

    return stat;

}

int _window_override(window_t *self, item_list_t *items) {

    return OK;

}

int _window_draw(window_t *self) {

    int stat = OK;
    container_t *container = NULL;

    if (self->boxed) {

        stat = _box_window(self);
        if (stat != OK) goto fini;

    }

    wattrset(self->inner, self->attribute);
    wcolorset(self->inner, self->fg, self->bg);

    for (container = que_first(&self->containers);
         container != NULL;
         container = que_next(&self->containers)) {

        stat = container_draw(container);
        if (stat != OK) goto fini;

    }

    wstandend(self->inner);
    wnoutrefresh(self->inner);

    fini:
    return stat;

}

int _window_erase(window_t *self) {

    int stat = OK;
    container_t *container = NULL;

    for (container = que_first(&self->containers);
         container != NULL;
         container = que_next(&self->containers)) {

        stat = container_erase(container);
        if (stat != OK) goto fini;

    }

    if (self->inner != NULL) {

        wclear(self->inner);

    }

    fini:
    wnoutrefresh(self->inner);

    return stat;

}

int _window_event(window_t *self, event_t *event) {

    int stat = ERR;
    container_t *container = NULL;

    for (container = que_first(&self->containers);
         container != NULL;
         container = que_next(&self->containers)) {

        stat = container_event(container, event);
        if (stat != OK) break;

    }

    wnoutrefresh(self->inner);

    return stat;

}

int _window_refresh(window_t *self) {

    int stat = ERR;
    container_t *container = NULL;

    if (self->boxed) {

        stat = _box_window(self);
        if (stat != OK) goto fini;

    }

    for (container = que_first(&self->containers);
         container != NULL;
         container = que_next(&self->containers)) {

        stat = container_refresh(container);
        if (stat != OK) goto fini;

    }

    fini:

    wstandend(self->inner);
    wnoutrefresh(self->inner);

    return stat;

}

int _window_add_container(window_t *self, container_t *container) {

    int stat = ERR;
    WINDOW *temp = NULL;

    if (self->inner != NULL) {

        if ((temp = derwin(self->inner, container->height, 
                           container->width, container->row, 
                           container->col))) {

            werase(temp);
            container->area = temp;

            if ((que_push_tail(&self->containers, container)) == QUE_OK) {

                stat = OK;

            }

        }

    }

    return stat;

}

int _window_remove_container(window_t *self, container_t *container) {

    int stat = ERR;
    container_t *temp = NULL;

    for (temp = que_first(&self->containers);
         temp != NULL;
         temp = que_next(&self->containers)) {

        if ((container_compare(temp, container)) == OK) {

            container_t *junk = que_delete(&self->containers);
            stat = container_destroy(junk);
            break;

        }

    }

    if (que_empty(&self->containers)) {

        que_init(&self->containers);

    }

    return stat;

}

/*----------------------------------------------------------------*/
/* private methods                                                */
/*----------------------------------------------------------------*/

static int _box_window(window_t *self) {

    int len = 0;
    int stat = ERR;

    if (self->outer != NULL) {

        wbkgd(self->outer, COLOR_PAIR(colornum(self->fg, self->bg)));

        if ((stat = box(self->outer, ACS_VLINE, ACS_HLINE)) == OK) {

            len = strlen(self->title);

            if (len > 0) {

                wcolorset(self->outer, self->fg, self->bg);

                wmove(self->outer, 0, 2);
                waddch(self->outer, ACS_RTEE);
                wmove(self->outer, 0, 3);
                waddstr(self->outer, self->title);
                wmove(self->outer, 0, 3 + len);
                waddch(self->outer, ACS_LTEE);

                wnoutrefresh(self->outer);
                wstandend(self->outer);

            }

        }

    }

    return stat;

}



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
#include "menus.h"
#include "colors.h"
#include "events.h"
#include "object.h"
#include "window.h"
#include "item_list.h"
#include "menus_list.h"
#include "menus_priv.h"
#include "error_codes.h"

require_klass(WINDOW_KLASS);

/*----------------------------------------------------------------*/
/* klass methods                                                  */
/*----------------------------------------------------------------*/

int _menus_ctor(object_t *object, item_list_t *);
int _menus_dtor(object_t *);
int _menus_compare(menus_t *, menus_t *);
int _menus_override(menus_t *, item_list_t *);
int _menus_show_description(menus_t *);

int _menus_draw(widget_t *);
int _menus_erase(widget_t *);
int _menus_add(widget_t *, void *);
int _menus_remove(widget_t *, void *);
int _menus_event(widget_t *, events_t *);

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

declare_klass(MENUS_KLASS) {
    .size = KLASS_SIZE(menus_t),
    .name = KLASS_NAME(menus_t),
    .ctor = _menus_ctor,
    .dtor = _menus_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

menus_t *menus_create(char *title, int startx, int starty, int height, int width, menus_list_t *list, int list_size) {

    int stat = ERR;
    int boxed = TRUE;
    item_list_t items[5];
    coordinates_t coords;
    menus_t *self = NULL;

    coords.startx = startx;
    coords.starty = starty;
    coords.height = height;
    coords.width  = width;

    if (title == NULL) title = "";

    SET_ITEM(items[0], WIDGET_K_COORDINATES, &coords, sizeof(coordinates_t), NULL);
    SET_ITEM(items[1], WINDOW_K_TITLE, title, strlen(title), NULL);
    SET_ITEM(items[2], WINDOW_K_BOXED, &boxed, sizeof(int), NULL);
    SET_ITEM(items[3], MENUS_K_LIST, list, list_size, NULL);
    SET_ITEM(items[4], 0, 0, 0, 0);

    self = (menus_t *)object_create(MENUS_KLASS, items, &stat);

    return self;

}

int menus_destroy(menus_t *self) {

    int stat = OK;

    when_error {

        if (self == NULL) {

            cause_error(E_INVPARM);

        }

        if (object_assert(self, menus_t)) {

            stat = self->dtor(OBJECT(self));
            check_status(stat, OK, E_INVOPS);

        } else {

            cause_error(E_INVOBJ);

        }

        exit_when;

    } use {

        stat = ERR;
        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int menus_compare(menus_t *us, menus_t *them) { 

    int stat = OK;

    when_error {

        if ((us == NULL) || (them == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = us->_compare(us, them);
        check_status(stat, OK, E_NOTSAME);

        exit_when;

    } use {

        stat = ERR;
        object_set_error2(us, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int menus_override(menus_t *self, item_list_t *items) {

    int stat = OK;

    when_error {

        if ((self == NULL) || (items == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_override(self, items);
        check_return(stat, self);

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

int _menus_ctor(object_t *object, item_list_t *items) {

    int x;
    int width = 0;
    int stat = ERR;
    int startx = 0;
    int starty = 0;
    int height = 0;
    int list_size = 0;
    menus_t *self = NULL;
    menus_list_t *list = NULL;
    userptr_data_t *userptr = NULL;                 

    if (object != NULL) {

        stat = OK;

        when_error_in {

            /* initialize the base klass here */

            stat = WINDOW_KLASS->ctor(object, items); 
            check_return(stat, object);

            /* overrides to the base initialization */

            WIDGET(object)->_add = _menus_add;
            WIDGET(object)->_draw = _menus_draw;
            WIDGET(object)->_erase = _menus_erase;
            WIDGET(object)->_event = _menus_event;
            WIDGET(object)->_remove = _menus_remove;

            startx = WIDGET(object)->coordinates->startx;
            starty = WIDGET(object)->coordinates->starty;
            height = WIDGET(object)->coordinates->height;
            width  = WIDGET(object)->coordinates->width;

            /* capture our items */

            if (items != NULL) {

                for (x = 0;; x++) {

                    if ((items[x].buffer_length == 0) &&
                        (items[x].item_code == 0)) break;

                    switch(items[x].item_code) {
                        case MENUS_K_LIST: {
                            list = items[x].buffer_address;
                            list_size = items[x].buffer_length;
                            break;
                        }
                    }

                }

            }

            /* initialize our derived klass here */

            self = MENUS(object);

            /* assign our methods here */

            self->ctor = _menus_ctor;
            self->dtor = _menus_dtor;
            self->_compare = _menus_compare;
            self->_override = _menus_override;
            self->_show_description = _menus_show_description;

            /* initialize variables here */

            self->data = NULL;

            /* create menu items */

            errno = 0;
            self->items_count = (list_size / sizeof(menus_list_t));
            self->items = calloc(self->items_count + 1, sizeof(ITEM));
            if (self->items == NULL) cause_error(errno);

            for (x = 0; x < self->items_count; x++) {

                errno = 0;
                self->items[x] = new_item(list[x].label, list[x].description);
                if (self->items[x] == NULL) cause_error(errno);

                errno = 0;
                userptr = calloc(1, sizeof(userptr_data_t));
                if (userptr == NULL) cause_error(errno);

                userptr->data = list[x].data;
                userptr->callback = list[x].callback;
                userptr->data_size = list[x].data_size;

                set_item_userptr(self->items[x], userptr);

            }

            self->items[x] = NULL;

            exit_when;

        } use {

            stat = ERR;
            object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
            clear_error();

        } end_when;

    }

    return stat;

}

int _menus_dtor(object_t *object) {

    int x;
    int stat = OK;
    menus_t *self = MENUS(object);

    /* free local resources here */

    if (self->data  != NULL) free(self->data);

    for (x = 0; x <= self->items_count; x++) {

        void *junk = item_userptr(self->items[x]);
        if (junk != NULL) free(junk);

        free(self->items[x]);

    }

    /* walk the chain, freeing as we go */

    object_demote(object, window_t);
    window_destroy(WINDOW(object));

    return stat;

}

int _menus_compare(menus_t *us, menus_t *them) {

    int stat = ERR;

    if (((window_compare(WINDOW(us), WINDOW(them)) == OK)) &&
        (us->_show_description == them->_show_description) &&
        (us->items == them->items) &&
        (us->data == them->data)) {

        stat = OK;

    }

    return stat;

}

int _menus_override(menus_t *self, item_list_t *items) {

    int x;
    int stat = ERR;

    when_error_in {

        stat = WINDOW(self)->_override(WINDOW(self), items);
        check_return(stat, self);

        for (x = 0;; x++) {

            if ((items[x].buffer_length == 0) &&
                (items[x].item_code == 0)) break;

            switch(items[x].item_code) {
                case MENUS_M_SHOW_DESCRIPTION: {
                    self->_show_description = items[x].buffer_address;
                    stat = OK;
                    break;
                }
            }

        }

        exit_when;

    } use {

        stat = ERR;
        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int _menus_show_description(menus_t *self) {

    int stat = ERR;
    ITEM *item = NULL;
    error_trace_t errors;
    const char *description = NULL;
    menus_data_t *data = self->data;

    when_error_in {

        if ((item = current_item(data->menu)) != NULL) {

            if ((description = item_description(item)) != NULL) {

                if (data->callback != NULL) {

                    stat = (*data->callback)(description, &errors);
                    check_status2(stat, OK, errors);

                }

            }

        }

        exit_when;

    } use {

        stat = ERR;
        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int _menus_add(widget_t *widget, void *data) {

    return OK;

}

int _menus_draw(widget_t *widget) {

    int stat = OK;
    menus_t *self = MENUS(widget);
    window_t *window = WINDOW(widget);

    when_error_in {

        if (self->data != NULL) {

            stat = wattrset(window->outer, widget->theme->attribute);
            check_status(stat, OK, E_INVOPS);

            stat = wcolorset(window->outer, widget->theme->foreground, widget->theme->background);
            check_status(stat, OK, E_INVOPS);

            stat = wbkgd(window->inner, COLOR_PAIR(colornum(widget->theme->foreground, widget->theme->background)));
            check_status(stat, OK, E_INVOPS);

            stat = window->_box(window);
            check_return(stat, window);

            errno = 0;
            if ((self->data->menu = new_menu(self->items)) == NULL) {

                cause_error(errno);

            }

            stat = set_menu_opts(self->data->menu, self->data->options);
            check_status(stat, E_OK, stat);

            stat = set_menu_mark(self->data->menu, self->data->mark);
            check_status(stat, E_OK, stat);

            stat = set_menu_format(self->data->menu, self->data->row, self->data->col);
            check_status(stat, E_OK, stat);

            stat = set_menu_win(self->data->menu, window->outer);
            check_status(stat, E_OK, stat);

            stat = set_menu_sub(self->data->menu, window->inner);
            check_status(stat, E_OK, stat);

            stat = post_menu(self->data->menu);
            check_status(stat, E_OK, stat);

            if (self->focus != NULL) {

                ITEM *item = self->focus;
                stat = set_current_item(self->data->menu, item);
                check_status(stat, E_OK, stat);

            } else {

                stat = set_current_item(self->data->menu, self->items[0]);
                check_status(stat, E_OK, stat);

            }

            stat = self->_show_description(self);
            check_return(stat, widget);

            stat = pos_menu_cursor(self->data->menu);
            check_status(stat, E_OK, stat);

            stat = curs_set(1);
            if (stat == ERR) cause_error(E_INVOPS);

        }

        stat = wstandend(window->inner);
        check_status(stat, OK, E_INVOPS);

        stat = wnoutrefresh(window->inner);
        check_status(stat, OK, E_INVOPS);

        exit_when;

    } use {

        stat = ERR;
        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int _menus_erase(widget_t *widget) {

    int stat = OK;
    menus_t *self = MENUS(widget);
    window_t *window = WINDOW(widget);

    when_error_in {

        if (self->data != NULL) {

            menus_data_t *data = (menus_data_t *)self->data;

            if (data->menu != NULL) {

                stat = unpost_menu(data->menu);
                check_status(stat, E_OK, stat);

                stat = free_menu(data->menu);
                check_status(stat, E_OK, stat);

            }

            stat = wnoutrefresh(window->inner);
            check_status(stat, OK, E_INVOPS);

        }

        exit_when;

    } use {

        stat = ERR;
        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int _menus_event(widget_t *widget, events_t *event) {

    return OK;

}

int _menus_remove(widget_t *widget, void *thing) {

    return OK;

}


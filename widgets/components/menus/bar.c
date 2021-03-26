
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
#include "component.h"
#include "item_list.h"
#include "menus_priv.h"
#include "error_codes.h"
#include "error_trace.h"

require_klass(COMPONENT_KLASS);

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

int _bar_show_description(widget_t *widget) {

    int stat = ERR;
    ITEM *item = NULL;
    const char *description = NULL;
    component_t *self = COMPONENT(widget);
    menus_data_t *data = COMPONENT(widget)->data;

    when_error_in {

        if ((item = current_item(data->menu)) != NULL) {

            if ((description = item_description(item)) != NULL) {

                stat = wmove(self->area, 1, 1);
                check_status(stat, OK, stat);

                stat = wclrtoeol(self->area);
                check_status(stat, OK, stat);

                stat = wprintw(self->area, description);
                check_status(stat, OK, stat);

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

int _bar_menu_event(widget_t *widget, events_t *event) {

    int stat = OK;
    error_trace_t errors;
    userptr_data_t *userptr = NULL;
    component_t *self = COMPONENT(widget);
    menus_data_t *data = COMPONENT(widget)->data;

    when_error_in {

        if (self->data != NULL) {

            if (event->type == EVENT_K_KEYBOARD) {

                KEVENT *kevent = (KEVENT *)event->data;

                switch(kevent->keycode) {
                    case KEY_LEFT: {
                        stat = menu_driver(data->menu, REQ_LEFT_ITEM);
                        check_status(stat, E_OK, stat);
                        break;
                    }
                    case 9:
                    case KEY_RIGHT: {
                        stat = menu_driver(data->menu, REQ_RIGHT_ITEM);
                        check_status(stat, E_OK, stat);
                        break;
                    }
                    case KEY_HOME: {
                        stat = menu_driver(data->menu, REQ_FIRST_ITEM);
                        check_status(stat, E_OK, stat);
                        break;
                    }
                    case KEY_END: {
                        stat = menu_driver(data->menu, REQ_LAST_ITEM);
                        check_status(stat, E_OK, stat);
                        break;
                    }
                    case 10:
                    case KEY_ENTER: {
                        ITEM *item = NULL;
                        item = current_item(data->menu);
                        if (item != NULL) {
                            if ((userptr = item_userptr(item)) != NULL) {
                                if (userptr->callback != NULL) {
                                    stat = userptr->callback(userptr->data, userptr->data_size, &errors);
                                    check_status2(stat, OK, errors);
                                }
                            }
                        }
                        break;
                    }
                }

                stat = data->show_description(widget);
                check_return(stat, widget);

                stat = pos_menu_cursor(data->menu);
                check_status(stat, E_OK, stat);

                data->focus = (void *)current_item(data->menu);

            }

            stat = wnoutrefresh(self->area);
            check_status(stat, OK, E_INVOPS);

        }

        self->data = data;

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

component_t *bar_menu_create(window_t *window, int startx, int starty, int height, int width, int tab, menus_list_t *list, int list_size) {

    item_list_t items[2];
    component_t *self = NULL;
    menus_data_t *data = NULL;

    when_error_in {
        
        self = menus_create(window, startx, starty, height, width, tab, list, list_size);
        check_creation(self);

        data = self->data;
        data->row = 1;
        data->col = 9;
        data->mark = ">";
        data->callback = NULL;
        data->show_description = _bar_show_description;
        data->options = (O_ONEVALUE | O_IGNORECASE | O_SHOWMATCH | O_ROWMAJOR);
        self->data = data;

        SET_ITEM(items[0], WIDGET_M_EVENT, _bar_menu_event, 0, NULL);
        SET_ITEM(items[1], 0, 0, 0, 0);

        component_override(self, items);

        exit_when;

    } use {

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return self;

}


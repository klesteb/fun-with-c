
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
#include "events.h"
#include "object.h"
#include "widget.h"
#include "item_list.h"
#include "menus_list.h"
#include "menus_priv.h"
#include "error_codes.h"

require_klass(MENUS_KLASS);

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

int _bar_menu_event(widget_t *widget, events_t *event) {

    int stat = OK;
    ITEM *item = NULL;
    menus_t *self = MENUS(widget);
    userptr_data_t *userptr = NULL;

    when_error_in {

        if (self->data != NULL) {

            if (event->type == EVENT_K_KEYBOARD) {

                KEVENT *kevent = (KEVENT *)event->data;

                switch(kevent->keycode) {
                    case KEY_LEFT: {
                        stat = menu_driver(self->data->menu, REQ_LEFT_ITEM);
                        check_status(stat, E_OK, stat);
                        break;
                    }
                    case 9:
                    case KEY_RIGHT: {
                        stat = menu_driver(self->data->menu, REQ_RIGHT_ITEM);
                        check_status(stat, E_OK, stat);
                        break;
                    }
                    case KEY_HOME: {
                        stat = menu_driver(self->data->menu, REQ_FIRST_ITEM);
                        check_status(stat, E_OK, stat);
                        break;
                    }
                    case KEY_END: {
                        stat = menu_driver(self->data->menu, REQ_LAST_ITEM);
                        check_status(stat, E_OK, stat);
                        break;
                    }
                    case 10:
                    case KEY_ENTER: {
                        stat = menu_driver(self->data->menu, REQ_TOGGLE_ITEM);
                        check_status(stat, E_OK, stat);
                        item = current_item(self->data->menu);
                        if (item != NULL) {
                            if ((userptr = item_userptr(item)) != NULL) {
                                if (userptr->callback != NULL) {
                                    stat = userptr->callback(userptr->data, userptr->data_size);
                                    check_status(stat, OK, E_INVOPS);
                                }
                            }
                        }
                        break;
                    }
                }

                stat = self->data->callback(widget);
                check_return(stat, self);
                
                stat = pos_menu_cursor(self->data->menu);
                check_status(stat, E_OK, stat);

                self->focus = (void *)current_item(self->data->menu);

            }

            stat = wnoutrefresh(self->inner);
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

menus_t *bar_menu_create(int startx, int starty, int height, int width, menus_list_t *list) {

    item_list_t items[2];
    menus_t *self = NULL;
    menus_data_t *data = NULL;

    if ((data = calloc(1, sizeof(menus_data_t)))) {

        data->row = 1;
        data->col = 16;
        data->mark = ">";
        data->callback = self->_show_description;
        data->options = (O_ONEVALUE | O_IGNORECASE | O_SHOWMATCH | O_ROWMAJOR);

        if ((self = menus_create("", startx, starty, height, width, list))) {

            self->data = data;

            SET_ITEM(items[0], WIDGET_M_EVENT, _bar_menu_event, 0, NULL);
            SET_ITEM(items[1], 0, 0, 0, 0);

            menus_override(self, items);

        }

    }

    return self;

}


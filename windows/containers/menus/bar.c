
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2020 by Kevin L. Esteb                       */
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

#include "object.h"
#include "container.h"
#include "menu_priv.h"

require_klass(CONTAINER_KLASS);

/*----------------------------------------------------------------*/
/* private methods                                                */
/*----------------------------------------------------------------*/

static int _show_description(container_t *self) {

    int stat = ERR;
    ITEM *item = NULL;
    const char *description = NULL;
    menu_data_t *data = (menu_data_t *)self->data;

    if ((item = current_item(data->menu)) != NULL) {

        if ((description = item_description(item)) != NULL) {

            wmove(self->area, 1, 1);
            wclrtoeol(self->area);
            waddstr(self->area, description); 
            stat = OK;

        }

    }

    return stat;

}

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

int _bar_menu_event(container_t *self, event_t *event) {

    int stat = ERR;
    ITEM *item = NULL;
    menu_data_t *data = (menu_data_t *)self->data;

    if (data != NULL) {

        if (event->type == EVENT_K_KEYBOARD) {

            KEVENT *kevent = (KEVENT *)event->data;

            switch(kevent->keycode) {
                case KEY_LEFT: {
                    menu_driver(data->menu, REQ_LEFT_ITEM);
                    break;
                }
                case 9:
                case KEY_RIGHT: {
                    menu_driver(data->menu, REQ_RIGHT_ITEM);
                    break;
                }
                case KEY_HOME: {
                    menu_driver(data->menu, REQ_FIRST_ITEM);
                    break;
                }
                case KEY_END: {
                    menu_driver(data->menu, REQ_LAST_ITEM);
                    break;
                }
                case 10:
                case KEY_ENTER: {
                    menu_driver(data->menu, REQ_TOGGLE_ITEM);
                    pos_menu_cursor(data->menu);
                    item = current_item(data->menu);
                    if (item != NULL) {
                        
                    }
                    break;
                }
            }

            _show_description(self);
            pos_menu_cursor(data->menu);

            self->focus = (void *)current_item(data->menu);
            self->data = (void *)data;
            stat = OK;

        }

    }

    return stat;

}

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

container_t *bar_menu_create(int width) {

    item_list_t items[7];
    container_t *self = NULL;
    menu_data_t *data = NULL;

    if ((self = container_create(0, 1, 2, width))) {

        if ((data = (menu_data_t *)calloc(1, sizeof(menu_data_t))) != NULL) {

            data->row = 1;
            data->col = 16;
            data->mark = ">";
            data->callback = _show_description;
            data->options = (O_ONEVALUE | O_IGNORECASE | O_SHOWMATCH | O_ROWMAJOR);

            self->type = CONTAINER_T_MENU;
            self->data = (void *)data;

            SET_ITEM(items[0], CONTAINER_M_DRAW, &_menu_draw, 0, NULL);
            SET_ITEM(items[1], CONTAINER_M_ERASE, &_menu_erase, 0, NULL);
            SET_ITEM(items[2], CONTAINER_M_DESTROY, &_menu_dtor, 0, NULL);
            SET_ITEM(items[3], CONTAINER_M_EVENT, &_bar_menu_event, 0, NULL);
            SET_ITEM(items[4], CONTAINER_M_REFRESH, &_menu_refresh, 0, NULL);
            SET_ITEM(items[5], CONTAINER_M_REMOVE_COMPONENT, &_menu_remove_component, 0, NULL);
            SET_ITEM(items[6], 0, 0, 0, 0);

            container_override(self, items);

        }

    }

    return self;

}


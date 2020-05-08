
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

#include "menus.h"
#include "common.h"
#include "object.h"
#include "container.h"
#include "menus_priv.h"
#include "components/menu/menu_items.h"

require_klass(MENUS_KLASS);

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

int _bar_menu_event(container_t *self, event_t *event) {

    int stat = OK;
    ITEM *item = NULL;
    userptr_data_t *userptr = NULL;
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
                    item = current_item(data->menu);
                    if (item != NULL) {
                        if ((userptr = item_userptr(item)) != NULL) {
                            if (userptr->callback != NULL) {
                                stat = userptr->callback(userptr->data, userptr->data_size);
                            }
                        }
                    }
                    break;
                }
            }

            data->callback(self);
            pos_menu_cursor(data->menu);

            self->focus = (void *)current_item(data->menu);
            self->data = (void *)data;

        }

    }

    return stat;

}

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

container_t *bar_menu_create(int width) {

    item_list_t items[2];
    menus_t *self = NULL;
    menu_data_t *data = NULL;

    if ((self = menus_create(0, 1, 2, width))) {

        if ((data = (menu_data_t *)calloc(1, sizeof(menu_data_t))) != NULL) {

            data->row = 1;
            data->col = 16;
            data->mark = ">";
            data->callback = self->_show_description;
            data->options = (O_ONEVALUE | O_IGNORECASE | O_SHOWMATCH | O_ROWMAJOR);

            CONTAINER(self)->type = CONTAINER_T_MENU;
            CONTAINER(self)->data = (void *)data;

            SET_ITEM(items[0], CONTAINER_M_EVENT, &_bar_menu_event, 0, NULL);
            SET_ITEM(items[1], 0, 0, 0, 0);

            menus_override(self, items);

        }

    }

    return self;

}


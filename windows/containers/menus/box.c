
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
#include "component.h"
#include "menus_priv.h"
#include "components/menu/menu_items.h"

require_klass(MENUS_KLASS);

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

int _box_menu_event(container_t *self, events_t *event) {

    int stat = ERR;
    userptr_data_t *userptr = NULL;
    menu_data_t *data = (menu_data_t *)self->data;

    if (data != NULL) {

        if (event->type == EVENT_K_KEYBOARD) {

            KEVENT *kevent = (KEVENT *)event->data;

            switch(kevent->keycode) {
                case KEY_UP: {
                    menu_driver(data->menu, REQ_UP_ITEM);
                    break;
                }
                case 9:
                case KEY_DOWN: {
                    menu_driver(data->menu, REQ_DOWN_ITEM);
                    break;
                }
                case KEY_LEFT: {
                    menu_driver(data->menu, REQ_LEFT_ITEM);
                    break;
                }
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
                case KEY_NPAGE: {
                    menu_driver(data->menu, REQ_SCR_DPAGE);
                    break;
                }
                case KEY_PPAGE: {
                    menu_driver(data->menu, REQ_SCR_UPAGE);
                    break;
                }
                case 10:
                case KEY_ENTER: {
                    ITEM *item = NULL;
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
            stat = OK;

        }

    }

    return stat;

}

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

menus_t *box_menu_create(int row, int col, int height, int width) {

    item_list_t items[7];
    menus_t *self = NULL;
    menu_data_t *data = NULL;

    if ((self = menus_create(row, col, height, width))) {

        if ((data = (menu_data_t *)calloc(1, sizeof(menu_data_t))) != NULL) {

            data->col = 3;
            data->row = 0;
            data->mark = ">";
            data->callback = self->_show_description;
            data->options = (O_ONEVALUE | O_ROWMAJOR | O_IGNORECASE | 
                             O_SHOWMATCH | O_NONCYCLIC);
            CONTAINER(self)->type = CONTAINER_T_MENU;
            CONTAINER(self)->data = (void *)data;

            SET_ITEM(items[0], CONTAINER_M_EVENT, &_box_menu_event, 0, NULL);
            SET_ITEM(items[1], 0, 0, 0, 0);

            menus_override(self, items);

        }

    }

    return self;

}



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
#include "component.h"
#include "item_list.h"
#include "components/menu/menus.h"

require_klass(CONTAINER_KLASS);

/*----------------------------------------------------------------*/
/* private data                                                   */
/*----------------------------------------------------------------*/

typedef struct _menus_data_s {
    MENU *menu;
    ITEM **items;
} menus_data_t;

/*----------------------------------------------------------------*/
/* private methods                                                */
/*----------------------------------------------------------------*/

static int _menu_remove(container_t *self) {

    int stat = OK;

    if (self->data != NULL) {

        menus_data_t *data = (menus_data_t *)self->data;

        if (data->menu != NULL) {

            unpost_menu(data->menu);
            free_menu(data->menu);

        }

        free(data->items);

    }

    return stat;

}

static int _menu_display(container_t *self) {

    int count = 0;
    int stat = OK;
    ITEM **items = NULL;
    component_t *temp = NULL;
    int size = que_size(&self->components);

    menus_data_t *data = (menus_data_t *)self->data;

    if (data != NULL) {

        if ((items = calloc(size + 1, sizeof(ITEM))) != NULL) {

            for (temp = que_first(&self->components);
                 temp != NULL;
                 temp = que_next(&self->components)) {

                if ((stat = component_draw(temp)) == OK) {

                    menu_data_t *data = (menu_data_t *)temp->data;

                    if (data != NULL) {

                        items[count] = data->item;
                        count++;

                    }

                }

            }

            items[count] = NULL;
            data->items = items;

        }

        if ((data->menu = new_menu(data->items)) != NULL) {

            if ((stat = set_menu_win(data->menu, self->area)) != E_OK) {

                goto fini;

            }

            if ((stat = set_menu_sub(data->menu, 
                 derwin(self->area, self->height - 2, self->width - 2, 0, 0))) 
                != E_OK) {

                goto fini;

            }

            if ((stat = post_menu(data->menu)) != E_OK) {

                goto fini;

            }

            if (self->focus != NULL) {

                ITEM *item = (ITEM *)self->focus;
                stat = set_current_item(data->menu, item);
                if (stat != E_OK) {

                    object_set_error(self, stat);
                    stat = ERR;
                    goto fini;

                }

            } else {

                stat = top_row(data->menu);
                if (stat != E_OK) {

                    object_set_error(self, stat);
                    stat = ERR;
                    goto fini;

                }

            }
            pos_menu_cursor(data->menu);
            curs_set(1);
            
        }

        fini:
        self->data = (void *)data;
        wnoutrefresh(self->area);

    }

    return stat;

}

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

int _menu_dtor(object_t *object) {

    int stat = OK;
    component_t *component = NULL;
    container_t *self = CONTAINER(object);

    /* free local resources here */

    stat = _menu_remove(self);

    while ((component = que_pop_head(&self->components))) {

        component_destroy(component);

    }

    que_init(&self->components);

    if (self->area != NULL) {

        werase(self->area);
        delwin(self->area);

    }

    wnoutrefresh(self->area);

    /* walk the chain, freeing as we go */

    object_demote(object, object_t);
    object_destroy(object);

    return stat;

}

int _menu_draw(container_t *self) {

    return _menu_display(self);
    
}

int _menu_erase(container_t *self) {

    return _menu_remove(self);
    
}

int _menu_refresh(container_t *self) {

    int stat = ERR;

    stat = _menu_remove(self);

    if (stat == OK) {

        stat = _menu_display(self);

    }

    return stat;

}

int _menu_remove_component(container_t *self, component_t *component) {

    int stat = ERR;

    return stat;

}

int _menu_event(container_t *self, event_t *event) {

    int stat = ERR;
    menus_data_t *data = (menus_data_t *)self->data;

    if (data != NULL) {

        if (event->type == EVENT_K_KEYBOARD) {

            KEVENT *kevent = (KEVENT *)event->data;

            switch(kevent->keycode) {
                case KEY_UP: {
                    menu_driver(data->menu, REQ_UP_ITEM);
                    self->focus = (void *)current_item(data->menu);
                    pos_menu_cursor(data->menu);
                    break;
                }
                case 9:
                case KEY_DOWN: {
                    menu_driver(data->menu, REQ_DOWN_ITEM);
                    self->focus = (void *)current_item(data->menu);
                    pos_menu_cursor(data->menu);
                    break;
                }
                case KEY_HOME: {
                    menu_driver(data->menu, REQ_FIRST_ITEM);
                    self->focus = (void *)current_item(data->menu);
                    pos_menu_cursor(data->menu);
                    break;
                }
                case KEY_END: {
                    menu_driver(data->menu, REQ_LAST_ITEM);
                    self->focus = (void *)current_item(data->menu);
                    pos_menu_cursor(data->menu);
                    break;
                }
                case KEY_NPAGE: {
                    menu_driver(data->menu, REQ_SCR_DPAGE);
                    self->focus = (void *)current_item(data->menu);
                    pos_menu_cursor(data->menu);
                    break;
                }
                case KEY_PPAGE: {
                    menu_driver(data->menu, REQ_SCR_UPAGE);
                    self->focus = (void *)current_item(data->menu);
                    pos_menu_cursor(data->menu);
                    break;
                }
                case 10:
                case KEY_ENTER: {
                    ITEM *item = NULL;
                    menu_driver(data->menu, REQ_TOGGLE_ITEM);
                    self->focus = (void *)current_item(data->menu);
                    pos_menu_cursor(data->menu);
                    item = current_item(data->menu);
                    if (item != NULL) {
                        
                    }
                    break;
                }
            }

            self->data = (void *)data;
            stat = OK;

        }

    }

    return stat;

}

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

container_t *menu_create(int row, int col, int height, int width) {

    item_list_t items[7];
    container_t *self = NULL;
    menus_data_t *data = NULL;

    if ((self = container_create(row, col, height, width))) {

        if ((data = (menus_data_t *)calloc(1, sizeof(menus_data_t))) != NULL) {

            self->type = CONTAINER_T_MENU;
            self->data = (void *)data;

            SET_ITEM(items[0], CONTAINER_M_DRAW, &_menu_draw, 0, NULL);
            SET_ITEM(items[1], CONTAINER_M_EVENT, &_menu_event, 0, NULL);
            SET_ITEM(items[2], CONTAINER_M_DESTROY, &_menu_dtor, 0, NULL);
            SET_ITEM(items[3], CONTAINER_M_ERASE, &_menu_erase, 0, NULL);
            SET_ITEM(items[4], CONTAINER_M_REFRESH, &_menu_refresh, 0, NULL);
            SET_ITEM(items[5], CONTAINER_M_REMOVE_COMPONENT, &_menu_remove_component, 0, NULL);
            SET_ITEM(items[6], 0, 0, 0, 0);

            container_override(self, items);

        }

    }

    return self;

}


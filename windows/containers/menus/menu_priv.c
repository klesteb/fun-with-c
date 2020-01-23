
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
#include "menu_priv.h"
#include "components/menu/menus.h"

require_klass(CONTAINER_KLASS);

/*----------------------------------------------------------------*/
/* private methods                                                */
/*----------------------------------------------------------------*/

int _menu_remove(container_t *self) {

    int stat = OK;

    if (self->data != NULL) {

        menu_data_t *data = (menu_data_t *)self->data;

        if (data->menu != NULL) {

            unpost_menu(data->menu);
            free_menu(data->menu);

        }

        free(data->items);

    }

    return stat;

}

int _menu_display(container_t *self) {

    int count = 0;
    int stat = OK;
    ITEM **items = NULL;
    component_t *temp = NULL;
    int size = que_size(&self->components);

    menu_data_t *data = (menu_data_t *)self->data;

    if (data != NULL) {

        if ((items = calloc(size + 1, sizeof(ITEM))) != NULL) {

            for (temp = que_first(&self->components);
                 temp != NULL;
                 temp = que_next(&self->components)) {

                if ((stat = component_draw(temp)) == OK) {

                    menu_item_t *data = (menu_item_t *)temp->data;

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

            if ((stat = set_menu_opts(data->menu, data->options)) != E_OK) {

                object_set_error(self, stat);
                goto fini;

            }

            if ((stat = set_menu_mark(data->menu, data->mark)) != E_OK) {

                object_set_error(self, stat);
                goto fini;

            }

            if ((stat = set_menu_win(data->menu, self->area)) != E_OK) {

                object_set_error(self, stat);
                goto fini;

            }

            if ((stat = set_menu_sub(data->menu, 
                 derwin(self->area, self->height - 2, self->width - 2, 0, 0))) 
                != E_OK) {

                object_set_error(self, stat);
                goto fini;

            }

            if ((stat = post_menu(data->menu)) != E_OK) {

                object_set_error(self, stat);
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


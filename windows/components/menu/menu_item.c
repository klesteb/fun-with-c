
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
#include <errno.h>
#include <menu.h>

#include "menus.h"
#include "colors.h"
#include "misc/misc.h"
#include "component.h"
#include "item_list.h"

require_klass(COMPONENT_KLASS);

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

int _menu_item_dtor(object_t *object) {

    int stat = OK;
    component_t *self = COMPONENT(object);

    /* free local resources here */

    component_erase(self);

    if (self->data != NULL) {

        item_data_t *data = (item_data_t *)self->data;

        if (data->item != NULL) {

            userptr_data_t *junk = item_userptr(data->item);
            free(junk);
            free_item(data->item);

        }

    }

    /* walk the chain, freeing as we go */

    object_demote(object, object_t);
    object_destroy(object);

    return stat;

}

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

component_t *menu_item_create(item_data_t *data) {

    ITEM *temp = NULL;
    item_list_t items[2];
    component_t *self = NULL;
    userptr_data_t *userptr = NULL;

    if ((self = component_create(1, 1, NULL, 0))) {

        self->type = COMPONENT_T_MENU_ITEM;

        errno = 0;
        if ((temp = new_item(data->label, data->description)) != NULL) {

            data->item = temp;
            self->data = (void *)data;
            self->size = sizeof(item_data_t);

            set_item_userptr(temp, NULL);

            errno = 0;
            if ((userptr = calloc(1, sizeof(userptr_data_t))) != NULL) {

                userptr->data = data->data;
                userptr->callback = data->callback;
                userptr->data_size = data->data_size;

                set_item_userptr(temp, userptr);

            } else {

                object_set_error1(self, errno);
                goto fini;

            }

        } else {

            object_set_error1(self, errno);
            goto fini;

        }

        SET_ITEM(items[0], COMPONENT_M_DESTROY, &_menu_item_dtor, 0, NULL);
        SET_ITEM(items[1], 0, 0, 0, 0);

        component_override(self, items);

    }

    fini:
    return self;

}


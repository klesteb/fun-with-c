
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
#include <string.h>
#include <ctype.h>

#include "misc/misc.h"
#include "component.h"
#include "item_list.h"
#include "colors.h"
#include "que_util.h"
#include "components/inkey.h"

require_klass(COMPONENT_KLASS);

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

int _inkey_event(component_t *self, events_t *event) {

    int x;
    int stat = OK;
    inkey_funcs_t *data = self->data;
    int count = (sizeof(inkey_funcs_t) / sizeof(data[0]));

    if (data != NULL) {

        if (event->type == EVENT_K_KEYBOARD) {

            KEVENT *key = (KEVENT *)event->data;

fprintf(stderr, "key = %d\n", key->keycode);
            if (isprint(key->keycode)) {

                for (x = 0; x < count; x++) {

                    if (data[x].keycode == key->keycode) {
                        
                        stat = (*data[x].callback)(data[x].data);

                    }

                }

            }

        }

    }

    return stat;

}

int _inkey_dtor(object_t *object) {

    int stat = OK;
    component_t *self = COMPONENT(object);

    /* free local resources here */

    component_erase(self);
    free(self->data);

    /* walk the chain, freeing as we go */

    object_demote(object, object_t);
    object_destroy(object);

    return stat;

}

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

component_t *inkey_create(inkey_funcs_t *data) {

    item_list_t items[3];
    component_t *input = NULL;

    if ((input = component_create(1, 1, NULL, 0))) {

        input->data = data;
        input->type = COMPONENT_T_INKEY;

        SET_ITEM(items[0], COMPONENT_M_EVENT, &_inkey_event, 0, NULL);
        SET_ITEM(items[1], COMPONENT_M_DESTROY, &_inkey_dtor, 0, NULL);
        SET_ITEM(items[2], 0, 0, 0, 0);

        component_override(input, items);

    }

    return input;

}


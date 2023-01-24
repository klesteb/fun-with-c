
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

require_klass(COMPONENT_KLASS);

/*----------------------------------------------------------------*/
/* private data structure                                         */
/*----------------------------------------------------------------*/

typedef struct input_data_s {
    char *tp;
    char *bp;
    char buf[256];
    int field;
    int defdisp;
    int insert;
} input_data_t;

/*----------------------------------------------------------------*/
/* private methods                                                */
/*----------------------------------------------------------------*/

static char wordchar(void) {

    return 0x17;    /* ^W */

}

static input_data_t *_input_data_create(int width, char *value, int size) {

    input_data_t *data = NULL;

    if ((data = calloc(1, sizeof(input_data_t))) != NULL) {

        data->tp = NULL;
        data->bp = NULL;
        data->field = size;
        data->insert = FALSE;
        data->defdisp = TRUE;
        strncpy(data->buf, value, size);

    }

    return data;
    
}

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

int _input_draw(component_t *self) {

    int stat = OK;
    input_data_t *data = self->data;

    if (data != NULL) {

        wattron(self->area, self->attribute);
        wcoloron(self->area, self->fg, self->bg);

        wprintw(self->area, "%s", data->bp);
        wstandend(self->area);
        wnoutrefresh(self->area);

    }

    return stat;

}

int _input_event(component_t *self, events_t *event) {

    int stat = OK;
    input_data_t *data = self->data;

    if (data != NULL) {

        if (event->type == EVENT_K_KEYBOARD) {

            KEVENT *key = (KEVENT *)event->data;

            switch(key->keycode) {
                case KEY_LEFT:
                    if (data->bp > data->buf)
                        data->bp--;
                    break;
                case KEY_RIGHT:
                    data->defdisp = FALSE;
                    if (data->bp - data->buf < strlen(data->buf))
                        data->bp++;
                    break;

                case '\t':            /* TAB -- because insert
                                         is broken on HPUX */
                case KEY_IC:          /* enter insert mode */
                case KEY_EIC:         /* exit insert mode */
                    data->defdisp = FALSE;
                    data->insert = !data->insert;
                    curs_set(data->insert ? 2 : 1);
                    break;

                default:
                    if (key->keycode == erasechar()) {       /* backspace, ^H */

                        if (data->bp > data->buf) {

                            memmove((void *)(data->bp - 1), 
                                    (const void *)data->bp, strlen(data->bp) + 1);
                            data->bp--;

                        }

                    } else if (key->keycode == killchar()) {   /* ^U */

                        data->bp = data->buf;
                        *data->bp = '\0';

                    } else if (key->keycode == wordchar()) {   /* ^W */

                        data->tp = data->bp;

                        while ((data->bp > data->buf) && (*(data->bp - 1) == ' ')) 
                            data->bp--;

                        while ((data->bp > data->buf) && (*(data->bp - 1) != ' ')) 
                            data->bp--;

                        memmove((void *)data->bp, 
                                (const void *)data->tp, strlen(data->tp) + 1);
                    
                    } else if (isprint(key->keycode)) {
                    
                        if (data->defdisp) {

                            data->bp = data->buf;
                            *data->bp = '\0';
                            data->defdisp = FALSE;

                        }

                        if (data->insert) {
                        
                            if (strlen(data->buf) < data->field - 1) {
                            
                                memmove((void *)(data->bp + 1), 
                                        (const void *)data->bp,
                                        strlen(data->bp) + 1);

                                *data->bp++ = key->keycode;

                            }

                        } else if (data->bp - data->buf < data->field - 1) {
                        
                            /* append new string terminator */

                            if (!*data->bp)
                                data->bp[1] = '\0';
            
                            *data->bp++ = key->keycode;

                        }

                    }
                    break;
            }
            
            stat = OK;

        }

    }

    return stat;

}

int _input_dtor(object_t *object) {

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

component_t *input_create(int width, char *value, int size) {

    item_list_t items[4];
    component_t *input = NULL;
    input_data_t *data = NULL;

    if ((data = _input_data_create(width, value, size))) {

        if ((input = component_create(size, width, NULL, 0))) {

            input->data = data;
            input->type = COMPONENT_T_INPUT;

            SET_ITEM(items[0], COMPONENT_M_DRAW, &_input_draw, 0, NULL);
            SET_ITEM(items[1], COMPONENT_M_EVENT, &_input_event, 0, NULL);
            SET_ITEM(items[2], COMPONENT_M_DESTROY, &_input_dtor, 0, NULL);
            SET_ITEM(items[3], 0, 0, 0, 0);

            component_override(input, items);

        }

    }

    return input;

}


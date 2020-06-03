
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
/* private data                                                   */
/*----------------------------------------------------------------*/

typedef struct config_s {
    int *term;
    int ins;
    int skip;
    int fix;
    int zap;
    int (*filter)();
} config_t;

/*----------------------------------------------------------------*/
/* private methods                                                */
/*----------------------------------------------------------------*/

static int _okfunc(void) {

    return 1;

}

static int _alpha(int ch) {


    return(isalpha(ch));

}

static int _number(int ch) {

    return(isdigit(ch));

}

static int _alpha_numeric(int ch) {

    return(isalnum(ch));

}

static int _upper_case(int ch) {

    return(isupper(ch));

}

static int _lower_case(int ch) {

    return(islower(ch));

}

static int _numeric(int ch) {

    return((isdigit(ch) || ch == '+' || ch == '-' || ch == '.'));

}

static int _boolean(int ch) {

    ch = toupper(ch);

    return((ch == 'Y' || ch == 'N' || ch == 'T' || 
            ch == 'F' || ch == '1' || ch == '0'));

}

static int _time(int ch) {

    return((isdigit(ch) || ch == ':'));

}

static int _date(int ch) {

    return((isdigit(ch) || ch == '/'));

}

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

int _textarea_draw(component_t *self) {

    int stat = ERR;
    wrapped_t *data = self->data;

    if (data != NULL) {

        data->cur_pos = 0;
        self->data = data;

        stat = _data_display(self);

    }

    return stat;

}

int _textarea_event(component_t *self, event_t *event) {

    int cols = 0;
    int rows = 0;
    int stat = ERR;
    wrapped_t *data = self->data;

    if (data != NULL) {

        getmaxyx(self->area, rows, cols);

        if (event->type == EVENT_K_KEYBOARD) {

            KEVENT *key = (KEVENT *)event->data;
            
            switch(key->keycode) {
                case KEY_UP: {
                    if (data->cur_pos < (data->size - 1)) { 
                        data->cur_pos++;
                    }
                    self->data = data;
                    werase(self->area);
                    stat = _data_display(self);
                    wnoutrefresh(self->area);
                    break;
                }
                case KEY_DOWN: {
                    if (data->cur_pos > 0) { 
                        data->cur_pos--;
                    }
                    self->data = data;
                    werase(self->area);
                    stat = _data_display(self);
                    wnoutrefresh(self->area);
                    break;
                }
                case KEY_HOME: {
                    data->cur_pos = 0;
                    self->data = data;
                    werase(self->area);
                    stat = _data_display(self);
                    wnoutrefresh(self->area);
                    break;
                }
                case KEY_END: {
                    data->cur_pos = (data->size - rows) + 1;
                    self->data = data;
                    werase(self->area);
                    stat = _data_display(self);
                    wnoutrefresh(self->area);
                    break;
                }
                case KEY_NPAGE: {
                    data->cur_pos += rows;
                    if (data->cur_pos > rows) {
                        data->cur_pos = (data->size - rows) + 1;
                    }
                    self->data = data;
                    werase(self->area);
                    stat = _data_display(self);
                    wnoutrefresh(self->area);
                    break;
                }
                case KEY_PPAGE: {
                    data->cur_pos -= rows;
                    if (data->cur_pos < 0) {
                        data->cur_pos = 0;
                    }
                    self->data = data;
                    werase(self->area);
                    stat = _data_display(self);
                    wnoutrefresh(self->area);
                    break;
                }

            }

            stat = OK;

        }

    }

    return stat;

}

int _textarea_dtor(object_t *object) {

    int stat = OK;
    component_t *self = COMPONENT(object);

    /* free local resources here */

    component_erase(self);

    if (self->data != NULL)  _data_destroy(self->data);

    /* walk the chain, freeing as we go */

    object_demote(object, object_t);
    object_destroy(object);

    return stat;

}

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

component_t *textedit_create(int width, char *value, int size, int *term, int ins, int skip, int fix, int zap, int (*filter)()) {

    item_list_t items[4];
    wrapped_t *data = NULL;
    component_t *textarea = NULL;
    
    if ((data = _data_create(width, value, size))) {

        if ((textarea = component_create(data->size, width, NULL, 0))) {

            textarea->data = data;
            textarea->type = COMPONENT_T_TEXTAREA;

            SET_ITEM(items[0], COMPONENT_M_DRAW, &_textarea_draw, 0, NULL);
            SET_ITEM(items[1], COMPONENT_M_EVENT, &_textarea_event, 0, NULL);
            SET_ITEM(items[2], COMPONENT_M_DESTROY, &_textarea_dtor, 0, NULL);
            SET_ITEM(items[3], 0, 0, 0, 0);

            component_override(textarea, items);

        }

    }

    return textarea;

}

/*----------------------------------------------------------------*/
/* private methods                                                */
/*----------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2019 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include <ncurses.h>
#include <form.h>
#include <errno.h>

#include "object.h"
#include "container.h"
#include "component.h"
#include "item_list.h"
#include "components/forms/forms.h"

require_klass(CONTAINER_KLASS);

/*----------------------------------------------------------------*/
/* private data                                                   */
/*----------------------------------------------------------------*/

typedef struct _forms_data_s {
    int ins;
    FORM *form;
    FIELD **fields;
} forms_data_t;

/*----------------------------------------------------------------*/
/* private methods                                                */
/*----------------------------------------------------------------*/

#define process_error(stat) {            \
    if ((stat) == E_SYSTEM_ERROR) {      \
        object_set_error1(self, errno);  \
    } else {                             \
        object_set_error1(self, (stat)); \
    }                                    \
    stat = ERR;                          \
    goto fini;                           \
}

static int _focus_first_field(container_t *self) {

    int stat = OK;
    component_t *temp = NULL;
    field_data_t *data = NULL;
    forms_data_t *forms = NULL;

    if (self->data != NULL) {

        forms = (forms_data_t *)self->data;

        for (temp = que_first(&self->components);
             temp != NULL;
             temp = que_next(&self->components)) {

            if (temp->type == COMPONENT_T_FORM_FIELD) {

                data = (field_data_t *)temp->data;

                if (data != NULL) {

                    errno = 0;
                    if ((stat = set_current_field(forms->form, data->field)) != E_OK) {

                        process_error(stat);

                    }

                    errno = 0;
                    if ((stat = form_driver(forms->form, REQ_END_LINE)) != E_OK) {

                        process_error(stat);

                    }

                    errno = 0;
                    if ((stat = pos_form_cursor(forms->form)) != E_OK) {

                        process_error(stat);

                    }

                    self->focus = (void *)data->field;
                    stat = curs_set(1);
                    break;

                }

            }

        }

    }

    fini:
    return stat;

}

static int _form_remove(container_t *self) {

    int stat = OK;

    if (self->data != NULL) {

        forms_data_t *data = (forms_data_t *)self->data;

        if (data->form != NULL) {

            unpost_form(data->form);
            free_form(data->form);

        }

        free(data->fields);

    }

    return stat;
    
}

static int _form_display(container_t *self) {

    int count = 0;
    int stat = OK;
    FIELD **fields = NULL;
    component_t *temp = NULL;
    int size = que_size(&self->components);

    forms_data_t *data = (forms_data_t *)self->data;

    if (data != NULL) {

        if ((fields = calloc(size + 1, sizeof(FIELD))) != NULL) {

            for (temp = que_first(&self->components);
                 temp != NULL;
                 temp = que_next(&self->components)) {

                if ((stat = component_draw(temp)) == OK) {

                    field_data_t *data = (field_data_t *)temp->data;

                    if (data != NULL) {

                        fields[count] = data->field;
                        count++;

                    }

                }

            }

            fields[count] = NULL;
            data->fields = fields;

        }

        if ((data->form = new_form(data->fields)) != NULL) {

            errno = 0;
            if ((stat = set_form_win(data->form, self->area)) != E_OK) {

                process_error(stat);

            }

            errno = 0;
            if ((stat = set_form_sub(data->form, 
                 derwin(self->area, self->height - 2, self->width - 2, 0, 0))) 
                != E_OK) {

                process_error(stat);

            }

            errno = 0;
            if ((stat = post_form(data->form)) != E_OK) {

                process_error(stat);

            }

            stat = _focus_first_field(self);

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

int _form_dtor(object_t *object) {

    int stat = OK;
    component_t *component = NULL;
    container_t *self = CONTAINER(object);

    /* free local resources here */

    stat = _form_remove(self);

    while ((component = que_pop_head(&self->components))) {

        component_destroy(component);

    }

    que_init(&self->components);
        
    if (self->area != NULL) {

        werase(self->area);
        delwin(self->area);

    }

    /* walk the chain, freeing as we go */

    object_demote(object, object_t);
    object_destroy(object);

    return stat;

}

int _form_draw(container_t *self) {

    return _form_display(self);
    
}

int _form_erase(container_t *self) {

    return _form_remove(self);
    
}

int _form_refresh(container_t *self) {

    int stat = ERR;

    stat = _form_remove(self);
    if (stat == OK) {
        
        stat = _form_display(self);
        
    }

    return stat;

}

int _form_remove_component(container_t *self, component_t *component) {

    int stat = ERR;

    return stat;

}

int _form_event(container_t *self, events_t *event) {

    int stat = ERR;
    forms_data_t *data = (forms_data_t *)self->data;

    if (data != NULL) {

        if (event->type == EVENT_K_KEYBOARD) {

            KEVENT *kevent = (KEVENT *)event->data;

            switch(kevent->keycode) {
                case KEY_UP: {
                    self->focus = (void *)current_field(data->form);
                    /* sync the form to display */
                    form_driver(data->form, REQ_NEXT_FIELD);
                    form_driver(data->form, REQ_PREV_FIELD);
                    /* move to the previous field */
                    form_driver(data->form, REQ_PREV_FIELD);
                    form_driver(data->form, REQ_END_LINE);
                    pos_form_cursor(data->form);
                    break;
                }
                case KEY_DOWN: {
                    self->focus = (void *)current_field(data->form);
                    /* sync the form to display */
                    form_driver(data->form, REQ_NEXT_FIELD);
                    form_driver(data->form, REQ_PREV_FIELD);
                    /* move to the next field */
                    form_driver(data->form, REQ_NEXT_FIELD);
                    form_driver(data->form, REQ_END_LINE);
                    pos_form_cursor(data->form);
                    break;
                }
                case KEY_LEFT: {
                    self->focus = (void *)current_field(data->form);
                    form_driver(data->form, REQ_PREV_CHAR);
                    break;
                }
                case KEY_RIGHT: {
                    self->focus = (void *)current_field(data->form);
                    form_driver(data->form, REQ_NEXT_CHAR);
                    break;
                }
                case KEY_BACKSPACE: {
                    self->focus = (void *)current_field(data->form);
                    form_driver(data->form, REQ_DEL_PREV);
                    break;
                }
                case KEY_DC: {
                    self->focus = (void *)current_field(data->form);
                    form_driver(data->form, REQ_DEL_CHAR);
                    break;
                }
                case KEY_IC: {
                    self->focus = (void *)current_field(data->form);
                    data->ins = !data->ins;
                    if (data->ins) {
                        form_driver(data->form, REQ_INS_MODE);
                    } else {
                        form_driver(data->form, REQ_OVL_MODE);
                    }
                    break;
                }
                case KEY_HOME: {
                    self->focus = (void *)current_field(data->form);
                    /* sync the form to display */
                    form_driver(data->form, REQ_NEXT_FIELD);
                    form_driver(data->form, REQ_PREV_FIELD);
                    /* move to the first field */
                    form_driver(data->form, REQ_FIRST_FIELD);
                    form_driver(data->form, REQ_END_LINE);
                    pos_form_cursor(data->form);
                    break;
                }
                case KEY_END: {
                    self->focus = (void *)current_field(data->form);
                    /* sync the form to display */
                    form_driver(data->form, REQ_NEXT_FIELD);
                    form_driver(data->form, REQ_PREV_FIELD);
                    /* move to the last field */
                    form_driver(data->form, REQ_LAST_FIELD);
                    form_driver(data->form, REQ_END_LINE);
                    pos_form_cursor(data->form);
                    break;
                }
                case KEY_NPAGE: {
                    self->focus = (void *)current_field(data->form);
                    form_driver(data->form, REQ_NEXT_PAGE);
                    break;
                }
                case KEY_PPAGE: {
                    self->focus = (void *)current_field(data->form);
                    form_driver(data->form, REQ_PREV_PAGE);
                    break;
                }
                case 10:
                case KEY_ENTER: {
                    self->focus = (void *)current_field(data->form);
                    /* sync the form to display */
                    form_driver(data->form, REQ_NEXT_FIELD);
                    form_driver(data->form, REQ_PREV_FIELD);
                    /* move to the next field */
                    form_driver(data->form, REQ_NEXT_FIELD);
                    form_driver(data->form, REQ_END_LINE);
                    pos_form_cursor(data->form);
                    break;
                }
                default: {
                    self->focus = (void *)current_field(data->form);
                    form_driver(data->form, kevent->keycode);
                    /* sync the form to display */
                    form_driver(data->form, REQ_NEXT_FIELD);
                    form_driver(data->form, REQ_PREV_FIELD);
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

container_t *form_create(int row, int col, int height, int width) {

    item_list_t items[7];
    container_t *self = NULL;
    forms_data_t *data = NULL;

    if ((self = container_create(row, col, height, width))) {

        if ((data = (forms_data_t *)calloc(1, sizeof(forms_data_t))) != NULL) {

            data->ins = 0;

            self->type = CONTAINER_T_FORM;
            self->data = (void *)data;

            SET_ITEM(items[0], CONTAINER_M_DRAW, &_form_draw, 0, NULL);
            SET_ITEM(items[1], CONTAINER_M_EVENT, &_form_event, 0, NULL);
            SET_ITEM(items[2], CONTAINER_M_DESTROY, &_form_dtor, 0, NULL);
            SET_ITEM(items[3], CONTAINER_M_ERASE, &_form_erase, 0, NULL);
            SET_ITEM(items[4], CONTAINER_M_REFRESH, &_form_refresh, 0, NULL);
            SET_ITEM(items[5], CONTAINER_M_REMOVE_COMPONENT, &_form_remove_component, 0, NULL);
            SET_ITEM(items[6], 0, 0, 0, 0);

            container_override(self, items);

        }

    }

    return self;

}


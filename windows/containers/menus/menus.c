
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
#include <panel.h>
#include <errno.h>

#include "when.h"
#include "menus.h"
#include "events.h"
#include "object.h"
#include "container.h"
#include "component.h"
#include "item_list.h"
#include "menus_priv.h"
#include "error_codes.h"
#include "components/menu/menu_items.h"

require_klass(OBJECT_KLASS);
require_klass(CONTAINER_KLASS);

/*----------------------------------------------------------------*/
/* klass defination                                               */
/*----------------------------------------------------------------*/

int _menus_ctor(object_t *object, item_list_t *);
int _menus_dtor(object_t *);
int _menus_compare(menus_t *, menus_t *);
int _menus_override(menus_t *, item_list_t *);

int _menus_draw(container_t *);
int _menus_erase(container_t *);
int _menus_refresh(container_t *);
int _menus_event(container_t *, event_t *);
int _menus_show_description(container_t *);
int _menus_remove_component(container_t *, component_t *);

static int _menus_remove(container_t *self);
static int _menus_display(container_t *self);

declare_klass(MENUS_KLASS) {
    .size = KLASS_SIZE(menus_t),
    .name = KLASS_NAME(menus_t),
    .ctor = _menus_ctor,
    .dtor = _menus_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

menus_t *menus_create(int row, int col, int height, int width) {

    int stat = ERR;
    item_list_t items[6];
    menus_t *self = NULL;

    SET_ITEM(items[0], CONTAINER_K_WIDTH, &width, sizeof(int), NULL);
    SET_ITEM(items[1], CONTAINER_K_HEIGHT, &height, sizeof(int), NULL);
    SET_ITEM(items[2], CONTAINER_K_ROW, &row, sizeof(int), NULL);
    SET_ITEM(items[3], CONTAINER_K_COL, &col, sizeof(int), NULL);
    SET_ITEM(items[4], 0, 0, 0, 0);

    self = (menus_t *)object_create(MENUS_KLASS, items, &stat);

    return self;

}

int menus_destroy(menus_t *self) {

    int stat = OK;

    when_error {
        
        if (self != NULL) {

            if (object_assert(self, menus_t)) {

                stat = self->dtor(OBJECT(self));
                check_status(stat, OK, E_INVOPS);

            } else {

                cause_error(E_INVOBJ);

            }

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int menus_compare(menus_t *us, menus_t *them) { 

    int stat = OK;

    when_error {

        if ((us != NULL) && (them != NULL)) {

            stat = container_compare(CONTAINER(us), CONTAINER(them));
            check_status(stat, OK, E_NOTSAME);

            stat = us->_compare(us, them);
            check_status(stat, OK, E_NOTSAME);

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(us, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int menus_override(menus_t *self, item_list_t *items) {

    int stat = OK;

    when_error {
        
        if ((self != NULL) && (items != NULL)) {

            stat = self->_override(self, items);
            check_status(stat, OK, E_INVOPS);

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int menus_erase(menus_t *self) {

    int stat = OK;

    when_error {

        stat = container_erase(CONTAINER(self));
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int menus_draw(menus_t *self) {

    int stat = OK;

    when_error {

        stat = container_draw(CONTAINER(self));
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int menus_event(menus_t *self, event_t *event) {

    int stat = OK;

    when_error {

        stat = container_event(CONTAINER(self), event);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int menus_refresh(menus_t *self) {

    int stat = OK;

    when_error {

        stat = container_refresh(CONTAINER(self));
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int menus_add_component(menus_t *self, component_t *component) {

    int stat = OK;

    when_error {

        stat = container_add_component(CONTAINER(self), component);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int menus_remove_component(menus_t *self, component_t *component) {

    int stat = OK;

    when_error {

        stat = container_remove_component(CONTAINER(self), component);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int menus_show_description(menus_t *self) {

    int stat = OK;

    when_error {

        if (self != NULL) {
            
            container_t *base = CONTAINER(self);
            stat = self->_show_description(base);
            check_status(stat, OK, E_INVOPS);

        } else {

            cause_error(E_INVPARM);

        }
    
        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

int _menus_ctor(object_t *object, item_list_t *items) {

    int stat = ERR;
    menus_t *self = NULL;
    container_t *base = NULL;

    if (object != NULL) {

        /* initialize our base klass here */

        CONTAINER_KLASS->ctor(object, items); 

        /* overrides to the base initialization */

        base = CONTAINER(object);
        base->_draw = _menus_draw;
        base->_event = _menus_event;
        base->_erase = _menus_erase;
        base->_refresh = _menus_refresh;
        base->_remove_component = _menus_remove_component;

        /* initialize our derived klass here */

        self = MENUS(object);

        /* assign our methods here */

        self->ctor = _menus_ctor;
        self->dtor = _menus_dtor;

        self->_compare = _menus_compare;
        self->_override = _menus_override;
        self->_show_description = _menus_show_description;

        stat = OK;

    }

    return stat;

}

int _menus_dtor(object_t *object) {

    int stat = OK;
    component_t *component = NULL;
    container_t *base = CONTAINER(object);

    /* free local resources here */

    while ((component = que_pop_head(&base->components))) {

        component_destroy(component);

    }

    que_init(&base->components);

    if (base->area != NULL) {
        
        werase(base->area);
        delwin(base->area);
        
    }

    /* walk the chain, freeing as we go */

    object_demote(object, object_t);
    object_destroy(object);

    return stat;

}

int _menus_compare(menus_t *us, menus_t *them) {

    int stat = ERR;

    if (us->_show_description == them->_show_description) {

        stat = OK;

    }

    return stat;

}

int _menus_override(menus_t *self, item_list_t *items) {

    int x;
    int stat = ERR;
    container_t *base = CONTAINER(self);

    for (x = 0;; x++) {

        if ((items[x].buffer_length == 0) &&
            (items[x].item_code == 0)) break;

        switch(items[x].item_code) {
            case CONTAINER_M_DESTROY: {
                self->dtor = items[x].buffer_address;
                stat = OK;
                break;
            }
            case CONTAINER_M_DRAW: {
                base->_draw = items[x].buffer_address;
                stat = OK;
                break;
            }
            case CONTAINER_M_EVENT: {
                base->_event = items[x].buffer_address;
                stat = OK;
                break;
            }
            case CONTAINER_M_ERASE: {
                base->_erase = items[x].buffer_address;
                stat = OK;
                break;
            }
            case CONTAINER_M_REFRESH: {
                base->_refresh = items[x].buffer_address;
                stat = OK;
                break;
            }
            case CONTAINER_M_ADD_COMPONENT: {
                base->_add_component = items[x].buffer_address;
                stat = OK;
                break;
            }
            case CONTAINER_M_REMOVE_COMPONENT: {
                base->_remove_component = items[x].buffer_address;
                stat = OK;
                break;
            }
            case MENUS_M_SHOW_DESCRIPTION: {
                self->_show_description = items[x].buffer_address;
                stat = OK;
                break;
            }
        }

    }

    return stat;

}

int _menus_show_description(container_t *self) {

    int stat = ERR;
    ITEM *item = NULL;
    event_t *event = NULL;
    const char *description = NULL;
    menu_data_t *data = (menu_data_t *)self->data;

    if ((item = current_item(data->menu)) != NULL) {

        if ((description = item_description(item)) != NULL) {

            if ((event = calloc(1, sizeof(event_t))) != NULL) {

                event->type = EVENT_K_MESSAGE;
                event->data = (void *)strdup(description);

                /* stat = workbench_inject_event(wb, event); */

            }

        }

    }

    return stat;

}

/*----------------------------------------------------------------*/
/* container overrides                                            */
/*----------------------------------------------------------------*/

int _menus_draw(container_t *self) {

    return _menus_display(self);
    
}

int _menus_erase(container_t *self) {

    return _menus_remove(self);
    
}

int _menus_refresh(container_t *self) {

    int stat = ERR;

    stat = _menus_remove(self);

    if (stat == OK) {

        stat = _menus_display(self);

    }

    return stat;

}

int _menus_remove_component(container_t *self, component_t *component) {

    int stat = ERR;

    return stat;

}

int _menus_event(container_t *self, event_t *event) {

    int stat = ERR;

    return stat;

}

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

static int _menus_display(container_t *self) {

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

                    item_data_t *data = (item_data_t *)temp->data;

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

            errno = 0;
            if ((stat = set_menu_opts(data->menu, data->options)) != E_OK) {

                process_error(stat);

            }

            errno = 0;
            if ((stat = set_menu_mark(data->menu, data->mark)) != E_OK) {

                process_error(stat);

            }

            errno = 0;
            if ((stat = set_menu_format(data->menu, data->row, data->col)) != E_OK) {

                process_error(stat);

            }

            errno = 0;
            if ((stat = set_menu_win(data->menu, self->area)) != E_OK) {

                process_error(stat);

            }

            errno = 0;
            if ((stat = set_menu_sub(data->menu, 
                 derwin(self->area, self->height - 2, self->width - 2, 0, 0))) 
                != E_OK) {

                process_error(stat);

            }

            errno = 0;
            if ((stat = post_menu(data->menu)) != E_OK) {

                process_error(stat);

            }

            errno = 0;
            if (self->focus != NULL) {

                ITEM *item = (ITEM *)self->focus;
                stat = set_current_item(data->menu, item);
                if (stat != E_OK) {

                    process_error(stat);

                }

            } else {

                stat = set_current_item(data->menu, items[0]);
                if (stat != E_OK) {

                    process_error(stat);

                }

            }

            if (data->callback != NULL) {

                stat = (*data->callback)(self);

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

static int _menus_remove(container_t *self) {

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


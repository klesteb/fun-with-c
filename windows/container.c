
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
#include <panel.h>

#include "when.h"
#include "events.h"
#include "object.h"
#include "container.h"
#include "component.h"
#include "item_list.h"
#include "error_codes.h"

require_klass(OBJECT_KLASS);

/*----------------------------------------------------------------*/
/* klass defination                                               */
/*----------------------------------------------------------------*/

int _container_ctor(object_t *object, item_list_t *);
int _container_dtor(object_t *);
int _container_draw(container_t *);
int _container_erase(container_t *);
int _container_refresh(container_t *);
int _container_event(container_t *, events_t *);
int _container_compare(container_t *, container_t *);
int _container_override(container_t *, item_list_t *);
int _container_add_component(container_t *, component_t *);
int _container_remove_component(container_t *, component_t *);

declare_klass(CONTAINER_KLASS) {
    .size = KLASS_SIZE(container_t),
    .name = KLASS_NAME(container_t),
    .ctor = _container_ctor,
    .dtor = _container_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

container_t *container_create(int row, int col, int height, int width) {

    int stat = ERR;
    item_list_t items[6];
    container_t *self = NULL;

    SET_ITEM(items[0], CONTAINER_K_WIDTH, &width, sizeof(int), NULL);
    SET_ITEM(items[1], CONTAINER_K_HEIGHT, &height, sizeof(int), NULL);
    SET_ITEM(items[2], CONTAINER_K_ROW, &row, sizeof(int), NULL);
    SET_ITEM(items[3], CONTAINER_K_COL, &col, sizeof(int), NULL);
    SET_ITEM(items[4], 0, 0, 0, 0);

    self = (container_t *)object_create(CONTAINER_KLASS, items, &stat);

    return self;

}

int container_destroy(container_t *self) {

    int stat = OK;

    when_error {
        
        if (self != NULL) {

            if (object_assert(self, container_t)) {

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

int container_compare(container_t *us, container_t *them) { 

    int stat = OK;

    when_error {

        if ((us != NULL) && (them != NULL)) {

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

int container_override(container_t *self, item_list_t *items) {

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

int container_erase(container_t *self) {

    int stat = OK;

    when_error {

        if (self != NULL) {

            stat = self->_erase(self);
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

int container_draw(container_t *self) {

    int stat = OK;

    when_error {

        if (self != NULL) {

            stat = self->_draw(self);
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

int container_event(container_t *self, events_t *event) {

    int stat = OK;

    when_error {

        if ((self != NULL) && (event != NULL)) {

            stat = self->_event(self, event);
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

int container_refresh(container_t *self) {

    int stat = OK;

    when_error {

        if (self != NULL) {

            stat = self->_refresh(self);
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

int container_add_component(container_t *self, component_t *component) {

    int stat = OK;

    when_error {

        if ((self != NULL) && (component != NULL)) {

            stat = self->_add_component(self, component);
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

int container_remove_component(container_t *self, component_t *component) {

    int stat = OK;

    when_error {

        if ((self != NULL) && (component != NULL)) {

            stat = self->_remove_component(self, component);
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

int _container_ctor(object_t *object, item_list_t *items) {

    int col = 0;
    int row = 0;
    int stat = ERR;
    int height = 1;
    int width = 32;
    container_t *self = NULL;

    if (object != NULL) {

        /* capture our items */

        if (items != NULL) {

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                switch(items[x].item_code) {
                    case CONTAINER_K_ROW: {
                        memcpy(&row, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case CONTAINER_K_COL: {
                        memcpy(&col, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case CONTAINER_K_HEIGHT: {
                        memcpy(&height, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case CONTAINER_K_WIDTH: {
                        memcpy(&width, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                }

            }

        }

        /* initilize our base klass here */

        object_set_error1(object, 0);

        /* initialize our derived klass here */

        self = CONTAINER(object);

        /* assign our methods here */

        self->ctor = _container_ctor;
        self->dtor = _container_dtor;

        self->_draw = _container_draw;
        self->_erase = _container_erase;
        self->_event = _container_event;
        self->_compare = _container_compare;
        self->_override = _container_override;
        self->_add_component = _container_add_component;
        self->_remove_component = _container_remove_component;

        /* initialize internal variables here */

        self->type = 0;
        self->col = col;
        self->row = row;
        self->data = NULL;
        self->focus = NULL;
        self->width = width;
        self->height = height;

        que_init(&self->components);
        
        stat = OK;

    }

    return stat;

}

int _container_dtor(object_t *object) {

    int stat = OK;
    component_t *component = NULL;
    container_t *self = CONTAINER(object);

    /* free local resources here */

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

int _container_compare(container_t *us, container_t *them) {

    int stat = ERR;

    if (((object_compare(OBJECT(us), OBJECT(them))) == 0) &&
        (us->ctor == them->ctor) &&
        (us->dtor == them->dtor) &&
        (us->_draw == them->_draw) &&
        (us->_erase == them->_erase) &&
        (us->_event == them->_event) &&
        (us->_refresh == them->_refresh) &&
        (us->_compare == them->_compare) &&
        (us->_override == them->_override) &&
        (us->_add_component == them->_add_component) &&
        (us->_remove_component == them->_remove_component) &&
        (us->col == them->col) &&
        (us->row == them->row) &&
        (us->width == them->width) &&
        (us->height == them->height)) {

        stat = OK;

    }

    return stat;

}

int _container_erase(container_t *self) {

    int stat = ERR;
    component_t *component = NULL;

    if (self->area != NULL) {

        for (component = que_last(&self->components);
             component != NULL;
             component = que_prev(&self->components)) {

            component_erase(component);

        }

        stat = wclear(self->area);
        wnoutrefresh(self->area);

    }

    return stat;

}

int _container_refresh(container_t *self) {

    int stat = OK;
    component_t *component = NULL;

    if (self->area != NULL) {

        for (component = que_last(&self->components);
             component != NULL;
             component = que_prev(&self->components)) {

fprintf(stderr, "found a component\n");
fprintf(stderr, "component %p\n", component);            
            if ((stat = component_refresh(component)) != OK) {
fprintf(stderr, "component stat %d\n", stat);

                break;

            }

        }

        wnoutrefresh(self->area);

    }

    return stat;

}

int _container_add_component(container_t *self, component_t *component) {

    int stat = ERR;

    if ((stat = que_push_tail(&self->components, component)) == QUE_OK) {

        stat = OK;

    }

    return stat;

}

int _container_remove_component(container_t *self, component_t *component) {

    int stat = ERR;
    component_t *temp = NULL;

    for (temp = que_first(&self->components);
         temp != NULL;
         temp = que_next(&self->components)) {

        if ((component_compare(component, temp) == OK)) {

            component_t *junk = que_delete(&self->components);
            stat = component_destroy(junk);
            break;

        }

    }

    if (que_empty(&self->components)) {

        que_init(&self->components);

    }

    return stat;

}

int _container_override(container_t *self, item_list_t *items) {
    
    int x;
    int stat = ERR;

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
                self->_draw = items[x].buffer_address;
                stat = OK;
                break;
            }
            case CONTAINER_M_EVENT: {
                self->_event = items[x].buffer_address;
                stat = OK;
                break;
            }
            case CONTAINER_M_ERASE: {
                self->_erase = items[x].buffer_address;
                stat = OK;
                break;
            }
            case CONTAINER_M_REFRESH: {
                self->_refresh = items[x].buffer_address;
                stat = OK;
                break;
            }
            case CONTAINER_M_ADD_COMPONENT: {
                self->_add_component = items[x].buffer_address;
                stat = OK;
                break;
            }
            case CONTAINER_M_REMOVE_COMPONENT: {
                self->_remove_component = items[x].buffer_address;
                stat = OK;
                break;
            }
        }
        
    }
    
    return stat;
    
}

int _container_draw(container_t *self) {

    /* must be overridden in a derived klass */

    return OK;

}

int _container_event(container_t *self, events_t *event) {

    /* must be overridden in a derived klass */

    return OK;

}


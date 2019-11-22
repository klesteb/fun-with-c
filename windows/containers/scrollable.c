
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

#include "object.h"
#include "container.h"
#include "component.h"
#include "item_list.h"
#include "que_util.h"

require_klass(CONTAINER_KLASS);

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

int _scrollable_draw(container_t *self) {

    int stat = OK;
    component_t *temp = NULL;

    if ((temp = que_first(&self->components)) != NULL) {

        component_set_area(temp, self->area);
        component_set_metrics(temp, 0, 0);

        stat = component_draw(temp);

    }

    return stat;

}

int _scrollable_refresh(container_t *self) {

    int stat = ERR;
    component_t *temp = NULL;

    if ((temp = que_first(&self->components)) != NULL) {

        stat = component_refresh(temp);

    }

    return stat;

}

int _scrollable_event(container_t *self, event_t *event) {

    int stat = ERR;
    component_t *temp = NULL;

    if ((temp = que_first(&self->components)) != NULL) {

        stat = component_event(temp, event);

    }

    wnoutrefresh(self->area);

    return stat;

}

int _scrollable_add_component(container_t *self, component_t *component) {

    int stat = ERR;

    /* there can only be one component of type textarea */

    if ((component->type == COMPONENT_T_TEXTAREA) &&
        (que_size(&self->components) < 1)) {

        if ((stat = que_push_tail(&self->components, component)) == QUE_OK) {

            stat = OK;

        }

    }

    return stat;

}

int _scrollable_dtor(object_t *object) {

    int stat = OK;
    component_t *component = NULL;
    container_t *self = CONTAINER(object);

    /* free local resources here */

    component = que_pop_head(&self->components);
    component_destroy(component);

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

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

container_t *scrollable_create(int row, int col, int height, int width) {

    item_list_t items[6];
    container_t *scrollable = NULL;

    if ((scrollable = container_create(row, col, height, width))) {

        scrollable->type = CONTAINER_T_SCROLLABLE;

        SET_ITEM(items[0], CONTAINER_M_DRAW, &_scrollable_draw, 0, NULL);
        SET_ITEM(items[1], CONTAINER_M_EVENT, &_scrollable_event, 0, NULL);
        SET_ITEM(items[2], CONTAINER_M_DESTROY, &_scrollable_dtor, 0, NULL);
        SET_ITEM(items[3], CONTAINER_M_REFRESH, &_scrollable_refresh, 0, NULL);
        SET_ITEM(items[4], CONTAINER_M_ADD_COMPONENT, &_scrollable_add_component, 0, NULL);
        SET_ITEM(items[5], 0, 0, 0, 0);

        container_override(scrollable, items);

    }

    return scrollable;

}


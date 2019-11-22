
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

#include "object.h"
#include "factory.h"

require_klass(OBJECT_KLASS);

/*----------------------------------------------------------------*/
/* klass defination                                               */
/*----------------------------------------------------------------*/

int _factory_ctor(object_t *object, item_list_t *);
int _factory_dtor(object_t *);

int  _factory_add(factory_t *, worker_t *);
int  _factory_remove(factory_t *, worker_t *);
int  _factory_override(factory_t *, item_list_t *);
void *_factory_dispatch(factory_t *, int, void *, int);

declare_klass(FACTORY_KLASS) {
    .size = KLASS_SIZE(factory_t),
    .name = KLASS_NAME(factory_t),
    .ctor = _factory_ctor,
    .dtor = _factory_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

factory_t *factory_create() {

    int stat = ERR;
    factory_t *self = NULL;

    self = (factory_t *)object_create(FACTORY_KLASS, NULL, &stat);

    return self;

}

int factory_destroy(factory_t *self) {

    int stat = ERR;

    if (self != NULL) {

        if (object_assert(self, factory_t)) {

            stat = self->dtor((object_t *)self);

        }

    }

    return stat;

}

int factory_add(factory_t *self, worker_t *worker) {

    int stat = ERR;

    if (self != NULL) {

        stat = self->_add(self, worker);

    }

    return stat;

}

int factory_remove(factory_t *self, worker_t *worker) {

    int stat = ERR;

    if (self != NULL) {

        stat = self->_remove(self, worker);

    }

    return stat;

}

void *factory_dispatch(factory_t *self, int type, void *data, int size) {

    void *result = NULL;

    if (self != NULL) {

        result = self->_dispatch(self, type, data, size);

    }

    return result;

}

int factory_override(factory_t *self, item_list_t *items) {

    int stat = ERR;

    if (self != NULL) {

        stat = self->_override(self, items);

    }

    return stat;

}

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

int _factory_ctor(object_t *object, item_list_t *items) {

    int stat = ERR;
    factory_t *self = NULL;

    if (object != NULL) {

        /* initilize our base klass here */

        /* initialize our derived klass here */

        self = FACTORY_OBJECT(object);

        /* assign our methods here */

        self->ctor = _factory_ctor;
        self->dtor = _factory_dtor;

        self->_add = _factory_add;
        self->_remove = _factory_remove;
        self->_dispatch = _factory_dispatch;
        self->_override = _factory_override;

        /* initialize internal variables here */

        que_init(&self->workers);

        stat = OK;

    }

    return stat;

}

int _factory_dtor(object_t *object) {

    int stat = OK;
    worker_t *worker = NULL;

    /* free local resources here */

    factory_t *self = FACTORY_OBJECT(object);

    while ((worker = (worker_t *)que_pop_head(&self->workers))) {

        worker_destroy(worker);

    }

    que_init(&self->workers);

    /* walk the chain, freeing as we go */

    object_demote(object, object_t);
    object_destroy(object);

    return stat;

}

int _factory_override(factory_t *self, item_list_t *items) {

    int stat = ERR;

    if (items != NULL) {

        stat = OK;

    }

    return stat;

}

int _factory_add(factory_t *self, worker_t *worker) {

    int stat = ERR;

    if (worker != NULL) {

        stat = que_push_tail(&self->workers, worker);

    }

    return stat;

}

int _factory_remove(factory_t *self, worker_t *worker) {

    int stat = ERR;
    worker_t *temp = NULL;

    if (worker != NULL) {

        for (temp = (worker_t *)que_first(&self->workers);
             temp != NULL;
             temp = (worker_t *)que_next(&self->workers)) {

            if ((worker_compare(temp, worker)) == 0) {

                worker_t *junk = que_delete(&self->workers);
                stat = worker_destroy(junk);

            }

        }

    }

    if (que_empty(&self->workers)) {

        que_init(&self->workers);

    }

    return stat;

}

void *_factory_dispatch(factory_t *self, int type, void *data, int size) {

    void *result = NULL;
    worker_t *temp = NULL;

    if (data != NULL) {

        for (temp = (worker_t *)que_first(&self->workers);
             temp != NULL;
             temp = (worker_t *)que_next(&self->workers)) {

            if ((worker_match(temp, type)) == 0) {

                result = worker_perform(temp, data, size);
                break;

            }

        }

    }

    return result;

}


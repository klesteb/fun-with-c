
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
#include "worker.h"

require_klass(OBJECT_KLASS);

/*----------------------------------------------------------------*/
/* klass methods                                                  */
/*----------------------------------------------------------------*/

int _worker_ctor(object_t *, item_list_t *);
int _worker_dtor(object_t *);

int _worker_compare(worker_t *, worker_t *);
int _worker_override(worker_t *, item_list_t *);
int  _worker_match(worker_t *, int);
void *_worker_perform(worker_t *, void *, int);

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

declare_klass(WORKER_KLASS) {
    .size = KLASS_SIZE(worker_t),
    .name = KLASS_NAME(worker_t),
    .ctor = _worker_ctor,
    .dtor = _worker_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

worker_t *worker_create(item_list_t *items) {

    int stat = ERR;
    worker_t *self = NULL;

    self = (worker_t *)object_create(WORKER_KLASS, items, &stat);

    return self;

}

int worker_destroy(worker_t *self) {

    int stat = -1;

    if (self != NULL) {

        if (object_assert(self, worker_t)) {

            stat = self->dtor(OBJECT(self));

        }

    }

    return stat;

}

int worker_match(worker_t *self, int type) {

    int stat = -1;

    if (self != NULL) {

        stat = self->_match(self, type);

    }

    return stat;

}

void *worker_perform(worker_t *self, void *data, int size) {

    void *result = NULL;

    if (self != NULL) {

        result = self->_perform(self, data, size);

    }

    return result;

}

int worker_override(worker_t *self, item_list_t *items) {

    int stat = -1;

    if (self != NULL) {

        stat = self->_override(self, items);

    }

    return stat;

}

int worker_compare(worker_t *us, worker_t *them) {

    int stat = -1;

    if (us != NULL) {

        if (object_assert(them, worker_t)) {

            stat = us->_compare(us, them);

        }

    }

    return stat;

}

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

int _worker_ctor(object_t *object, item_list_t *items) {

    int type = 0;
    int stat = ERR;
    worker_t *self = NULL;

    if (object != NULL) {

        /* capture our items */

        if (items != NULL) {

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                switch(items[x].item_code) {
                    case WORKER_K_TYPE: {
                        memcpy(&type, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                }

            }

        }

        /* initilize our base klass here */


        /* initialize our derived klass here */

        self = WORKER_OBJECT(object);

        /* assign our methods here */

        self->ctor = _worker_ctor;
        self->dtor = _worker_dtor;
        self->_match = _worker_match;
        self->_compare = _worker_compare;
        self->_perform = _worker_perform;
        self->_override = _worker_override;

        /* initialize internal variables here */
        
        self->type = type;

        stat = OK;

    }

    return stat;

}

int _worker_dtor(object_t *object) {

    int stat = 0;

    /* free local resources here */


    /* walk the chain, freeing as we go */

    object_demote(object, object_t);
    object_destroy(object);

    return stat;

}

int _worker_override(worker_t *self, item_list_t *items) {

    int stat = -1;

    if (items != NULL) {

        int x;
        for (x = 0;; x++) {

            if ((items[x].buffer_length == 0) &&
                (items[x].item_code == 0)) break;

            switch(items[x].item_code) {
                case WORKER_M_DESTRUCTOR: {
                    self->dtor = items[x].buffer_address;
                    stat = 0;
                    break;
                }
                case WORKER_M_MATCH: {
                    self->_match = items[x].buffer_address;
                    stat = 0;
                    break;
                }
                case WORKER_M_PERFORM: {
                    self->_perform = items[x].buffer_address;
                    stat = 0;
                    break;
                }
            }

        }

    }

    return stat;

}

int _worker_compare(worker_t *self, worker_t *other) {

    int stat = -1;

    if ((object_compare(OBJECT(self), OBJECT(other)) == 0) &&
        (self->ctor == other->ctor) &&
        (self->dtor == other->dtor) &&
        (self->_compare == other->_compare) &&
        (self->_override == other->_override) &&
        (self->_match == other->_match) &&
        (self->_perform == other->_perform) &&
        (self->type == other->type)) {

        stat = 0;

    }

    return stat;

}

int _worker_match(worker_t *self, int type) {

    return (self->type == type) ? 0 : -1;

}

void *_worker_perform(worker_t *self, void *data, int size) {

    void *result = NULL;

    if (data != NULL) {

        result = data;

    }

    return result;

}


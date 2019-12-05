
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

#include "when.h"
#include "object.h"
#include "template.h"
#include "error_codes.h"

require_klass(OBJECT_KLASS);

/*----------------------------------------------------------------*/
/* klass methods                                                  */
/*----------------------------------------------------------------*/

int _template_ctor(object_t *, item_list_t *);
int _template_dtor(object_t *);

int _template_compare(template_t *, template_t *);
int _template_override(template_t *, item_list_t *);

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

declare_klass(TEMPLATE_KLASS) {
    .size = KLASS_SIZE(template_t),
    .name = KLASS_NAME(template_t),
    .ctor = _template_ctor,
    .dtor = _template_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

template_t *template_create(item_list_t *items) {

    int stat = ERR;
    template_t *self = NULL;

    self = (template_t *)object_create(TEMPLATE_KLASS, items, &stat);

    return self;

}

int template_destroy(template_t *self) {

    int stat = OK;

    when_error {

        if (self != NULL) {

            if (object_assert(self, template_t)) {

                stat = self->dtor(OBJECT(self));
                check_status(stat, OK, E_INVOPS);

            } else {

                cause_error(E_INVOBJ);

            }

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } error_handler {

        stat = ERR;

        object_set_error2(OBJECT(self), trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int template_override(template_t *self, item_list_t *items) {

    int stat = OK;

    when_error {
        
        if (self != NULL) {

            stat = self->_override(self, items);
            check_status(stat, OK, E_INVOPS);

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } error_handler {

        stat = ERR;

        object_set_error2(OBJECT(self), trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int template_compare(template_t *us, template_t *them) {

    int stat = OK;

    when_error {

        if (us != NULL) {

            if (object_assert(them, template_t)) {

                stat = us->_compare(us, them);
                check_status(stat, OK, E_NOTSAME);

            } else {

                cause_error(E_INVOBJ);

            }

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } error_handler {

        stat = ERR;

        object_set_error2(OBJECT(us), trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

int _template_ctor(object_t *object, item_list_t *items) {

    int stat = ERR;
    template_t *self = NULL;

    if (object != NULL) {

        /* capture our items */

        if (items != NULL) {

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                /* switch(items[x].item_code) { */
                /*     case TEMPLATE_K_TYPE: { */
                /*         memcpy(&type,  */
                /*                items[x].buffer_address,  */
                /*                items[x].buffer_length); */
                /*         break; */
                /*     } */
                /* } */

            }

        }

        /* initilize our base klass here */


        /* initialize our derived klass here */

        self = TEMPLATE(object);

        /* assign our methods here */

        self->ctor = _template_ctor;
        self->dtor = _template_dtor;
        self->_compare = _template_compare;
        self->_override = _template_override;

        /* initialize internal variables here */
        
        stat = OK;

    }

    return stat;

}

int _template_dtor(object_t *object) {

    int stat = 0;

    /* free local resources here */


    /* walk the chain, freeing as we go */

    object_demote(object, object_t);
    object_destroy(object);

    return stat;

}

int _template_override(template_t *self, item_list_t *items) {

    int stat = -1;

    if (items != NULL) {

        int x;
        for (x = 0;; x++) {

            if ((items[x].buffer_length == 0) &&
                (items[x].item_code == 0)) break;

            switch(items[x].item_code) {
                case TEMPLATE_M_DESTRUCTOR: {
                    self->dtor = items[x].buffer_address;
                    stat = 0;
                    break;
                }
            }

        }

    }

    return stat;

}

int _template_compare(template_t *self, template_t *other) {

    int stat = ERR;

    if ((object_compare(OBJECT(self), OBJECT(other)) == 0) &&
        (self->ctor == other->ctor) &&
        (self->dtor == other->dtor) &&
        (self->_compare == other->_compare) &&
        (self->_override == other->_override)) {

        stat = OK;

    }

    return stat;

}


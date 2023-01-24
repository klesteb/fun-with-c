
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

#include "include/when.h"
#include "objects/object.h"
#include "template1/template.h"
#include "include/item_list.h"
#include "include/error_codes.h"
#include "template1/template_priv.h"

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

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
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

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
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

    } use {

        stat = ERR;

        object_set_error2(us, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

char *template_version(template_t *self) {
    
    char *version = VERSION;

    return version;
    
}



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

#include <stdio.h>
#include <errno.h>

#include "when.h"
#include "trace.h"
#include "errors.h"
#include "object.h"
#include "error_codes.h"

require_klass(OBJECT_KLASS);

/*----------------------------------------------------------------*/
/* klass methods                                                  */
/*----------------------------------------------------------------*/

int _trace_ctor(object_t *, item_list_t *);
int _trace_dtor(object_t *);

int _trace_compare(trace_t *, trace_t *);
int _trace_override(trace_t *, item_list_t *);
int _trace_add(trace_t *, error_trace_t *);
int _trace_dump(trace_t *, int (*output)(char *));

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

declare_klass(TRACE_KLASS) {
    .size = KLASS_SIZE(trace_t),
    .name = KLASS_NAME(trace_t),
    .ctor = _trace_ctor,
    .dtor = _trace_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

trace_t *trace_create(item_list_t *items) {

    int stat = ERR;
    trace_t *self = NULL;

    self = (trace_t *)object_create(TRACE_KLASS, items, &stat);

    return self;

}

int trace_destroy(trace_t *self) {

    int stat = OK;

    when_error_in {

        if (self != NULL) {

            if (object_assert(self, trace_t)) {

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

int trace_override(trace_t *self, item_list_t *items) {

    int stat = OK;

    when_error_in {
        
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

int trace_compare(trace_t *us, trace_t *them) {

    int stat = OK;

    when_error_in {

        if (us != NULL) {

            if (object_assert(them, trace_t)) {

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

int trace_add(trace_t *self, error_trace_t *error) {

    int stat = OK;

    when_error_in {

        if ((self != NULL) && (error != NULL)) {

            stat = self->_add(self, error);
            check_return(stat, self);

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

int trace_dump(trace_t *self, int (*output)(char *)) {

    int stat = OK;

    when_error_in {

        if (self != NULL) {

            stat = self->_dump(self, output);
            check_return(stat, self);

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

int _trace_ctor(object_t *object, item_list_t *items) {

    int stat = ERR;
    trace_t *self = NULL;

    if (object != NULL) {

        /* capture our items */

        if (items != NULL) {

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                /* switch(items[x].item_code) { */
                /*     case TRACE_K_TYPE: { */
                /*         memcpy(&type,  */
                /*                items[x].buffer_address,  */
                /*                items[x].buffer_length); */
                /*         break; */
                /*     } */
                /* } */

            }

        }

        /* initilize our base klass here */

        object_set_error1(object, OK);

        /* initialize our derived klass here */

        self = TRACE(object);

        /* assign our methods here */

        self->ctor = _trace_ctor;
        self->dtor = _trace_dtor;
        self->_compare = _trace_compare;
        self->_override = _trace_override;
        self->_add = _trace_add;
        self->_dump = _trace_dump;

        /* initialize internal variables here */

        when_error_in {

            self->errs = errors_create();
            check_creation(self->errs);

            stat = que_init(&self->errors);
            check_status(stat, QUE_OK, E_NOQUEUE);

            stat = OK;
            exit_when;

        } use {

            object_set_error1(self, trace_errnum);

        } end_when;

    }

    return stat;

}

int _trace_dtor(object_t *object) {

    int stat = OK;
    error_trace_t *error = NULL;
    trace_t *trace = TRACE(object);

    /* free local resources here */

    while ((error = que_pop_head(&trace->errors))) {

        free(error->filename);
        free(error->function);
        free(error);

    }

    errors_destroy(trace->errs);

    /* walk the chain, freeing as we go */

    object_demote(object, object_t);
    object_destroy(object);

    return stat;

}

int _trace_override(trace_t *self, item_list_t *items) {

    int stat = ERR;

    if (items != NULL) {

        int x;
        for (x = 0;; x++) {

            if ((items[x].buffer_length == 0) &&
                (items[x].item_code == 0)) break;

            switch(items[x].item_code) {
                case TRACE_M_DESTRUCTOR: {
                    self->dtor = items[x].buffer_address;
                    stat = 0;
                    break;
                }
            }

        }

    }

    return stat;

}

int _trace_compare(trace_t *self, trace_t *other) {

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

int _trace_add(trace_t *self, error_trace_t *error) {

    int stat = OK;

    errno = 0;
    if ((stat = que_push_head(&self->errors, error)) != QUE_OK) {

        stat = ERR;
        object_set_error1(self, errno);

    }

    return stat;

}

int _trace_dump(trace_t *self, int (*output)(char *)) {

    int stat = OK;
    char text[1024];
    char nemonic[32];
    char message[1024];
    error_trace_t *error = NULL;
    char *format = "%s, message: %s; at line: %d, file: %s, function: %s";

    for (error = que_first(&self->errors);
         error != NULL;
         error = que_next(&self->errors)) {

        errors_get_text(self->errs, error->errnum, nemonic, 32);
        errors_get_message(self->errs, error->errnum, message, 1024);

        snprintf(text, 1024, format, nemonic, message, 
                 error->lineno, error->filename, error->function);

        output(text);

    }

    return stat;

}


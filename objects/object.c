
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

/*---------------------------------------------------------------------------*/
/* the concept for these routines was extracted from vk_object.[c|h]         */
/* located at: https://github.com/TragicWarrior/libviper                     */
/*---------------------------------------------------------------------------*/

#include <stdlib.h>
#include <string.h>

#include "object.h"

/*----------------------------------------------------------------*/
/* klass defination                                               */
/*----------------------------------------------------------------*/

declare_klass(OBJECT_KLASS) {
    .name = KLASS_NAME(object_t),
    .size = KLASS_SIZE(object_t),
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

object_t *object_construct(const void *klass, item_list_t *items, int *stat) {

    *stat = ERR;
    object_t *object = NULL;
    
    if (klass != NULL) {

	    if ((object = calloc(1, OBJECT(klass)->size))) {

			/* copy template to newly allocated object */

			memcpy(object, klass, sizeof(object_t));

			if (object->ctor != NULL) {

                object->error = NULL;

				/* pass the pointer to the argument list structure */

				*stat = object->ctor(object, items);

			}

		}

	}

    return object;

}

int object_destroy(object_t *object) {

    if (!object_assert(object, object_t)) {

        if (object->error != NULL) {

            free(object->error->filename);
            free(object->error->function);
            free(object->error);

        }

        object->dtor(object);

    }

    free(object);

    return OK;

}

int object_compare(object_t *us, object_t *them) {

    int stat = ERR;

    if ((strcmp(us->name, them->name) == 0) &&
        (us->size == them->size) &&
        (us->ctor == them->ctor) &&
        (us->dtor == them->dtor) &&
        (us->error == them->error)) {

        stat = OK;

    }

    return stat;

}

int object_get_error(object_t *self, error_trace_t *error) {

    int stat = ERR;

    if ((self != NULL) && (error != NULL)) {

        error->errnum   = self->error->errnum;
        error->lineno   = self->error->lineno;
        error->filename = strdup(self->error->filename);
        error->function = strdup(self->error->function);

        stat = OK;

    }

    return stat;

}

int object_set_error(object_t *self, int errnum, int lineno, char *file, const char *function) {

    if (self->error != NULL) {

        free(self->error->filename);
        free(self->error->function);

        self->error->errnum = errnum;
        self->error->lineno = lineno;
        self->error->filename = strdup(file);
        self->error->function = strdup(function);

    } else {

        error_trace_t *error = NULL;

        if ((error = calloc(1, sizeof(error_trace_t)))) {

            error->errnum = errnum;
            error->lineno = lineno;
            error->filename = strdup(file);
            error->function = strdup(function);

            self->error = error;

        }

    }

    return OK;

}


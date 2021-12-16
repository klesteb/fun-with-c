
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

#include <stdlib.h>
#include <string.h>

#include "cclibs/misc/misc.h"
#include "worker.h"

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

void *_worker1_perform(worker_t *self, void *data, int size) {

    char *result = NULL;
    
    if (data != NULL) {

        char *junk = xmalloc(size);
        junk = memcpy(junk, data, size);

        result = strrev(junk);

    }

    return (void *)result;

}

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

worker_t *worker1_create(int type) {

    item_list_t items[2];
    item_list_t methods[2];
    worker_t *self = NULL;

    SET_ITEM(items[0], WORKER_K_TYPE, &type, sizeof(int), NULL);
    SET_ITEM(items[1], 0, 0, 0, 0);

    self = worker_create(items);

    SET_ITEM(methods[0], WORKER_M_PERFORM, &_worker1_perform, 0, 0);
    SET_ITEM(methods[1], 0, 0, 0, 0);

    worker_override(self, methods);

    return self;

}


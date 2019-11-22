
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
#include "worker.h"

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/
    
/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

worker_t *worker2_create(int type) {

    item_list_t items[2];
    worker_t *self = NULL;

    SET_ITEM(items[0], WORKER_K_TYPE, &type, sizeof(int), NULL);
    SET_ITEM(items[1], 0, 0, 0, 0);

    self = worker_create(items);

    return self;

}

/*----------------------------------------------------------------*/


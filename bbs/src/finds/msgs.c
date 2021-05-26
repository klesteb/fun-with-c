
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2021 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that this copyright notice appears in all copies. The author    */
/*  makes no representations about the suitability of this software for      */
/*  any purpose. It is provided "as is" without express or implied warranty. */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#include <stdio.h>
#include "finds.h"

int find_index_all(ulong selector, jam_index_t *index) {

    return TRUE;

}

int find_index_by_user(ulong selector, jam_index_t *index) {

    int stat = FALSE;

    if (selector == index->user_crc) {

        stat = TRUE;

    }

    return stat;

}


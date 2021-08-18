
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

#include "bbs/src/finds/finds.h"

int find_nodes_active(void *data1, void *data2) {
    
    int stat = FALSE;
    node_base_t *node = (node_base_t *)data2;

    if (node->action != NODE_OFFL) {

        stat = TRUE;

    }

    return stat;

}

int find_node_by_number(void *data1, void *data2) {

    int nodenum;
    int stat = FALSE;
    node_base_t *node = (node_base_t *)data2;

    memcpy(&nodenum, data1, sizeof(int));

    if (node->nodenum == nodenum) {

        stat = TRUE;

    }

    return stat;

}

int find_nodes_all(void *data1, void *data2) {

    return TRUE;

}


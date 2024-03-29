
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

#include "bbs/src/main/bbs_common.h"
#include "bbs/src/main/bbs_protos.h"

/*---------------------------------------------------------------------------*/

int bbs_send_status(int action, error_trace_t *errors) {

    int stat = OK;

    when_error_in {

        qnode.action = action;
        qnode.useron = (action == NODE_OFFL) ? 0 : useron.eternal;

        stat = node_put(nodes, qnode_index, &qnode);
        check_return(stat, nodes);

        exit_when;

    } use {

        stat = ERR;
        copy_error(errors);
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

int bbs_get_status(error_trace_t *errors) {

    int stat = OK;

    when_error_in {

        stat = node_get(nodes, qnode_index, &qnode);
        check_return(stat, nodes);

        exit_when;

    } use {

        stat = ERR;
        copy_error(errors);
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}


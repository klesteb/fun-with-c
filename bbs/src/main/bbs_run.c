
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

static int clear_message(void *data) {

    int stat = OK;
    error_trace_t errors;

    when_error_in {

        stat = bbs_clear_message(&errors);
        check_status2(stat, OK, errors);

        exit_when;

    } use {

        stat = ERR;
        capture_trace(dump);
        clear_error();

        event_break(events);

    } end_when;

    return stat;

}

static int dispatch(void *data) {

    int stat = OK;
    int again = FALSE;

    when_error_in {

        stat = workbench_dispatch(workbench, &again);
        check_return(stat, workbench);

        exit_when;

    } use {

        stat = ERR;
        capture_trace(dump);
        clear_error();

        event_break(events);

    } end_when;

    return stat;

}

static int read_stdin(void *data) {

    int stat = OK;

    when_error_in {

        stat = workbench_capture(workbench);
        check_return(stat, workbench);

        exit_when;

    } use {

        stat = ERR;
        capture_trace(dump);
        clear_error();

        event_break(events);

    } end_when;

    return stat;

}

static int monitor_nodes(void *data) {

    int stat = OK;
    error_trace_t error;

    when_error_in {

        stat = bbs_get_status(&error);
        check_status2(stat, OK, error);

        exit_when;

    } use {

        stat = ERR;
        capture_trace(dump);
        clear_error();

        event_break(events);

    } end_when;

    return stat;

}

int bbs_run(error_trace_t *errors) {

    int stat = OK;

    when_error_in {

        stat = event_register_input(events, fileno(stdin), read_stdin, NULL);
        check_return(stat, events);

        stat = event_register_timer(events, TRUE, 10.0, monitor_nodes, NULL);
        check_return(stat, events);

        stat = event_register_timer(events, TRUE, 0.1, dispatch, NULL);
        check_return(stat, events);

        /* stat = event_register_timer(events, TRUE, 10.0, clear_message, NULL); */
        /* check_return(stat, events); */

        stat = event_loop(events);
        check_return(stat, events);

        exit_when;

    } use {

        stat = ERR;
        copy_error(errors);
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}


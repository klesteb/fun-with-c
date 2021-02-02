
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

#include "common.h"
#include "interfaces.h"

/*---------------------------------------------------------------------------*/

int read_stdin(void *data) {

    workbench_capture(workbench);
    return OK;

}

int bbs_run(error_trace_t *errors) {

    int stat = OK;
    error_trace_t error;

    when_error_in {

        stat = event_register_input(events, fileno(stdin), read_stdin, NULL);
        check_return(stat, events);

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


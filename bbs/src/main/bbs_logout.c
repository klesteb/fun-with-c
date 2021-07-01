
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
#include <errno.h>

#include "bbs_common.h"
#include "bbs_protos.h"
#include "windows/query_window.h"

/* local items ---------------------------------------------------------- */

window_t *win = NULL;

/* ---------------------------------------------------------------------- */

static int logout(int yesno, error_trace_t *errors) {

    int stat = OK;
    error_trace_t error;

    when_error_in {

fprintf(stderr, "enterng logout(): %d\n", yesno);

        if (yesno) {

fprintf(stderr, "event_break()\n");
            stat = bbs_logoff(&error);
            check_status2(stat, OK, error);

            stat = event_break(events);
            check_return(stat, events);

        } else {

fprintf(stderr, "workbench_remove()\n");
            stat = workbench_remove_async(workbench, win);
            check_return(stat, workbench);

        }

        exit_when;

    } use {

        stat = ERR;
        copy_error(errors);
        capture_trace(dump);
        clear_error();

    } end_when;

fprintf(stderr, "leaving logout() - stat: %d\n", stat);
    return stat;

}

int bbs_logoff(error_trace_t *errors) {

    int stat = OK;
    error_trace_t error;

    when_error_in {

        stat = user_put(users, user_index, &useron);
        check_return(stat, users);

        stat = bbs_send_status(NODE_OFFL, &error);
        check_status2(stat, OK, error);

        exit_when;

    } use {

        stat = ERR;
        copy_error(errors);
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

int bbs_logout(void *data, int size, error_trace_t *errors) {

    int stat = OK;
    char *title = "Logout";
    char *value = "Are you sure you want to logoff?";

    when_error_in {

        win = query_window(title, logout, value);
        check_creation(win);

        stat = workbench_add(workbench, win);
        check_return(stat, workbench);

        exit_when;

    } use {

        stat = ERR;
        copy_error(errors);
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}


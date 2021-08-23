
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

/* ------------------------------------------------------------------------ */

int bbs_doors(door_t *doors, room_base_t *room, error_trace_t *errors) {

    int stat = OK;
    user_base_t sysop;
    error_trace_t error;
    profile_base_t *profile = NULL;

fprintf(stderr, "entering bbs_doors()\n");

    when_error_in {

        stat = user_get(users, SYSOP, &sysop);
        check_return(stat, users);

        if (has_profile(&useron)) {

            stat = profile_get(profiles, useron.profile, profile);
            check_return(stat, profile);

        }

        stat = bbs_send_status(NODE_XTRN, &error);
        check_status2(stat, OK, error);

        /* exit ncurses */

        def_prog_mode();
        endwin();

        /* run the subsystem */

        stat = door_run(doors, &qnode, room, &sysop, &useron, profile);
        check_return(stat, doors);

        /* reenter ncurses */

        refresh();

        exit_when;

    } use {

        stat = ERR;
        copy_error(errors);
        capture_trace(dump);
        clear_error();

    } end_when;

fprintf(stderr, "leaving bbs_doors() - stat: %d\n", stat);
    return stat;

}


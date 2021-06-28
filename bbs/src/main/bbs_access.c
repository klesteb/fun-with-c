
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

#include "bbs_common.h"
#include "bbs_protos.h"

/*---------------------------------------------------------------------------*/

int known_room(room_search_t *result, user_base_t *user) {

    int stat = FALSE;

    if ((result->roomnum == MAILROOM) || (result->roomnum == LOBBY)) {

        stat = TRUE;

    } else if (bit_test(result->flags, RM_PREFONLY)) {

        stat = TRUE;

    } else if (bit_test(result->flags, RM_PRIVATE)) {

        if (is_removed(result, user)) {

            stat = FALSE;
            goto fini;

        }

        if (is_invited(result, user)) {

            stat = TRUE;

        }

    } else if (result->aide == user->eternal) {

        stat = TRUE;

    } else if (user->axlevel >= AX_AIDE) {

        stat = TRUE;

    } else if (is_forgotten(result, user)) {

        stat = FALSE;

    }

    fini:
    return stat;

}

int allowed_in_room(room_search_t *result, user_base_t *user) {

    int stat = FALSE;

    if (bit_test(result->flags, RM_INUSE)) {

        if ((result->roomnum == MAILROOM) || (result->roomnum == LOBBY)) {
fprintf(stderr, "allowed_in_room() - MAILROOM || LOBBY\n");

            stat = TRUE;

        } else if (bit_test(result->flags, RM_PRIVATE)) {
fprintf(stderr, "allowed_in_room() - RM_PRIVATE\n");

            if (is_removed(result, user)) {

                stat = FALSE;
                goto fini;

            }

            if (is_invited(result, user)) {

                stat = TRUE;

            }

        } else if (bit_test(result->flags, RM_PREFONLY)) {
fprintf(stderr, "allowed_in_room() - RM_PREFONLY\n");

            stat = TRUE;

        } else if (result->aide == user->eternal) {
fprintf(stderr, "allowed_in_room() - user is room aide\n");

            stat = TRUE;

        } else if (user->axlevel >= AX_AIDE) {
fprintf(stderr, "allowed_in_room() - user is an aide\n");

            stat = TRUE;

        } else if (is_forgotten(result, user)) {
fprintf(stderr, "allowed_in_room() - user has forgetten room\n");

            stat = FALSE;

        }

    }

    fini:
    return stat;

}

int is_forgotten(room_search_t *result, user_base_t *user) {

    int stat = OK;
    int test = FALSE;
    off_t recnum = 0;
    room_status_t status;
    room_status_find_t find;
    int recsize = sizeof(room_status_find_t);

fprintf(stderr, "entering is_forgetten()\n");
    when_error_in {

        find.roomnum = result->roomnum;
        find.usernum = user->eternal;

        stat = room_status_find(rstatus, &find, recsize, find_room_status, &recnum);
        check_return(stat, rstatus);

        if (recnum > 0) {

            stat = room_status_get(rstatus, recnum, &status);
            check_return(stat, rstatus);

            if (bit_test(status.flags, RS_FORGET)) {

                test = TRUE;

            }

        }

        exit_when;

    } use {

        test = FALSE;

    } end_when;

fprintf(stderr, "leaving is_forgetten() - test: %d, stat: %d\n", test, stat);
    return test;
    
}

int is_removed(room_search_t *result, user_base_t *user) {

    int stat = OK;
    int test = FALSE;
    off_t recnum = 0;
    room_status_t status;
    room_status_find_t find;
    int recsize = sizeof(room_status_find_t);

fprintf(stderr, "entering is_removed()\n");
    when_error_in {

        find.roomnum = result->roomnum;
        find.usernum = user->eternal;
fprintf(stderr, "is_removed() - roomnum: %ld, usernum: %ld\n", result->roomnum, user->eternal);

        stat = room_status_find(rstatus, &find, recsize, find_room_status, &recnum);
        check_return(stat, rstatus);
fprintf(stderr, "is_removed() - after find, stat: %d\n", stat);
fprintf(stderr, "is_removed() - after find, recnum: %ld\n", recnum);

        if (recnum > 0) {
fprintf(stderr, "is_removed() - status found\n");

            stat = room_status_get(rstatus, recnum, &status);
            check_return(stat, rstatus);

            if (bit_test(status.flags, RS_REMOVED)) {
fprintf(stderr, "is_removed() - RS_REMOVED\n");

                test = TRUE;

            }

        }

        exit_when;

    } use {

        test = FALSE;

    } end_when;

fprintf(stderr, "leaving is_removed() - test: %d, stat: %d\n", test, stat);
    return test;

}

int is_invited(room_search_t *result, user_base_t *user) {

    int stat = OK;
    int test = FALSE;
    off_t recnum = 0;
    room_status_t status;
    room_status_find_t find;
    int recsize = sizeof(room_status_find_t);

fprintf(stderr, "entering is_invited()\n");
    when_error_in {

        find.roomnum = result->roomnum;
        find.usernum = user->eternal;
fprintf(stderr, "is_invited() - roomnum: %ld, usernum: %ld\n", result->roomnum, user->eternal);

        stat = room_status_find(rstatus, &find, recsize, find_room_status, &recnum);
        check_return(stat, rstatus);

        if (recnum > 0) {
fprintf(stderr, "is_invited() - status found\n");

            stat = room_status_get(rstatus, recnum, &status);
            check_return(stat, rstatus);

            if (bit_test(status.flags, RS_INVITED)) {
fprintf(stderr, "is_removed() - RS_INVITED\n");

                test = TRUE;

            }

        }

        exit_when;

    } use {

        test = FALSE;

    } end_when;

fprintf(stderr, "leaving is_invited() - test: %d, stat: %d\n", test, stat);
    return test;

}

int is_aide(room_base_t *room, user_base_t *user) {

    int stat = FALSE;

    if (bit_test(room->flags, RM_INUSE)) {

        if (room->aide == user->eternal) {

            stat = TRUE;

        } else if (user->axlevel >= AX_AIDE) {

            stat = TRUE;

        }

    }

    return stat;

}

int is_sysop(room_base_t *room, user_base_t *user) {

    int stat = FALSE;

    if (bit_test(room->flags, RM_INUSE)) {

        if (user->axlevel >= AX_SYSOP) {

            stat = TRUE;

        }

    }

    return stat;

}

int can_create_room(user_base_t *user) {

    int stat = FALSE;

    if (user->axlevel >= makeroom) {

        stat = TRUE;

    }

    return stat;

}

int can_post_lobby(user_base_t *user) {

    int stat = FALSE;

    if (user->axlevel >= lobbypost) {

        stat = TRUE;

    }

    return stat;

}

int can_edit_aide(user_base_t *user) {

    int stat = FALSE;

    if (user->axlevel >= AX_SYSOP) {

        stat = TRUE;

    }

    return stat;

}


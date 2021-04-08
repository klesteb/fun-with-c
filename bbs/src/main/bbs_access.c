
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

    } else if (! bit_test(result->flags, RM_PRIVATE)) {

        stat = TRUE;

    } else if (result->aide == user->eternal) {

        stat = TRUE;

    } else if (user->axlevel >= AX_AIDE) {

        stat = TRUE;

    }

    return stat;

}

int allowed_in_room(room_search_t *result, user_base_t *user) {

    int stat = FALSE;

    if (bit_test(result->flags, RM_INUSE)) {

        if ((result->roomnum == MAILROOM) || (result->roomnum == LOBBY)) {

            stat = TRUE;

        } else if (! bit_test(result->flags, RM_PRIVATE)) {

            stat = TRUE;

        } else if (bit_test(result->flags, RM_PREFONLY)) {

            stat = TRUE;

        } else if (result->aide == user->eternal) {

            stat = TRUE;

        } else if (user->axlevel >= AX_AIDE) {

            stat = TRUE;

        }

    }

    return stat;

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


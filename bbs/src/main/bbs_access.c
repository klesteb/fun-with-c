
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
#include "interfaces.h"

/*---------------------------------------------------------------------------*/

int known_room(room_search_t *result) {

    int stat = FALSE;

    if ((result->roomnum == MAILROOM) || (result->roomnum == LOBBY)) {

        stat = TRUE;

    } else if (!(result->flags & RM_PERFONLY)) {

        stat = TRUE;

    } else if (!(result->flags & RM_PRIVATE)) {

        stat = TRUE;

    } else if (result->aide == useron.eternal) {

        stat = TRUE;

    } else if (useron.axlevel >= AX_AIDE) {

        stat = TRUE;

    }

    return stat;

}

int allowed_in_room(room_base_t *room, user_base_t *user) {

    int stat = FALSE;

    if (room->flags & RM_INUSE) {

        if ((room->roomnum == MAILROOM) || (room->roomnum == LOBBY)) {

            stat = TRUE;

        } else if (!(room->flags & RM_PRIVATE)) {

            stat = TRUE;

        } else if (!(room->flags & RM_PERFONLY)) {

            stat = TRUE;

        } else if (room->aide == user->eternal) {

            stat = TRUE;

        } else if (user->axlevel >= AX_AIDE) {

            stat = TRUE;

        }

    }

    return stat;

}


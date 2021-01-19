
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

#include "finds.h"

int find_rooms_all(void *data, int len, room_base_t *room) {

    return TRUE;

}

int find_room_by_conference(void *data, int len, room_base_t *room) {

    int stat = FALSE;
    short conference;

    memcpy(&conference, data, len);

    if (room->conference == conference) {

        stat = TRUE;

    }

    return stat;

}

int find_rooms_messages(void *data, int len, room_base_t *room) {

    int stat = FALSE;

    if (room->flags & RM_MESSAGES) {

        stat = TRUE;

    }

    return stat;

}

int find_rooms_networked(void *data, int len, room_base_t *room) {

    int stat = FALSE;

    if (room->flags & RM_NETWORK) {

        stat = TRUE;

    }

    return stat;

}


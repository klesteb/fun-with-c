
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

#include "finds.h"
#include "bitops.h"

int find_rooms_all(void *data, int len, room_base_t *room) {

    int stat = FALSE;

    if (bit_test(room->flags, RM_INUSE)) {

        stat= TRUE;

    }

    return stat;

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

int find_room_by_number(void *data, int len, room_base_t *room) {

    int roomnum;
    int stat = FALSE;

    memcpy(&roomnum, data, len);

    if (room->roomnum == roomnum) {

        stat = TRUE;

    }

    return stat;

}

int find_rooms_messages(void *data, int len, room_base_t *room) {

    int stat = FALSE;

    if (bit_test(room->flags, RM_MESSAGES)) {

        stat = TRUE;

    }

    return stat;

}

int find_rooms_networked(void *data, int len, room_base_t *room) {

    int stat = FALSE;

    if (bit_test(room->flags, RM_NETWORK)) {

        stat = TRUE;

    }

    return stat;

}

int find_room_status(void *data1, void *data2) {

    int stat = FALSE;
    room_status_t *status = (room_status_t *)data2;
    room_status_find_t *find = (room_status_find_t *)data1;

fprintf(stderr, "find_room_status() - find; roomnum: %d, usernum: %d\n", find->roomnum, find->usernum);
fprintf(stderr, "find_room_status() - status; roomnum: %d, usernum: %d\n", status->roomnum, status->usernum);
    
    if ((find->roomnum == status->roomnum) &&
        (find->usernum == status->usernum)) {

        stat = TRUE;

    }

    return stat;

}

int find_room_status_for_room(void *data1, void *data2) {

    int stat = FALSE;
    room_status_t *status = (room_status_t *)data2;
    room_status_find_t *find = (room_status_find_t *)data1;

    if (find->roomnum == status->roomnum) {

        stat = TRUE;

    }

    return stat;

}


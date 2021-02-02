
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

#include "bbs_common.h"
#include "interfaces.h"

/*---------------------------------------------------------------------------*/

int bbs_list_known_rooms(queue *results, error_trace_t *errors) {

    queue temp;
    int stat = OK;
    error_trace_t error;
    room_search_t *result = NULL;

    when_error_in {

        stat = que_init(&temp);
        check_status(stat, OK, E_INVOPS);

        stat = room_search(rooms, NULL, 0, find_rooms_all, &temp);
        check_return(stat, rooms);

        while ((result = que_pop_tail(&temp))) {

            if (known_room(result)) {

                stat = que_push_head(results);
                check_status(stat, OK, E_NOQUEUE);

            }

            free(result);

        }

        exit_when;

    } use {

        stat = ERR;
        copy_error(errors);
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

int bbs_list_message_rooms(queue *results, error_trace_t *errors) {

    queue temp;
    int stat = OK;
    error_trace_t error;
    room_search_t *result = NULL;

    when_error_in {

        stat = que_init(&temp);
        check_status(stat, OK, E_INVOPS);

        stat = room_search(rooms, NULL, 0, find_rooms_messages, &temp);
        check_return(stat, rooms);

        while ((result = que_pop_tail(&temp))) {

            if (known_room(result)) {

                stat = que_push_head(results);
                check_status(stat, OK, E_NOQUEUE);

            }

            free(result);

        }

        exit_when;

    } use {

        stat = ERR;
        copy_error(errors);
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

int bbs_load_room(int index, room_base_t *room, error_trace_t *errors) {

    int stat = OK;

    when_error_in {

        stat = room_get(rooms, index, room);
        check_status(stat, rooms);

        exit_when;

    } use {

        stat = ERR;
        copy_error(errors);
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}


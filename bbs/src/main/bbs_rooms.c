
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
#include "bbs_protos.h"

#include "jam.h"
#include "event.h"
#include "windows/box_menu.h"
#include "components/menus/menus.h"

/*---------------------------------------------------------------------------*/

int bbs_load_rooms(queue *results, int (*filter)(void *, int , room_base_t *), error_trace_t *errors) {

    queue temp;
    int stat = OK;
    room_search_t *result = NULL;

    when_error_in {

        stat = que_init(&temp);
        check_status(stat, OK, E_INVOPS);

        stat = room_search(rooms, NULL, 0, filter, &temp);
        check_return(stat, rooms);

        while ((result = que_pop_tail(&temp))) {

            if (allowed_in_room(result, &useron)) {

                stat = que_push_head(results, result);
                check_status(stat, OK, E_NOQUEUE);

            } else {

                free(result);

            }

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


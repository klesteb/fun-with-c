
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

#include "event.h"
#include "windows/menus/box.h"

/*---------------------------------------------------------------------------*/

int display_description(const char *message) {

    int stat;
    events_t *event = NULL;

    if ((event = calloc(1, sizeof(events_t))) != NULL) {

        event->type = EVENT_K_MESSAGE;

        if (message != NULL) {

            event->data = (void *)strndup(message, strlen(message));

        } else {

            event->data = (void *)strdup("no data provided");

        }

        stat = workbench_inject_event(workbench, event);

    }

    return stat;
    
}

int list_rooms(queue *results, int (*filter)(void *, int , room_base_t *), error_trace_t *errors) {

    queue temp;
    int stat = OK;
    room_search_t *result = NULL;

    when_error_in {

        stat = que_init(&temp);
        check_status(stat, OK, E_INVOPS);

        stat = room_search(rooms, NULL, 0, filter, &temp);
        check_return(stat, rooms);

        while ((result = que_pop_tail(&temp))) {

            if (known_room(result, &useron)) {

                stat = que_push_head(results, result);
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

int bbs_load_room(void *data, int len, error_trace_t *errors) {

    int stat = OK;

    when_error_in {

        memcpy(&qroom_index, data, len);

        stat = room_get(rooms, qroom_index, &qroom);
        check_return(stat, rooms);

        if (qroom.flags & RM_MESSAGES) {

        } else if (qroom.flags & RM_BULLETIN) {

        } else if (qroom.flags & RM_DIRECTORY) {

        } else if (qroom.flags & RM_SUBSYS) {

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

int bbs_list_rooms(void *data, int len, error_trace_t *errors) {

    int type = 0;
    int stat = OK;
    int count = 0;
    queue results;
    int list_size = 0;
    char *title = NULL;
    error_trace_t error;
    menus_t *bmenu = NULL;
    menus_list_t *list = NULL;
    room_search_t *result = NULL;
    int startx = getbegy(stdscr) / 2;
    int starty = getbegx(stdscr) / 2;
    int width  = getmaxx(stdscr) / 2;
    int height = getmaxy(stdscr) / 2;

    when_error_in {

        stat = que_init(&results);
        check_status(stat, QUE_OK, E_INVOPS);

        if (len > 0) memcpy(&type, data, len);

        switch (type) {
            case RM_BULLETIN:
            case RM_DIRECTORY:
            case RM_SUBSYS:
            case RM_MESSAGES:
                title = "Message Rooms";
                stat = list_rooms(&results, find_rooms_messages, &error);
                check_status2(stat, OK, error);
                break;
            default:
                title = "Available Rooms";
                stat = list_rooms(&results, find_rooms_all, &error);
                check_status2(stat, OK, error);
                break;
        }

        if ((count = que_size(&results)) > 1) {

            errno = 0;
            list = calloc(count, sizeof(menus_list_t));
            if (list == NULL) cause_error(errno);
            list_size = count * sizeof(menus_list_t);

            int x;
            for (x = 0; x <= count; x++) {

                if ((result = que_pop_tail(&results)) != NULL) {

                    SET_MENU(list[x], result->name, result->description, 
                             &result->index, sizeof(int), bbs_load_room);

                    free(result);

                }

            }

            bmenu = box_menu_create(title, startx, starty, height, width, display_description, list, list_size);
            check_creation(bmenu);

            stat = workbench_add(workbench, WINDOW(bmenu));
            check_return(stat, workbench);

            stat = workbench_refresh(workbench);
            check_return(stat, workbench);

            free(list);

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


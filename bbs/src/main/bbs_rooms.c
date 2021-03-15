
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

static int list_rooms(queue *results, int (*filter)(void *, int , room_base_t *), error_trace_t *errors) {

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
    queue results;
    int count = 0;
    int startx = 0;
    int starty = 0;
    int height = 12;
    int width  = 60;
    int list_size = 0;
    char *title = NULL;
    error_trace_t error;
    menus_t *bmenu = NULL;
    menus_list_t *list = NULL;
    room_search_t *result = NULL;

    when_error_in {

        startx = ((getmaxx(stdscr) - width) / 2);
        starty = ((getmaxy(stdscr) - height) / 2);

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

            int x = 0;
            while ((result = que_pop_tail(&results))) {

                errno = 0;
                int *index = calloc(1, sizeof(int));
                if (index == NULL) cause_error(errno);
                memcpy(index, &result->index, sizeof(int));

                SET_MENU(list[x], result->name, result->description, 
                         (void *)index, sizeof(int), bbs_load_room);

                x++;
                free(result);

            }

            bmenu = box_menu_create(title, startx, starty, height, width, bbs_send_message, list, list_size);
            check_creation(bmenu);

            stat = workbench_add(workbench, (window_t *)bmenu);
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


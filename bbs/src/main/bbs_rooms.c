
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

#include "containers/menus/bar.h"
#include "containers/menus/box.h"
#include "components/menu/menu_items.h"

/*---------------------------------------------------------------------------*/

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

            if (known_room(result)) {

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

int load_room(void *data, int len) {

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
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

int create_menu_item(room_search_t *result, component_t **item, error_trace_t *errors) {

    int stat = OK;
    item_data_t *data = NULL;

    when_error_in {

        errno = 0;
        data = calloc(1, sizeof(item_data_t));
        if (data == NULL) {

            cause_error(errno);

        }

        errno = 0;
        *item = calloc(1, sizeof(component_t));
        if (*item == NULL) {

            cause_error(errno);

        }

        data->label = strdup(result->name);
        data->description = strdup(result->description);
        data->callback = load_room;
        data->data = (void *)&result->index;
        data->data_size = sizeof(int);

        *item = menu_item_create(data);
        check_creation(*item);

        exit_when;

    } use {

        stat = ERR;
        copy_error(errors);
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

int bbs_list_rooms(int type, error_trace_t *errors) {

    int stat = OK;
    queue results;
    char *title = NULL;
    error_trace_t error;
    container_t *menu = NULL;
    component_t *item = NULL;
    room_search_t *result = NULL;
    int row = getbegy(stdscr) / 2;
    int col = getbegx(stdscr) / 2;
    int width = getmaxx(stdscr) / 2;
    int height = getmaxy(stdscr) / 2;

    when_error_in {

        stat = que_init(&results);
        check_status(stat, QUE_OK, E_INVOPS);

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

        available_rooms = window_create(row, col, height, width);
        check_creation(available_rooms);

        stat = window_box(available_rooms, title);
        check_return(stat, available_rooms);

        menu = box_menu_create(1, 1, height - 2, width - 2);
        check_creation(menu);

        while ((result = que_pop_tail(&results))) {

            stat = create_menu_item(result, &item, &error);
            check_status2(stat, OK, error);

            stat = container_add_component(menu, item);
            check_return(stat, menu);

            free(result);

        }

        stat = window_add_container(available_rooms, menu);
        check_return(stat, available_rooms);

        stat = workbench_add_window(workbench, available_rooms);
        check_return(stat, workbench);

        stat = workbench_set_focus(workbench, available_rooms);
        check_return(stat, workbench);

        exit_when;

    } use {

        stat = ERR;
        copy_error(errors);
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}


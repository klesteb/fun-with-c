
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

#include "common.h"
#include "interfaces.h"

window_t *available_rooms = NULL;

/*---------------------------------------------------------------------------*/

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
        data->callback = bbs_load_room;
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

int bbs_known_rooms(queue *results, error_trace_t *error) {

    int stat = OK;
    queue results;
    container_t *menu = NULL;
    component_t *item = NULL;
    int row = getbegy(stdscr) / 2;
    int col = getbegx(stdscr) / 2;
    int width = getmaxx(stdscr) / 2;
    int height = getmaxy(stdscr) / 2;

    when_error_in {

        available_rooms = window_create(row, col, height, width);
        check_creation(available_rooms);

        stat = window_box(available_rooms, "Available Rooms");
        check_return(stat, available_rooms);

        menu = box_menu_create(1, 1, height - 2, width - 2);
        check_creation(menu);

        stat = que_init(&results);
        check_status(stat, QUE_OK, E_INVOPS);

        stat = room_search(rooms, NULL, 0, find_rooms_all, &results);
        check_return(stat, rooms);

        while ((result = que_pop_tail(&results))) {

            if (known_room(result)) {

                stat = create_menu_item(result, &item, &error);
                check_status2(stat, OK, error);

                stat = container_add_component(menu, item);
                check_return(stat, menu);

            }

            free(result);

        }

        stat = window_add_container(available_rooms, menu);
        check_return(stat, available_rooms);

        exit_when;

    } use {

        stat = ERR;
        copy_error(errors);
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}



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

#include "window.h"
#include "container.h"
#include "component.h"
#include "containers/menus/bar.h"
#include "components/menu/menu_items.h"

window_t *screen = NULL;
container_t *menu = NULL;




int bbs_create_screen(error_trace_t *errors) {

    int start = 0;
    int stat = OK;
    queue results;
    error_trace_t error;
    item_data_t *item = NULL;
    int row = getbegy(stdscr);
    int col = getbegx(stdscr);
    room_search_t *result = NULL;
    int width = getmaxx(stdscr) - 2;
    int height = getmaxy(stdscr) - 2;

    when_error_in {

        stat = que_init(&results);
        check_status(stat, QUE_OK, E_INVOPS);

        stat = room_search(rooms, NULL, 0, find_rooms_all, &results);
        check_return(stat, rooms);

        menu = bar_menu_create(width - 2);
        check_creation(menu);

        while ((result = que_pop_head(&results))) {

            if (known_room(result)) {

                stat = create_menu_item(result, &item, &error);
                check_status2(stat, OK, error);

                stat = container_add_component(menu, item);
                check_return(stat, menu);

            }

            free(result);

        }

        start = (row + (height - 5));
        screen = window_create(start, col, 4, width);
        check_creation(screen);

        stat = window_add_container(screen, menu);
        check_return(stat, screen);

        stat = workbench_add_window(workbench, screen);
        check_return(stat, workbench);

        stat = workbench_set_focus(workbench, screen);
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

int bbs_init_terminal(error_trace_t *errors) {

    int stat = OK;
    error_trace_t error;

    when_error_in {

        stat = bbs_create_screen(&error);
        check_status2(stat, OK, error);

        /* stat = bbs_create_prompt("Lobby> ", &error); */
        /* check_status2(stat, OK, errors); */

        /* stat = workbench_add_window(workbench, screen); */
        /* check_return(stat, workbench); */

        /* stat = workbench_add_window(workbench, prompt); */
        /* check_return(stat, workbench); */

        /* stat = workbench_refresh(workbench); */
        /* check_return(stat, workbench); */

        exit_when;

    } use {

        stat = ERR;
        copy_error(errors);
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}


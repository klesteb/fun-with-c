
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
#include <errno.h>

#include "bbs_common.h"
#include "bbs_protos.h"

#include "jam.h"
#include "colors.h"
#include "window.h"
#include "component.h"
#include "misc/misc.h"
#include "windows/alert.h"
#include "components/vline.h"
#include "components/menus/list.h"
#include "components/menus/menus.h"

extern int print_result();

/* local items ---------------------------------------------------------- */


/* ---------------------------------------------------------------------- */

int bbs_msgs_menu(jam_t *jam, room_base_t *room, error_trace_t *errors) {

    int idx = -1;
    theme_t theme;
    int stat = OK;
    int width = 0;
    int count = 6;
    char title[40];
    int height = 0;
    int startx = 0;
    int starty = 0;
    error_trace_t error;
    window_t *win = NULL;
    char *help = "messages.hlp";

    int list_size = 0;
    component_t *vline = NULL;
    component_t *bmenu = NULL;
    menus_list_t *list = NULL;

    /* theme.attribute  = A_NORMAL; */
    /* theme.foreground = BROWN; */
    /* theme.background = BLACK; */

    when_error_in {

        memset(title, '\0', 40);
        snprintf(title, 38, "Room: %s", room->name); 

        width  = (getmaxx(stdscr) - 4);
        height = (getmaxy(stdscr) - 7);
        startx = (getbegx(stdscr) + 1);
        starty = (getbegy(stdscr) + 4);

        errno = 0;
        list = calloc(count, sizeof(menus_list_t));
        if (list == NULL) cause_error(errno);

        idx++; SET_MENU(list[idx], "Forward", "forward retrieval of messages", MENUS_T_ITEM, NULL, 0, print_result);
        idx++; SET_MENU(list[idx], "New", "read new messages", MENUS_T_ITEM, NULL, 0, print_result);
        idx++; SET_MENU(list[idx], "Old", "read old messages", MENUS_T_ITEM, NULL, 0, print_result);
        idx++; SET_MENU(list[idx], "Write", "create a new message", MENUS_T_ITEM, NULL, 0, print_result);
        idx++; SET_MENU(list[idx], "Reverse", "reverse retrieval of messages", MENUS_T_ITEM, NULL, 0, print_result);

        if (is_aide(room, &useron)) {

            errno = 0;
            count += 5;
            list = realloc(list, count * sizeof(menus_list_t));
            if (list == NULL) cause_error(errno);

            idx++; SET_MENU(list[idx], "-Aide--", "aide functions", MENUS_T_SEPERATOR, NULL, 0, NULL);
            idx++; SET_MENU(list[idx], "Delete", "delete empty rooms", MENUS_T_ITEM, NULL, 0, print_result);
            idx++; SET_MENU(list[idx], "Edit", "edit room", MENUS_T_ITEM, NULL, 0, print_result);
            idx++; SET_MENU(list[idx], "Insert", "insert pulled messages", MENUS_T_ITEM, NULL, 0, print_result);
            idx++; SET_MENU(list[idx], "Kill", "remove this room", MENUS_T_ITEM, NULL, 0, print_result);

        }

        if (is_sysop(room, &useron)) {

            errno = 0;
            count += 3;
            list = realloc(list, count * sizeof(menus_list_t));
            if (list == NULL) cause_error(errno);

            idx++; SET_MENU(list[idx], "-Sysop-", "sysop functions", MENUS_T_SEPERATOR, NULL, 0, NULL);
            idx++; SET_MENU(list[idx], "Aide", "set/clear aide privileges", MENUS_T_ITEM, NULL, 0, print_result);
            idx++; SET_MENU(list[idx], "Kill", "disable a user account", MENUS_T_ITEM, NULL, 0, print_result);

        }

        idx++; SET_MENU(list[idx], "Help", "Help with messages", MENUS_T_ITEM, help, strlen(help), print_result);
        list_size = idx * sizeof(menus_list_t);

        stat = bbs_create_window(title, startx, starty, height, width, &win, &error);
        check_status2(stat, OK, error);

        bmenu = list_menu_create(win, 0, 0, height - 3, 9, 1, bbs_send_message, list, list_size);
        check_creation(bmenu);

        vline = vline_create(win, 0, 9, height - 2);
        check_creation(vline);

        stat = window_add(win, bmenu);
        check_return(stat, win);

        stat = window_add(win, vline);
        check_return(stat, win);

        stat = window_set_tab(win, 1);
        check_return(stat, win);

        stat = workbench_add(workbench, win);
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


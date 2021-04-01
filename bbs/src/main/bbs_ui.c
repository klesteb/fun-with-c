
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

#include "colors.h"
#include "window.h"
#include "component.h"
#include "misc/misc.h"
#include "windows/alert.h"
#include "components/text.h"
#include "windows/bar_menu.h"
#include "components/hline.h"
#include "components/menus/menus.h"

int print_result(void *data, int size, error_trace_t *errors) {

    int stat = OK;
    window_t *temp = NULL;

    when_error_in {

        temp = alert("Results", 3, (char *)data);
        check_creation(temp);

        stat = workbench_add(workbench, temp);
        check_return(stat, workbench);

        stat = workbench_set_focus(workbench, temp);
        check_return(stat, workbench);

        stat = workbench_refresh(workbench);
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

int bbs_create_window(char *title, int startx, int starty, int height, int width, window_t **win, error_trace_t *errors) {

    int col = 0;
    int row = 0;
    int stat = OK;
    component_t *text = NULL;
    component_t *hline = NULL;
    char *value = "F10=Main  F11=Cycle  F12=Quit";

    when_error_in {

        *win = window_create(title, startx, starty, height, width, TRUE);
        check_creation(*win);

        row = height - 2;
        hline = hline_create(*win, row, col, width);
        check_creation(hline);

        row = height - 1;
        col = ((width - strlen(value)) / 2);
        width = strlen(value) + 1;
        text = text_create(*win, row, col, width, value, strlen(value));
        check_creation(text);

        stat = window_add(*win, hline);
        check_return(stat, *win);

        stat = window_add(*win, text);
        check_return(stat, *win);

        exit_when;

    } use {

        stat = ERR;
        copy_error(errors);
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

int bbs_clear_message(error_trace_t *errors) {

    int stat = OK;
    events_t *event = NULL;
    int width = getmaxx(stdscr) - 3;

    when_error_in {

        errno = 0;
        event = calloc(1, sizeof(events_t));
        if (event == NULL) cause_error(errno);

        event->type = EVENT_K_MESSAGE;
        event->data = (void *)spaces(width);

        stat = workbench_inject_event(workbench, event);
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

int bbs_send_message(const char *message, error_trace_t *errors) {

    int stat = OK;
    events_t *event = NULL;

    when_error_in {

        errno = 0;
        event = calloc(1, sizeof(events_t));
        if (event == NULL) cause_error(errno);

        event->type = EVENT_K_MESSAGE;

        if (message != NULL) {

            event->data = (void *)strndup(message, strlen(message));

        } else {

            event->data = (void *)strdup("no data provided");

        }

        stat = workbench_inject_event(workbench, event);
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


int bbs_main_menu(error_trace_t *errors) {

    theme_t theme;
    int stat = OK;
    int width = 0;
    int startx = 0;
    int starty = 0;
    int height = 2;
    int list_size = 0;
    window_t *bmenu = NULL;
    menus_list_t *list = NULL;
    char *data1 = "this is data for test1";
    char *data2 = "this is data for test2";
    char *data3 = "this is data for test3";
    char *data4 = "this is data for test4";
    char *data5 = "this is data for test5";
    char *data6 = "this is data for test6";
    char *data7 = "this is data for test7";
    char *data8 = "this is data for test8";
    char *data9 = "this is data for test9";

    theme.attribute  = A_NORMAL;
    theme.foreground = BROWN;
    theme.background = BLACK;

    when_error_in {

        startx = getbegx(stdscr);
        starty = getbegy(stdscr);
        width  = getmaxx(stdscr) - 2;

        errno = 0;
        list = calloc(6, sizeof(menus_list_t));
        if (list == NULL) cause_error(errno);
        list_size = 6 * sizeof(menus_list_t);

        SET_MENU(list[0], "Goto", "goto a specific room", MENUS_T_ITEM, NULL, 0, bbs_list_rooms);
        SET_MENU(list[1], "Who", "who's online", MENUS_T_ITEM, data2, strlen(data2), print_result);
        SET_MENU(list[2], "User", "user maintence", MENUS_T_ITEM, data5, strlen(data5), print_result);
        SET_MENU(list[3], "System", "system statistics", MENUS_T_ITEM, data6, strlen(data6), print_result);
        SET_MENU(list[4], "Help", "show help", MENUS_T_ITEM, data8, strlen(data8), print_result);
        SET_MENU(list[5], "Logout", "logout of system", MENUS_T_ITEM, data9, strlen(data9), print_result);

        bmenu = bar_menu(startx, starty, height, width, list, list_size);
        check_creation(bmenu);

        stat = window_set_theme(bmenu, &theme);
        check_return(stat, bmenu);

        stat = workbench_set_menu(workbench, bmenu);
        check_return(stat, workbench);

        stat = workbench_refresh(workbench);
        check_return(stat, workbench);

        free(list);

        exit_when;

    } use {

        stat = ERR;
        copy_error(errors);
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}


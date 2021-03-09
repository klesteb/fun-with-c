
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
#include "windows/alert.h"
#include "windows/menus/bar.h"
#include "windows/menus/menus.h"
#include "windows/menus/menus_list.h"

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

int bbs_mail_menu(error_trace_t *errors) {

    theme_t theme;
    int stat = OK;
    int width = 0;
    int startx = 0;
    int starty = 0;
    int list_size = 0;
    menus_t *bmenu = NULL;
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

        if (is_aide(&qroom, &useron)) {

            errno = 0;
            list = calloc(9, sizeof(menus_list_t));
            if (list == NULL) cause_error(errno);
            list_size = 9 * sizeof(menus_list_t);

            SET_MENU(list[0], "Goto", "goto a specific room", data1, strlen(data1), print_result);
            SET_MENU(list[1], "Forward", "forward retrieval of message", data2, strlen(data2), print_result);
            SET_MENU(list[2], "New", "read new messages", data5, strlen(data5), print_result);
            SET_MENU(list[3], "Old", "read old messages", data6, strlen(data6), print_result);
            SET_MENU(list[4], "Write", "create a new message", data7, strlen(data7), print_result);
            SET_MENU(list[5], "Reverse", "reverse retrieval of messages", data4, strlen(data4), print_result);
            SET_MENU(list[6], "Aide", "aide options", data9, strlen(data9), print_result);
            SET_MENU(list[7], "Help", "help", data8, strlen(data8), print_result);
            SET_MENU(list[8], "Logout", "logout of system", data9, strlen(data9), print_result);

        } else {

            errno = 0;
            list = calloc(8, sizeof(menus_list_t));
            if (list == NULL) cause_error(errno);
            list_size = 8 * sizeof(menus_list_t);

            SET_MENU(list[0], "Goto", "goto a specific room", data1, strlen(data1), print_result);
            SET_MENU(list[1], "Forward", "forward retrieval of message", data2, strlen(data2), print_result);
            SET_MENU(list[2], "New", "read new messages", data5, strlen(data5), print_result);
            SET_MENU(list[3], "Old", "read old messages", data6, strlen(data6), print_result);
            SET_MENU(list[4], "Write", "create a new message", data7, strlen(data7), print_result);
            SET_MENU(list[5], "Reverse", "reverse retrieval of messages", data4, strlen(data4), print_result);
            SET_MENU(list[6], "Help", "help", data8, strlen(data8), print_result);
            SET_MENU(list[7], "Logout", "logout of system", data9, strlen(data9), print_result);

        }

        bmenu = bar_menu_create(startx, starty, 2, width, list, list_size);
        check_creation(bmenu);

        stat = menus_set_theme(bmenu, &theme);
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

int bbs_main_menu(error_trace_t *errors) {

    theme_t theme;
    int stat = OK;
    int width = 0;
    int startx = 0;
    int starty = 0;
    int list_size = 0;
    menus_t *bmenu = NULL;
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

        if (is_aide(&qroom, &useron)) {

            errno = 0;
            list = calloc(8, sizeof(menus_list_t));
            if (list == NULL) cause_error(errno);
            list_size = 8 * sizeof(menus_list_t);

            SET_MENU(list[0], "Goto", "goto a specific room", NULL, 0, bbs_list_rooms);
            SET_MENU(list[1], "Enter", "enter the current room", data2, strlen(data2), print_result);
            SET_MENU(list[2], "Who", "who's online", data2, strlen(data2), print_result);
            SET_MENU(list[3], "User", "user functions", data5, strlen(data5), print_result);
            SET_MENU(list[4], "System", "system statistics", data6, strlen(data6), print_result);
            SET_MENU(list[5], "Aide", "aide options", data9, strlen(data9), print_result);
            SET_MENU(list[6], "Help", "show help", data8, strlen(data8), print_result);
            SET_MENU(list[7], "Logout", "logout of system", data9, strlen(data9), print_result);

        } else {

            errno = 0;
            list = calloc(7, sizeof(menus_list_t));
            if (list == NULL) cause_error(errno);
            list_size = 7 * sizeof(menus_list_t);

            SET_MENU(list[0], "Goto", "goto a specific room", NULL, 0, bbs_list_rooms);
            SET_MENU(list[1], "Enter", "enter the current room", data2, strlen(data2), print_result);
            SET_MENU(list[2], "Who", "who's online", data2, strlen(data2), print_result);
            SET_MENU(list[3], "User", "user functions", data5, strlen(data5), print_result);
            SET_MENU(list[4], "System", "system statistics", data6, strlen(data6), print_result);
            SET_MENU(list[5], "Help", "show help", data8, strlen(data8), print_result);
            SET_MENU(list[6], "Logout", "logout of system", data9, strlen(data9), print_result);

        }

        bmenu = bar_menu_create(startx, starty, 2, width, list, list_size);
        check_creation(bmenu);

        stat = menus_set_theme(bmenu, &theme);
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


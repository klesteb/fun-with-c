
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
#include "windows/base_window.h"
#include "components/menus/list.h"
#include "components/menus/menus.h"

extern int print_result();

/* local items ---------------------------------------------------------- */

int old;
int forward;

typedef struct _message_s {
    jam_message_t *message;
    queue fields;
    char *text;
} message_t;

/* ---------------------------------------------------------------------- */

static int set_forward(void *data, int size, error_trace_t *errors) {

    forward = TRUE;
    return OK;

}

static int set_reverse(void *data, int size, error_trace_t *errors) {

    forward = FALSE;
    return OK;

}

static int load_new_msgs(jam_t *jam, user_base_t *user, queue *results) {

    queue index;
    int stat = OK;
    ulong user_crc = 0;
    jam_lastread_t lastread;
    message_t *result = NULL;
    jam_search_t *search = NULL;

fprintf(stderr, "entering load_new_msgs()\n");

    when_error_in {

        stat = que_init(&index);
        check_status(stat, QUE_OK, E_INVOPS);
fprintf(stderr, "load_new_msgs() - past que_init()\n");

        stat = jam_crc32(jam, (uchar *)user->username, strlen(user->username), &user_crc);
        check_return(stat, jam);
fprintf(stderr, "load_new_msgs() - past jam_crc32()\n");

        stat = jam_get_lastread(jam, user_crc, &lastread);
        check_return(stat, jam);
fprintf(stderr, "load_new_msgs() - past jam_get_lastrun()\n");

        stat = jam_search_index(jam, 0, find_index_all, &index);
        check_return(stat, jam);
fprintf(stderr, "load_new_msgs() - past jam_search_index()\n");

        while ((search = que_pop_head(&index))) {

            if (search->msgnum > lastread.last_read_msg) {

                errno = 0;
                result = calloc(1, sizeof(message_t));
                if (result == NULL) cause_error(errno);

                stat = que_init(&result->fields);
                check_status(stat, QUE_OK, E_INVOPS);

                stat = jam_new_message(jam, &result->message);
                check_return(stat, jam);

                stat = jam_get_message(jam, search->msgnum, result->message, &result->fields, &result->text);
                check_return(stat, jam);

                if (forward) {

                    stat = que_push_head(results, result);
                    check_status(stat, QUE_OK, E_INVOPS);

                } else {

                    stat = que_push_tail(results, result);
                    check_status(stat, QUE_OK, E_INVOPS);

                }

            }

            free(search);

        }

        exit_when;

    } use {

        stat = ERR;
        capture_trace(dump);
        clear_error();

    } end_when;

fprintf(stderr, "leaving load_new_msgs() - stat: %d\n", stat);
    return stat;

}

static int load_old_msgs(jam_t *jam, user_base_t *user, queue *results) {

    queue index;
    int stat = OK;
    ulong user_crc = 0;
    jam_lastread_t lastread;
    message_t *result = NULL;
    jam_search_t *search = NULL;

    when_error_in {

        stat = que_init(&index);
        check_status(stat, QUE_OK, E_INVOPS);

        stat = jam_crc32(jam, (uchar *)user->username, strlen(user->username), &user_crc);
        check_return(stat, jam);

        stat = jam_get_lastread(jam, user_crc, &lastread);
        check_return(stat, jam);

        stat = jam_search_index(jam, 0, find_index_all, &index);
        check_return(stat, jam);

        while ((search = que_pop_head(&index))) {

            if (search->msgnum < lastread.last_read_msg) {

                errno = 0;
                result = calloc(1, sizeof(message_t));
                if (result == NULL) cause_error(errno);

                stat = que_init(&result->fields);
                check_status(stat, QUE_OK, E_INVOPS);

                stat = jam_new_message(jam, &result->message);
                check_return(stat, jam);

                stat = jam_get_message(jam, search->msgnum, result->message, &result->fields, &result->text);
                check_return(stat, jam);

                if (forward) {

                    stat = que_push_head(results, result);
                    check_status(stat, QUE_OK, E_INVOPS);

                } else {

                    stat = que_push_tail(results, result);
                    check_status(stat, QUE_OK, E_INVOPS);

                }

            }

            free(search);

        }

        exit_when;

    } use {

        stat = ERR;
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

int bbs_new_msgs(void *data, int size, error_trace_t *errors) {

    int stat = OK;
    queue results;
    jam_t *jam = (jam_t *)data;

fprintf(stderr, "entering bbs_new_msgs()\n");

    when_error_in {

        stat = que_init(&results);
        check_status(stat, QUE_OK, E_INVOPS);

        stat = load_new_msgs(jam, &useron, &results);
        check_return(stat, jam);

        exit_when;

    } use {

        stat = ERR;
        copy_error(errors);
        capture_trace(dump);
        clear_error();

    } end_when;

fprintf(stderr, "leaving bbs_new_msgs() - stat: %d\n", stat);
    return stat;

}

int bbs_old_msgs(void *data, int size, error_trace_t *errors) {

    int stat = OK;
    queue results;
    jam_t *jam = (jam_t *)data;

    when_error_in {

        stat = que_init(&results);
        check_status(stat, QUE_OK, E_INVOPS);

        stat = load_old_msgs(jam, &useron, &results);
        check_return(stat, jam);

        exit_when;

    } use {

        stat = ERR;
        copy_error(errors);
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

int bbs_msgs_menu(jam_t *jam, room_base_t *room, error_trace_t *errors) {

    int idx = 0;
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
    char *help = "messages";

    int list_size = 0;
    component_t *vline = NULL;
    component_t *bmenu = NULL;
    menus_list_t *list = NULL;

    /* theme.attribute  = A_NORMAL; */
    /* theme.foreground = BROWN; */
    /* theme.background = BLACK; */

fprintf(stderr, "entering bbs_msgs_menu()\n");

    when_error_in {

        old = FALSE;
        forward = TRUE;

        memset(title, '\0', 40);
        snprintf(title, 38, "Room: %s", room->name); 

        width  = (getmaxx(stdscr) - 4);
        height = (getmaxy(stdscr) - 7);
        startx = (getbegx(stdscr) + 1);
        starty = (getbegy(stdscr) + 4);

        errno = 0;
        list = calloc(count, sizeof(menus_list_t));
        if (list == NULL) cause_error(errno);

        SET_MENU(list[idx], "Forward", "forward retrieval of messages", MENUS_T_ITEM, NULL, 0, set_forward);
        idx++; SET_MENU(list[idx], "New", "read new messages", MENUS_T_ITEM, jam, sizeof(jam_t), bbs_new_msgs);
        idx++; SET_MENU(list[idx], "Old", "read old messages", MENUS_T_ITEM, jam, sizeof(jam_t), bbs_old_msgs);
        idx++; SET_MENU(list[idx], "Write", "create a new message", MENUS_T_ITEM, NULL, 0, print_result);
        idx++; SET_MENU(list[idx], "Reverse", "reverse retrieval of messages", MENUS_T_ITEM, NULL, 0, set_reverse);
        idx++; SET_MENU(list[idx], "Help", "help with messages", MENUS_T_ITEM, help, strlen(help), bbs_help);

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
            idx++; SET_MENU(list[idx], "Disable", "disable a user account", MENUS_T_ITEM, NULL, 0, print_result);

        }

        list_size = ((idx + 1) * sizeof(menus_list_t));

        win = base_window(title, startx, starty, height, width);
        check_creation(win);

        bmenu = list_menu_create(win, 0, 0, height - 2, 9, 1, bbs_send_message, list, list_size);
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

fprintf(stderr, "leaving bbs_msgs_menu() - stat: %d\n", stat);
    return stat;

}


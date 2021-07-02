
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
#include <time.h>

#include "bbs_common.h"
#include "bbs_protos.h"

#include "colors.h"
#include "window.h"
#include "fnm_util.h"
#include "component.h"
#include "misc/misc.h"
#include "windows/alert.h"
#include "components/text.h"
#include "components/more.h"
#include "windows/bar_menu.h"
#include "windows/box_menu.h"
#include "components/hline.h"
#include "windows/base_window.h"
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

int bbs_user(void *data, int size, error_trace_t *errors) {

    queue lines;
    theme_t theme;
    int stat = OK;
    int width = 0;
    char title[40];
    int height = 0;
    int startx = 0;
    int starty = 0;
    char *line = NULL;
    error_trace_t error;
    window_t *temp = NULL;
    component_t *more = NULL;

    when_error_in {

        stat = que_init(&lines);
        check_status(stat, QUE_OK, E_INVOPS);

        line = spaces(80);
        sprintf(line, "User Name         : %s\n", useron.username);
        stat = que_push_tail(&lines, line);
        check_status(stat, QUE_OK, E_NOQUEUE);

        line = spaces(80);
        sprintf(line, "User Number       : %ld\n", useron.eternal);
        stat = que_push_tail(&lines, line);
        check_status(stat, QUE_OK, E_NOQUEUE);

        line = spaces(80);
        sprintf(line, "Terminal          : %s\n", useron.term);
        stat = que_push_tail(&lines, line);
        check_status(stat, QUE_OK, E_NOQUEUE);

        line = spaces(80);
        sprintf(line, "Columns           : %d\n", useron.cols);
        stat = que_push_tail(&lines, line);
        check_status(stat, QUE_OK, E_NOQUEUE);

        line = spaces(80);
        sprintf(line, "Rows              : %d\n", useron.rows);
        stat = que_push_tail(&lines, line);
        check_status(stat, QUE_OK, E_NOQUEUE);

        line = spaces(80);
        sprintf(line, "AX Level          : %s\n", axdefs[useron.axlevel]);
        stat = que_push_tail(&lines, line);
        check_status(stat, QUE_OK, E_NOQUEUE);

        line = spaces(80);
        sprintf(line, "QWK Flags         : (");
        
        if (bit_test(useron.qwk, QWK_FILES)) strcat(line, "FILES,");
        if (bit_test(useron.qwk, QWK_EMAIL)) strcat(line, "EMAIL,");
        if (bit_test(useron.qwk, QWK_ALLMAIL)) strcat(line, "ALLMAIL,");
        if (bit_test(useron.qwk, QWK_DELMAIL)) strcat(line, "DELMAIL,");
        if (bit_test(useron.qwk, QWK_BYSELF)) strcat(line, "BYSELF,");
        if (bit_test(useron.qwk, QWK_EXPCTLA)) strcat(line, "EXPCTLA,");
        if (bit_test(useron.qwk, QWK_RETCTLA)) strcat(line, "RETCTLA,");
        if (bit_test(useron.qwk, QWK_ATTACH)) strcat(line, "ATTACH,");
        if (bit_test(useron.qwk, QWK_NOINDEX)) strcat(line, "NOINDEX,");
        if (bit_test(useron.qwk, QWK_TZ)) strcat(line, "TZ,");
        if (bit_test(useron.qwk, QWK_VIA)) strcat(line, "VIA,");
        if (bit_test(useron.qwk, QWK_NOCTRL)) strcat(line, "NOCTRL");

        if (line[strlen(line) - 1] == ',') line[strlen(line) - 1] = '\0';
        strcat(line, ")");

        stat = que_push_tail(&lines, line);
        check_status(stat, QUE_OK, E_NOQUEUE);

        line = spaces(256);
        sprintf(line, "%s", "Flags             : (");

        if (bit_test(useron.flags, US_SYSOP)) strcat(line, "SYSOP,");
        if (bit_test(useron.flags, US_ROOMAIDE)) strcat(line, "ROOMAIDE,");
        if (bit_test(useron.flags, US_NEEDVALID)) strcat(line, "NEEDVALID,");
        if (bit_test(useron.flags, US_SUBSYSTEM)) strcat(line, "SUBSYSTEM,");
        if (bit_test(useron.flags, US_PERM)) strcat(line, "PERM,");
        if (bit_test(useron.flags, US_EXPERT)) strcat(line, "EXPERT,");
        if (bit_test(useron.flags, US_NOHIDE)) strcat(line, "NOHIDE,");
        if (bit_test(useron.flags, US_REGIS)) strcat(line, "REGIS,");
        if (bit_test(useron.flags, US_PROFILE)) strcat(line, "PROFILE,");
        if (bit_test(useron.flags, US_INACTIVE)) strcat(line, "INACTIVE,");
        if (bit_test(useron.flags, US_DELETED)) strcat(line, "DELETED");

        if (line[strlen(line) - 1] == ',') line[strlen(line) - 1] = '\0';
        strcat(line, ")");

        stat = que_push_tail(&lines, line);
        check_status(stat, QUE_OK, E_NOQUEUE);

        line = spaces(80);
        sprintf(line, "Messages Posted   : %d\n", useron.posted);
        stat = que_push_tail(&lines, line);
        check_status(stat, QUE_OK, E_NOQUEUE);

        line = spaces(80);
        sprintf(line, "Times Called      : %d\n", useron.timescalled);
        stat = que_push_tail(&lines, line);
        check_status(stat, QUE_OK, E_NOQUEUE);

        line = spaces(80);
        sprintf(line, "Online Today      : %-6ld (minutes)\n", useron.today);
        stat = que_push_tail(&lines, line);
        check_status(stat, QUE_OK, E_NOQUEUE);
        
        line = spaces(80);
        sprintf(line, "Daily Time Limit  : %-6ld (minutes)\n", useron.timelimit);
        stat = que_push_tail(&lines, line);
        check_status(stat, QUE_OK, E_NOQUEUE);

        line = spaces(80);
        sprintf(line, "Total Time Online : %-6ld (minutes)\n", useron.online);
        stat = que_push_tail(&lines, line);
        check_status(stat, QUE_OK, E_NOQUEUE);

        line = spaces(80);
        struct tm *lc = localtime(&useron.lastcall);
        sprintf(line, "Last Call         : %d-%02d-%02d %02d:%02d:%02d\n", lc->tm_year + 1900, lc->tm_mon + 1, lc->tm_mday, lc->tm_hour, lc->tm_min, lc->tm_sec);
        stat = que_push_tail(&lines, line);
        check_status(stat, QUE_OK, E_NOQUEUE);

        line = spaces(80);
        struct tm *fc = localtime(&useron.firstcall);
        sprintf(line, "First Call        : %d-%02d-%02d %02d:%02d:%02d\n", fc->tm_year + 1900, fc->tm_mon + 1, fc->tm_mday, fc->tm_hour, fc->tm_min, fc->tm_sec);
        stat = que_push_tail(&lines, line);
        check_status(stat, QUE_OK, E_NOQUEUE);

        memset(title, '\0', 40);
        snprintf(title, 38, "User: %s", useron.username); 

        width  = (getmaxx(stdscr) - 4);
        height = (getmaxy(stdscr) - 7);
        startx = (getbegx(stdscr) + 1);
        starty = (getbegy(stdscr) + 4);

        temp = base_window(title, startx, starty, height, width);
        check_creation(temp);

        more = more_create(temp, 1, 1, height - 3, width - 2, 1, &lines);
        check_creation(more);

        stat = window_add(temp, more);
        check_return(stat, temp);

        stat = workbench_add(workbench, temp);
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

int bbs_who(void *data, int size, error_trace_t *errors) {

    queue lines;
    queue results;
    theme_t theme;
    int stat = OK;
    int width = 0;
    char action[32];
    char title[40];
    int height = 0;
    int startx = 0;
    int starty = 0;
    user_base_t user;
    char *line = NULL;
    error_trace_t error;
    window_t *temp = NULL;
    component_t *more = NULL;
    node_search_t *result = NULL;

    when_error_in {

        memset(action, '\0', 32);

        stat = que_init(&lines);
        check_status(stat, QUE_OK, E_INVOPS);

        stat = que_init(&results);
        check_status(stat, QUE_OK, E_INVOPS);

        stat = node_search(nodes, NULL, 0, find_nodes_active, node_capture, &results);
        check_return(stat, nodes);

        line = spaces(80);
        sprintf(line, "Active Nodes: %d/%d\n", que_size(&results), nodenum);
        stat = que_push_tail(&lines, line);
        check_status(stat, QUE_OK, E_NOQUEUE);

        line = spaces(80);
        sprintf(line, "%s\n", " ");
        stat = que_push_tail(&lines, line);
        check_status(stat, QUE_OK, E_NOQUEUE);

        line = strdup("  Node   Handle                           Action\n");
        stat = que_push_tail(&lines, line);
        check_status(stat, QUE_OK, E_NOQUEUE);

        line = strdup("--------------------------------------------------------------------------\n");
        stat = que_push_tail(&lines, line);
        check_status(stat, QUE_OK, E_NOQUEUE);

        while ((result = que_pop_head(&results))) {

            stat = user_get(users, result->useron, &user);
            check_return(stat, users);

            if (result->action == NODE_MAIN) strcpy(action, "Main Menu");
            if (result->action == NODE_RMSG) strcpy(action, "Reading Messages");
            if (result->action == NODE_RMAL) strcpy(action, "Reading Mail");
            if (result->action == NODE_SMAL) strcpy(action, "Sending Mail");
            if (result->action == NODE_RTXT) strcpy(action, "Reading Bulletins");
            if (result->action == NODE_RSML) strcpy(action, "Reading Sent Mail");
            if (result->action == NODE_PMSG) strcpy(action, "Posting Message");
            if (result->action == NODE_AMSG) strcpy(action, "Auto Message");
            if (result->action == NODE_XTRN) strcpy(action, "External Program");
            if (result->action == NODE_DFLT) strcpy(action, "Main Defaults");
            if (result->action == NODE_XFER) strcpy(action, "Transfer Prompt");
            if (result->action == NODE_DLNG) strcpy(action, "Downloading File");
            if (result->action == NODE_ULNG) strcpy(action, "Uploading File");
            if (result->action == NODE_BXFR) strcpy(action, "Bidirectional Transfer");
            if (result->action == NODE_LFIL) strcpy(action, "Listing Files");
            if (result->action == NODE_LOGN) strcpy(action, "Logging on");
            if (result->action == NODE_LCHT) strcpy(action, "Local Chat");
            if (result->action == NODE_MCHT) strcpy(action, "Multi Chat");
            if (result->action == NODE_CHAT) strcpy(action, "Chat");
            if (result->action == NODE_SYSP) strcpy(action, "Sysop Activity");
            if (result->action == NODE_TQWK) strcpy(action, "Transfering QWK Packet");
            if (result->action == NODE_PCHT) strcpy(action, "Private Chat");
            if (result->action == NODE_PAGE) strcpy(action, "Paging Others");
            if (result->action == NODE_RFSD) strcpy(action, "Retrieving Files");
            if (result->action == NODE_OFFL) strcpy(action, "Node Offline");

            line = spaces(80);
            snprintf(line, 80, "  %-5d  %-31s  %-31s\n", result->record, user.username, action);
            stat = que_push_tail(&lines, line);
            check_status(stat, QUE_OK, E_NOQUEUE);

            free(result);

        }

        memset(title, '\0', 40);
        snprintf(title, 38, "Who"); 

        width  = (getmaxx(stdscr) - 4);
        height = (getmaxy(stdscr) - 7);
        startx = (getbegx(stdscr) + 1);
        starty = (getbegy(stdscr) + 4);

        temp = base_window(title, startx, starty, height, width);
        check_creation(temp);

        more = more_create(temp, 1, 1, height - 3, width - 2, 1, &lines);
        check_creation(more);

        stat = window_add(temp, more);
        check_return(stat, temp);

        stat = workbench_add(workbench, temp);
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

int bbs_system(void *data, int size, error_trace_t *errors) {

    int len = 0;
    queue lines;
    theme_t theme;
    int stat = OK;
    int width = 0;
    char title[40];
    int height = 0;
    int startx = 0;
    int starty = 0;
    char *no = "no";
    char *yes = "yes";
    char *line = NULL;
    error_trace_t error;
    window_t *temp = NULL;
    component_t *more = NULL;

    when_error_in {

        stat = que_init(&lines);
        check_status(stat, QUE_OK, E_INVOPS);

        line = spaces(80);
        sprintf(line, "Version : %s\n", bbs_version());
        stat = que_push_tail(&lines, line);
        check_status(stat, QUE_OK, E_NOQUEUE);

        line = spaces(80);
        sprintf(line, "Serial #: %s\n", serialnum);
        stat = que_push_tail(&lines, line);
        check_status(stat, QUE_OK, E_NOQUEUE);

        line = spaces(80);
        sprintf(line, "%s\n", " ");
        stat = que_push_tail(&lines, line);
        check_status(stat, QUE_OK, E_NOQUEUE);

        line = spaces(80);
        sprintf(line, "This systems's nodename is      : %s\n", networknode);
        stat = que_push_tail(&lines, line);
        check_status(stat, QUE_OK, E_NOQUEUE);

        line = spaces(80);
        sprintf(line, "This system's human name is     : %s\n", humannode);
        stat = que_push_tail(&lines, line);
        check_status(stat, QUE_OK, E_NOQUEUE);

        line = spaces(80);
        sprintf(line, "This system is networked        : %s\n", (networked) ? yes : no);
        stat = que_push_tail(&lines, line);
        check_status(stat, QUE_OK, E_NOQUEUE);

        if (useron.axlevel >= AX_AIDE) {

            line = spaces(80);
            sprintf(line, "%s\n", " ");
            stat = que_push_tail(&lines, line);
            check_status(stat, QUE_OK, E_NOQUEUE);

            if (useron.axlevel > AX_AIDE) {

                /* aides don't need to know the paths used by the system */

                line = spaces(80);
                sprintf(line, "The path to the bbs is          : %s\n", bbsdir);
                stat = que_push_tail(&lines, line);
                check_status(stat, QUE_OK, E_NOQUEUE);

                line = spaces(80);
                sprintf(line, "The path to the messages is     : %s\n", msgpath);
                stat = que_push_tail(&lines, line);
                check_status(stat, QUE_OK, E_NOQUEUE);

                line = spaces(80);
                sprintf(line, "The path to the datastore is    : %s\n", datapath);
                stat = que_push_tail(&lines, line);
                check_status(stat, QUE_OK, E_NOQUEUE);

                line = spaces(80);
                sprintf(line, "The path to the text files is   : %s\n", textpath);
                stat = que_push_tail(&lines, line);
                check_status(stat, QUE_OK, E_NOQUEUE);

                line = spaces(80);
                sprintf(line, "%s\n", " ");
                stat = que_push_tail(&lines, line);
                check_status(stat, QUE_OK, E_NOQUEUE);
                
            }

            line = spaces(80);
            sprintf(line, "Minimum axlevel to create rooms : %d (%s)\n", makeroom, axdefs[makeroom]);
            stat = que_push_tail(&lines, line);
            check_status(stat, QUE_OK, E_NOQUEUE);

            line = spaces(80);
            sprintf(line, "Minimum axlevel to post in lobby: %d (%s)\n", lobbypost, axdefs[lobbypost]);
            stat = que_push_tail(&lines, line);
            check_status(stat, QUE_OK, E_NOQUEUE);

            line = spaces(80);
            sprintf(line, "Initial new user access level   : %d (%s)\n", initax, axdefs[initax]);
            stat = que_push_tail(&lines, line);
            check_status(stat, QUE_OK, E_NOQUEUE);

            line = spaces(80);
            sprintf(line, "%s\n", " ");
            stat = que_push_tail(&lines, line);
            check_status(stat, QUE_OK, E_NOQUEUE);

            line = spaces(80);
            sprintf(line, "Automatic room aide assignment  : %s\n", (creataide) ? yes : no);
            stat = que_push_tail(&lines, line);
            check_status(stat, QUE_OK, E_NOQUEUE);

            line = spaces(80);
            sprintf(line, "Default user time limit         : %d (minutes)\n", timelim);
            stat = que_push_tail(&lines, line);
            check_status(stat, QUE_OK, E_NOQUEUE);

            line = spaces(80);
            sprintf(line, "Users are required to register  : %s\n", (regiscall) ? yes : no);
            stat = que_push_tail(&lines, line);
            check_status(stat, QUE_OK, E_NOQUEUE);

            line = spaces(80);
            sprintf(line, "%s\n", " ");
            stat = que_push_tail(&lines, line);
            check_status(stat, QUE_OK, E_NOQUEUE);

            line = spaces(80);
            sprintf(line, "Maximum rooms in the system     : %d\n", roomnum);
            stat = que_push_tail(&lines, line);
            check_status(stat, QUE_OK, E_NOQUEUE);

            line = spaces(80);
            sprintf(line, "Maximum nodes in system         : %d\n", nodenum);
            stat = que_push_tail(&lines, line);
            check_status(stat, QUE_OK, E_NOQUEUE);

            line = spaces(80);
            sprintf(line, "Maximum users in the system     : %d\n", usernum);
            stat = que_push_tail(&lines, line);
            check_status(stat, QUE_OK, E_NOQUEUE);

            line = spaces(80);
            sprintf(line, "%s\n", " ");
            stat = que_push_tail(&lines, line);
            check_status(stat, QUE_OK, E_NOQUEUE);

        }

        memset(title, '\0', 40);
        snprintf(title, 38, "System Config"); 

        width  = (getmaxx(stdscr) - 4);
        height = (getmaxy(stdscr) - 7);
        startx = (getbegx(stdscr) + 1);
        starty = (getbegy(stdscr) + 4);

        temp = base_window(title, startx, starty, height, width);
        check_creation(temp);

        more = more_create(temp, 1, 1, height - 3, width - 2, 1, &lines);
        check_creation(more);

        stat = window_add(temp, more);
        check_return(stat, temp);

        stat = workbench_add(workbench, temp);
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

int bbs_help(void *data, int size, error_trace_t *errors) {

    int len = 0;
    queue lines;
    theme_t theme;
    int stat = OK;
    int width = 0;
    char title[40];
    int height = 0;
    int startx = 0;
    int starty = 0;
    char file[256];
    FileName fname;
    error_trace_t error;
    window_t *temp = NULL;
    component_t *more = NULL;

    when_error_in {

        stat = que_init(&lines);
        check_status(stat, QUE_OK, E_INVOPS);

        memset(file, '\0', 256);
        len = (size > 255) ? 255 : size;
        strncpy(file, (char *)data, len);

        fname = fnm_create(1, file, ".hlp", textpath, NULL);

        stat = bbs_load_file(fnm_path(fname), &lines, &error);
        check_status2(stat, OK, error);

        memset(title, '\0', 40);
        snprintf(title, 38, "Help: %s", file); 

        width  = (getmaxx(stdscr) - 4);
        height = (getmaxy(stdscr) - 7);
        startx = (getbegx(stdscr) + 1);
        starty = (getbegy(stdscr) + 4);

        temp = base_window(title, startx, starty, height, width);
        check_creation(temp);

        more = more_create(temp, 1, 1, height - 3, width - 2, 1, &lines);
        check_creation(more);

        stat = window_add(temp, more);
        check_return(stat, temp);

        stat = workbench_add(workbench, temp);
        check_return(stat, workbench);

        fnm_destroy(fname);

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
    char *xmain = "main";
    error_trace_t error;
    window_t *bmenu = NULL;
    menus_list_t *list = NULL;


    theme.attribute  = A_NORMAL;
    theme.foreground = BROWN;
    theme.background = BLACK;

    when_error_in {

        stat = bbs_send_status(NODE_MAIN, &error);
        check_status2(stat, OK, error);

        startx = getbegx(stdscr);
        starty = getbegy(stdscr);
        width  = getmaxx(stdscr) - 2;

        errno = 0;
        list = calloc(6, sizeof(menus_list_t));
        if (list == NULL) cause_error(errno);
        list_size = 6 * sizeof(menus_list_t);

        SET_MENU(list[0], "Rooms", "goto a specific room", MENUS_T_ITEM, NULL, 0, bbs_list_rooms);
        SET_MENU(list[1], "Who", "who's online", MENUS_T_ITEM, NULL, 0, bbs_who);
        SET_MENU(list[2], "User", "user maintence", MENUS_T_ITEM, NULL, 0, bbs_user);
        SET_MENU(list[3], "System", "system statistics", MENUS_T_ITEM, NULL, 0, bbs_system);
        SET_MENU(list[4], "Help", "show help", MENUS_T_ITEM, xmain, strlen(xmain), bbs_help);
        SET_MENU(list[5], "Logout", "logout of system", MENUS_T_ITEM, NULL, 0, bbs_logout);

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


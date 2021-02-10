
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
#include <stdlib.h>
#include <errno.h>

#include "msgs.h"
#include "node.h"
#include "room.h"
#include "user.h"
#include "when.h"
#include "event.h"
#include "finds.h"
#include "files.h"
#include "errors.h"
#include "tracer.h"
#include "fnm_util.h"
#include "misc/misc.h"
#include "workbench.h"
#include "interfaces.h"
#include "bbs_errors.h"
#include "bbs_workbench.h"
#include "errors_ncurses.h"
#include "bbs_error_codes.h"

/* global items */

node_t *nodes = NULL;
user_t *users = NULL;
room_t *rooms = NULL;
tracer_t *dump = NULL;
errors_t *errs = NULL;
event_t *events = NULL;

workbench_t *workbench = NULL;
window_t *available_rooms = NULL;

int xnode = 1;
int sysop = FALSE;
int user_index = 0;
int qroom_index = 0;
int qnode_index = 0;
char *username = NULL;

room_base_t qroom;
user_base_t useron;
node_base_t qnode;

/*---------------------------------------------------------------------------*/

char *bbs_version(void) {

    char *version = VERSION;
    return version;

}

int setup(error_trace_t *errors) {

    int stat = OK;
    int lobby = LOBBY;

    int base = 1;
    int timeout = 1;
    int retries = 30;
    int roomnum = 32;
    int nodenum = 32;
    int usernum = 256;
    char *datapath = "../../data/";
    char *msgpath = "../../messages/";

    when_error_in {

        /* setup error handling */

        errs = errors_create();
        check_creation(errs);

        stat = errors_load(errs, bbs_codes, sizeof(bbs_codes));
        check_return(stat, errs);

        stat = errors_load(errs, ncurses_codes, sizeof(ncurses_codes));
        check_return(stat, errs);

        dump = tracer_create(errs);
        check_creation(dump);

        /* create the objects */

        workbench = bbs_workbench_create();
        check_creation(workbench);

        events = event_create();
        check_creation(events);

        rooms = room_create(datapath, msgpath, roomnum, retries, timeout, base, dump);
        check_creation(rooms);

        users = user_create(datapath, usernum, retries, timeout, dump);
        check_creation(users);

        nodes = node_create(datapath, nodenum, retries, timeout, dump);
        check_creation(nodes);

        /* access the databases */

        stat = room_open(rooms);
        check_return(stat, rooms);

        stat = user_open(users);
        check_return(stat, users);

        stat = node_open(nodes);
        check_return(stat, nodes);

        /* load the node record */
        
        stat = node_find(nodes, &xnode, sizeof(int), find_node_by_number, &qnode_index);
        check_return(stat, nodes);

        if (qnode_index > 0) {

            stat = node_get(nodes, qnode_index, &qnode);
            check_return(stat, nodes);

        } else {

            cause_error(E_UNKNODE);

        }

        /* load the user record */
        
        stat = user_find(users, username, strlen(username), find_user_by_name, &user_index);
        check_return(stat, users);

        if (user_index > 0) {

            stat = user_get(users, user_index, &useron);
            check_return(stat, users);

        } else {

            cause_error(E_UNKUSER);

        }

        /* load the lobby */

        stat = room_find(rooms, &lobby, sizeof(int), find_room_by_number, &qroom_index);
        check_return(stat, rooms);

        if (qroom_index > 0) {

            stat = room_get(rooms, qroom_index, &qroom);
            check_return(stat, rooms);

        } else {

            cause_error(E_UNKROOM);

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

int cleanup(void) {

    room_close(rooms);
    room_destroy(rooms);

    user_close(users);
    user_destroy(users);

    node_close(nodes);
    node_destroy(nodes);

    tracer_destroy(dump);
    errors_destroy(errs);
    event_destroy(events);
    workbench_destroy(workbench);

    return OK;

}

int print_dump(char *string) {

    fprintf(stderr, "%s\n",string);
    return OK;

}

int main(int argc, char **argv) {

    int c = 0;
    int stat = OK;
    extern int optind;
    extern char *optarg;
    char *configs = NULL;
    error_trace_t errors;
    int rc = EXIT_SUCCESS;
    char opts[] = "c:n:u:vsh?";

    opterr = 0;
    username = strndup(getenv("LOGNAME"), LEN_NAME);

    while ((c = getopt(argc, argv, opts)) != -1) {

        switch(c) {
            case 'c':
                configs = strdup(optarg);
                break;
            case 's':
                sysop = TRUE;
                free(username);
                username = strdup("sysop");
                break;
            case 'n':
                xnode = atol(optarg);
                break;
            case 'u':
                free(username);
                username = strndup(optarg, LEN_NAME);
                break;
            case 'h':
                printf("\n");
                printf("xa-bbs %s - linux port\n", bbs_version());
                printf("\n");
                printf("Usage: xa-bbs [-s] [-n <number>] [-c <filename>]\n");
                printf("\n");
                printf("    -s - enable sysop options.\n");
                printf("    -v - shows the version number.\n");
                printf("    -n <node> - node number of this instance.\n");
                printf("    -c <filename> - use this configuration file.\n");
                printf("\n");
                goto fini;
                break;
            case '?':
                printf("Usage: xa-bbs [-s] [-n <number>] [-c <filename>]\n");
                goto fini;
                break;
            case 'v':
                printf("xa-bbs - v%s\n", bbs_version());
                goto fini;
                break;
        }

    }

    when_error_in {

        stat = setup(&errors);
        check_status2(stat, OK, errors);

        /* stat = bbs_init_terminal(&errors); */
        /* check_status2(stat, OK, errors); */

        stat = bbs_run(&errors);
        check_status2(stat, OK, errors);

        stat = bbs_logoff(&errors);
        check_status2(stat, OK, errors);

        exit_when;

    } use {

        rc = EXIT_FAILURE;
        capture_trace(dump);
        tracer_dump(dump, print_dump);
        clear_error();

    } end_when;

    cleanup();

    fini:
    return rc;

}


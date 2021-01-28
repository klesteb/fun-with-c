
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
#include "finds.h"
#include "files.h"
#include "errors.h"
#include "events.h"
#include "tracer.h"
#include "fnm_util.h"
#include "misc/misc.h"
#include "interfaces.h"

/* global items */

node_t *nodes = NULL;
user_t *users = NULL;
room_t *rooms = NULL;
tracer_t *dump = NULL;
errors_t *errs = NULL;
events_t *events = NULL;

int xnode = 1;
int sysop = FALSE;

char *bbs_version(void) {
    
    char *version = VERSION;
    return version;

}

int setup(void) {

    int base = 1;
    int stat = OK;
    int timeout = 1;
    int retries = 30;
    int roomnum = 32;
    int nodenum = 32;
    int usernum = 256;
    char *datapath = "../../data/";
    char *msgpath = "../../messages/";

    when_error_in {

        errs = errors_create();
        check_creation(errs);

        dump = tracer_create(errs);
        check_creation(dump);

        rooms = room_create(datapath, msgpath, roomnum, retries, timeout, base, dump);
        check_creation(rooms);

        users = user_create(datapath, usernum, retries, timeout, dump);
        check_creation(users);

        nodes = node_create(datapath, nodenum, retries, timeout, dump);
        check_creation(nodes);

        events = events_create();
        check_creation(events);

        /* access the databases */

        stat = room_open(rooms);
        check_return(stat, rooms);
        
        stat = user_open(users);
        check_return(stat, users);
        
        stat = node_open(nodes);
        check_return(stat, nodes);

        exit_when;

    } use {

        stat = ERR;
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

int cleanup(void) {

    room_destroy(rooms);
    user_destroy(users);
    node_destroy(nodes);
    tracer_destroy(dump);
    errors_destroy(errs);
    events_destroy(events);

    return OK;

}

int print_dump(char *string) {

    fprintf(stderr, string);
    return OK;

}

int main(int argc, char **argv) {

    int c = 0;
    int stat = OK;
    extern int optind;
    extern char *optarg;
    char *configs = NULL;
    int rc = EXIT_SUCCESS;
    char opts[] = "c:n:vsh?";

    opterr = 0;

    while ((c = getopt(argc, argv, opts)) != -1) {

        switch(c) {
            case 'c':
                configs = argv[optind];
                break;
            case 's':
                sysop = TRUE;
                break;
            case 'n':
                xnode = atol(argv[optind]);
                break;
            case 'h':
                printf("\n");
                printf("xa-bbs %s - Linux port\n", bbs_version());
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

        stat = setup();
        check_status(stat, OK, E_INVOPS);

        stat = bbs_run();
        check_status(stat, OK, E_INVOPS);

        stat = bbs_logoff();
        check_status(stat, OK, E_INVOPS);

        exit_when;

    } use {

        rc = EXIT_FAILURE;
        tracer_dump(dump, print_dump);
        clear_error();

    } end_when;

    cleanup();

    fini:
    return rc;

}


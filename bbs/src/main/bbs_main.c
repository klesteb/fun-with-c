
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
#include "bbs_config.h"
#include "bbs_protos.h"
#include "bbs_errors.h"
#include "room_status.h"
#include "errors_ncurses.h"
#include "bbs_error_codes.h"

/* global items ---------------------------------------------------------- */

rms_t *nodes = NULL;
rms_t *users = NULL;
room_t *rooms = NULL;
rms_t *rstatus = NULL;
tracer_t *dump = NULL;
errors_t *errs = NULL;
event_t *events = NULL;
workbench_t *workbench = NULL;

int xnode = 1;
int sysop = FALSE;
off_t user_index = 0;
off_t qnode_index = 0;
char *username = NULL;

user_base_t useron;
node_base_t qnode;

/* config items -----------------------------------------------------------*/

int base = MSGBASE;                 /* base message number                 */
int xtimeout = TIMEOUT;             /* timeout for file locking            */
int retries = RETRIES;              /* retires for file locking            */
int roomnum = ROOMNUM;              /* max number of rooms                 */
int nodenum = NODENUM;              /* max number of nodes                 */
int usernum = USERNUM;              /* max number of users                 */
int networked = NETWORKED;          /* this system belongs to a network    */
int creataide = CREATAIDE;          /* automatic room aide assignments     */
int lobbypost = LOBBYPOST;          /* AX level to be able to post in lobby */
int makeroom = MAKEROOM;            /* AX level to be create a new room    */ 
int initax = INITAX;                /* AX level for new users              */
int validax = VALIDAX;              /* AX level for validated users        */
int regiscall = REGISCALL;          /* user must register on first call    */
int timelim = TIMELIM;              /* default time limit on system        */
char *bbsdir = BBSDIR;              /* root directory of the bbs           */
char *datapath = DATAPATH;          /* where the data files are located    */
char *msgpath = MSGPATH;            /* where the messages are located      */
char *textpath = TEXTPATH;          /* where the text files are located    */
char *networknode = NODENAME;       /* network node name                   */
char *humannode = HUMANNODE;        /* human readable node name            */
char *serialnum = SERIALNUM;        /* system serial number                */
int rstatnum = 0;                   /* max number of room status records   */

/* nemonics ---------------------------------------------------------------*/

char *axdefs[8]= {
    "Marked for deletion",
    "New User",
    "Read-Only User",
    "Normal User",
    "Subsystem User",
    "Preferred User",
    "Aide",
    "Sysop"
};

/*-------------------------------------------------------------------------*/

int bbs_init(error_trace_t *errors) {
    
    int stat = OK;

    when_error_in {

        /* access the databases */

        stat = room_open(rooms);
        check_return(stat, rooms);

        stat = user_open(users);
        check_return(stat, users);

        stat = node_open(nodes);
        check_return(stat, nodes);

        stat = room_status_open(rstatus);
        check_return(stat, rstatus);

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

        exit_when;

    } use {

        stat = ERR;
        copy_error(errors);
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

} 

char *bbs_version(void) {

    char *version = VERSION;
    return version;

}

int setup(error_trace_t *errors) {

    int stat = OK;
    FileName dpath;
    FileName mpath;

    when_error_in {

        dpath = fnm_create(1, datapath, NULL);
        if (dpath == NULL) cause_error(E_UNKFILE);

        mpath = fnm_create(1, msgpath, NULL);
        if (mpath == NULL) cause_error(E_UNKFILE);

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

        workbench = workbench_create(NULL);
        check_creation(workbench);

        events = event_create();
        check_creation(events);

        rooms = room_create(fnm_directory(dpath), fnm_path(mpath), roomnum, retries, xtimeout, base, dump);
        check_creation(rooms);

        rstatnum = roomnum * usernum;
        rstatus = room_status_create(fnm_directory(dpath), rstatnum, retries, xtimeout, dump);
        check_creation(rstatus);

        users = user_create(fnm_directory(dpath), usernum, retries, xtimeout, dump);
        check_creation(users);

        nodes = node_create(fnm_directory(dpath), nodenum, retries, xtimeout, dump);
        check_creation(nodes);

        fnm_destroy(dpath);
        fnm_destroy(mpath);

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

    room_status_close(rstatus);
    room_status_destroy(rstatus);

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
    error_trace_t errors;
    int rc = EXIT_SUCCESS;
    char opts[] = "n:u:vsh?";

    opterr = 0;
    username = strndup(getenv("LOGNAME"), LEN_NAME);

    while ((c = getopt(argc, argv, opts)) != -1) {

        switch(c) {
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
                printf("xa-bbs v%s - linux port\n", bbs_version());
                printf("\n");
                printf("Usage: xa-bbs [-s | -u <user name>] [-n <number>]\n");
                printf("\n");
                printf("    -s - enable sysop options.\n");
                printf("    -v - shows the version number.\n");
                printf("    -u <user name> - use this user.\n");
                printf("    -n <node> - node number of this instance.\n");
                printf("\n");
                goto fini;
                break;
            case '?':
                printf("Usage: xa-bbs [-s | -u <user name>] [-n <number>]\n");
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

        stat = bbs_init(&errors);
        check_status2(stat, OK, errors);

        stat = bbs_main_menu(&errors);
        check_status2(stat, OK, errors);

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


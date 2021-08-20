
#include <stdio.h>
#include <errno.h>
#include <time.h>

#include "files/files.h"
#include "gpl/fnm_util.h"
#include "include/when.h"
#include "tracer/tracer.h"
#include "errors/errors.h"
#include "cclibs/misc/misc.h"
#include "cclibs/misc/dates.h"

#include "bbs/src/door/door.h"
#include "bbs/src/node/node.h"
#include "bbs/src/room/room.h"
#include "bbs/src/user/user.h"
#include "bbs/src/finds/finds.h"
#include "bbs/src/main/bbs_config.h"
#include "bbs/src/main/bbs_errors.h"
#include "bbs/src/main/bbs_error_codes.h"

rms_t *nodes = NULL;
rms_t *users = NULL;
room_t *rooms = NULL;
door_t *doors = NULL;
tracer_t *dump = NULL;
errors_t *errs = NULL;

room_base_t *room;
node_base_t xnode;
user_base_t useron;
user_base_t sysop;

int dump_trace(char *buffer) {

    fprintf(stderr, "%s\n", buffer);

    return OK;

}

int setup(void) {

    int stat = OK;
    time_t laston;
    int nodenum = 1;
    char *name = "00004";
    off_t user_index = 0;
    off_t qnode_index = 0;
    char username[LEN_NAME+1];
    char *path = "../../doors";

    when_error_in {

        /* create resources */

        errs = errors_create();
        check_creation(errs);

        stat = errors_load(errs, bbs_codes, sizeof(bbs_codes));
        check_return(stat, errs);

        dump = tracer_create(errs);
        check_creation(dump);

        rooms = room_create(DATAPATH, ROOMNUM, RETRIES, TIMEOUT, MSGBASE, dump);
        check_creation(rooms);

        users = user_create(DATAPATH, USERNUM, RETRIES, TIMEOUT, dump);
        check_creation(users);

        nodes = node_create(DATAPATH, NODENUM, RETRIES, TIMEOUT, dump);
        check_creation(nodes);

        doors = door_create(path, name, RETRIES, TIMEOUT, dump);
        check_creation(doors);

        /* open resources */

        stat = room_open(rooms);
        check_return(stat, rooms);

        stat = user_open(users);
        check_return(stat, users);

        stat = node_open(nodes);
        check_return(stat, nodes);

        stat = door_open(doors);
        check_return(stat, doors);

        /* load node 0 */

        stat = node_find(nodes, &nodenum, sizeof(int), find_node_by_number, &qnode_index);
        check_return(stat, nodes);

        if (qnode_index > 0) {

            stat = node_get(nodes, qnode_index, &xnode);
            check_return(stat, nodes);

        } else {

            cause_error(E_UNKNODE);

        }

        /* load the guest record */

        strcpy(username, "guest");
        stat = user_find(users, username, strlen(username), find_user_by_name, &user_index);
        check_return(stat, users);

        if (user_index > 0) {

            stat = user_get(users, user_index, &useron);
            check_return(stat, users);

            useron.timescalled++;
            laston = useron.lastcall;
            useron.lastcall = time(NULL);

            DATE a = time_to_date(laston);
            DATE b = time_to_date(useron.lastcall);

            if (date_compare(a, b) != 0) {

                useron.online += useron.today;
                useron.today = 0;

            }

            if (useron.firstcall == 0) {

                useron.firstcall = time(NULL);

            }

            if (useron.today > useron.timelimit) {

                cause_error(E_TIMELMT);

            }

        } else {

            cause_error(E_UNKUSER);

        }

        /* load the sysop record */

        strcpy(username, "sysop");
        stat = user_find(users, username, strlen(username), find_user_by_name, &user_index);
        check_return(stat, users);

        if (user_index > 0) {

            stat = user_get(users, user_index, &sysop);
            check_return(stat, users);

        } else {

            cause_error(E_UNKUSER);

        }

        /* load the lobby record */

        stat = room_get(rooms, LOBBY, &room);
        check_return(stat, rooms);

        exit_when;

    } use {

        stat = ERR;
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

void cleanup(void) {

    room_close(rooms);
    room_destroy(rooms);

    user_close(users);
    user_destroy(users);

    node_close(nodes);
    node_destroy(nodes);

    door_close(doors);
    door_destroy(doors);

    tracer_destroy(dump);
    errors_destroy(errs);

}

int main(int argc, char **argv) {

    int stat = OK;

    when_error_in {

        stat = setup();
        check_status(stat, OK, E_INVOPS);

        stat = door_open(doors);
        check_return(stat, doors);

        stat = door_run(doors, &xnode, room, &sysop, &useron, NULL);
        check_return(stat, doors);

        stat = door_close(doors);
        check_return(stat, doors);

        exit_when;

    } use {

        capture_trace(dump);
        tracer_dump(dump, dump_trace);
        clear_error();

    } end_when;

    cleanup();

    return 0;

}



#include <stdio.h>
#include <errno.h>

#include "rms/files.h"
#include "gpl/fnm_util.h"
#include "include/when.h"
#include "tracer/tracer.h"
#include "errors/errors.h"
#include "cclibs/misc/misc.h"

#include "bbs/src/room/msgs.h"
#include "bbs/src/room/room.h"
#include "bbs/src/finds/finds.h"
#include "bbs/src/main/bbs_config.h"

room_t *room;
tracer_t *dump;
errors_t *errs;

int display(room_base_t *temp) {

    printf("--------------------------------\n");
    printf("room #    : %ld\n", temp->roomnum);
    printf("room aide : %ld\n", temp->aide);
    printf("base      : %d\n", temp->base);
    printf("timeout   : %d\n", temp->timeout);
    printf("retries   : %d\n", temp->retries);
    printf("conference: %d\n", temp->conference);
    printf("flags     : %d\n", temp->flags);
    printf("name      : %s\n", temp->name);
    printf("path      : %s\n", temp->path);
    printf("revision  : %d\n", temp->revision);

    return OK;

}

int dump_trace(char *buffer) {

    fprintf(stderr, "%s\n", buffer);

    return OK;

}

int setup(void) {

    int stat = OK;

    when_error_in {

        errs = errors_create();
        check_creation(errs);

        dump = tracer_create(errs);
        check_creation(dump);

        room = room_create(DATAPATH, ROOMNUM, RETRIES, TIMEOUT, MSGBASE, dump);
        check_creation(room);

        exit_when;

    } use {

        stat = ERR;
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

void cleanup(void) {

    room_destroy(room);
    tracer_destroy(dump);
    errors_destroy(errs);

}

int main(int argc, char **argv) {

    int stat = OK;
    int index = 0;
    short conference = 10;
    room_base_t *temp = NULL;

    when_error_in {

        stat = setup();
        check_status(stat, OK, E_INVOPS);

        stat = room_open(room);
        check_return(stat, room);

        stat = room_find(room, &conference, sizeof(short), find_room_by_conference, &index);
        check_return(stat, room);

        if (index > 0) {

            stat = room_get(room, index, &temp);
            check_return(stat, room);

            display(temp);

            temp->flags = (RM_PUBLIC | RM_INUSE);

            stat = room_put(room, index, temp);
            check_return(stat, room);

            stat = room_free(room, temp);
            check_return(stat, room);

            stat = room_get(room, index, &temp);
            check_return(stat, room);

            display(temp);

            stat = room_del(room, index);
            check_return(stat, room);

            stat = room_free(room, temp);
            check_return(stat, room);

            stat = room_get(room, index, &temp);
            check_return(stat, room);

            display(temp);

            stat = room_free(room, temp);
            check_return(stat, room);

        }

        stat = room_close(room);
        check_return(stat, room);

        exit_when;

    } use {

        capture_trace(dump);
        tracer_dump(dump, dump_trace);
        clear_error();

    } end_when;

    cleanup();

    return 0;

}


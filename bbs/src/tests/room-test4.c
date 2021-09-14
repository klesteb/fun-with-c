
#include <stdio.h>
#include <errno.h>

#include "rms/files.h"
#include "gpl/fnm_util.h"
#include "include/when.h"
#include "tracer/tracer.h"
#include "errors/errors.h"
#include "cclibs/misc/misc.h"

#include "bbs/src/jam/jam.h"
#include "bbs/src/room/msgs.h"
#include "bbs/src/room/room.h"
#include "bbs/src/finds/finds.h"
#include "bbs/src/main/bbs_config.h"

room_t *room;
tracer_t *dump;
errors_t *errs;

int display(room_base_t *room, int size) {

    printf("---------------------------------\n");
    printf("room #    : %ld\n", room->roomnum);
    printf("room aide : %ld\n", room->aide);
    printf("name      : %s\n", room->name);
    printf("path      : %s\n", room->path);
    printf("conference: %d\n", room->conference);
    printf("flags     : %d\n", room->flags);
    printf("revision  : %d\n", room->revision);
    printf("room size : %d\n", size);

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
    queue results;
    ssize_t size = 0;
    room_base_t temp1;
    room_base_t *temp2;
    jam_t *jam = NULL;
    short conference = 10;
    room_search_t *result = NULL;
    char *msgpath = "../../messages/";

    when_error_in {

        stat = que_init(&results);
        check_status(stat, QUE_OK, E_INVOPS);

        stat = setup();
        check_status(stat, OK, E_INVOPS);

        stat = room_open(room);
        check_return(stat, room);

        temp1.aide = 1;
        temp1.base = 1;
        temp1.timeout = 1;
        temp1.retries = 30;
        strcpy(temp1.name, "Testing");
        temp1.conference = conference;
        temp1.flags = (RM_PERMROOM | RM_PUBLIC | RM_INUSE | RM_MESSAGES);
        strncpy(temp1.path, fnm_build(1, FnmPath, msgpath, NULL), 255);

        stat = room_add(room, &temp1);
        check_return(stat, room);

        stat = room_search(room, NULL, 0, find_rooms_messages, &results);
        check_return(stat, room);

        while ((result = que_pop_head(&results))) {

            stat = room_get(room, result->index, &temp2);
            check_return(stat, room);

            stat = room_handler(room, temp2, &jam);
            check_return(stat, room);

            if (jam != NULL) {

                stat = jam_size(jam, &size);
                check_return(stat, jam);

            }

            display(temp2, size);

            stat = room_free(room, temp2);
            check_return(stat, room);

            free(result);

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



#include <stdio.h>
#include <errno.h>

#include "room.h"
#include "when.h"
#include "files.h"
#include "errors.h"
#include "tracer.h"
#include "misc/misc.h"

room_t *room;
tracer_t *dump;
errors_t *errs;

int dump_trace(char *buffer) {

    fprintf(stderr, "%s\n", buffer);

    return OK;

}

int setup(void) {

    int base = 1;
    int stat = OK;
    int rooms = 32;
    int timeout = 1;
    int retries = 30;
    char *dbpath = "../../data/";
    char *msgpath = "../../messages/";

    when_error_in {

        errs = errors_create();
        check_creation(errs);

        dump = tracer_create(errs);
        check_creation(dump);

        room = room_create(dbpath, msgpath, rooms, retries, base, timeout, dump);
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

    when_error_in {

        stat = setup();
        check_status(stat, OK, E_INVOPS);

        stat = room_open(room);
        check_return(stat, room);

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



#include <stdio.h>
#include <errno.h>

#include "jam.h"
#include "room.h"
#include "when.h"
#include "files.h"
#include "errors.h"
#include "tracer.h"
#include "fnm_util.h"
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

        room = room_create(dbpath, msgpath, rooms, retries, timeout, base, dump);
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
    room_base_t temp1;
    room_base_t temp2;
    ssize_t count = 0;
    short conference = 10;
    char *msgpath = "../messages/";
    
    when_error_in {

        stat = setup();
        check_status(stat, OK, E_INVOPS);

        stat = room_open(room);
        check_return(stat, room);

        temp1.base = 1;
        temp1.timeout = 1;
        temp1.retries = 30;
        strcpy(temp1.name, "Testing");
        temp1.conference = conference;
        temp1.flags = (PERMROOM | PUBLIC | INUSE);
        strncpy(temp1.path, fnm_build(1, FnmPath, msgpath, NULL), 255);

        stat = room_add(room, &temp1);
        check_return(stat, room);

        stat = room_first(room, &temp2, &count);
        check_return(stat, room);

        while (count > 0) {

            if (temp2.conference == conference) {

                printf("room      : %ld\n", temp2.roomnum);
                printf("base      : %d\n", temp2.base);
                printf("timeout   : %d\n", temp2.timeout);
                printf("retries   : %d\n", temp2.retries);
                printf("conference: %d\n", temp2.conference);
                printf("flags     : %d\n", temp2.flags);
                printf("name      : %s\n", temp2.name);
                printf("path      : %s\n", temp2.path);
                printf("revision  : %d\n", temp2.revision);
                
                break;
                
            }
            
            stat = room_next(room, &temp2, &count);
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


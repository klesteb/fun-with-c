
#include <stdio.h>
#include <errno.h>

#include "room.h"
#include "when.h"
#include "finds.h"
#include "files.h"
#include "bitops.h"
#include "errors.h"
#include "tracer.h"
#include "que_util.h"
#include "misc/misc.h"

room_t *room;
tracer_t *dump;
errors_t *errs;

int display(room_base_t *room) {

    printf("---------------------------------\n");
    printf("room #    : %ld\n", room->roomnum);
    printf("room aide : %ld\n", room->aide);
    printf("name      : %s\n", room->name);
    printf("path      : %s\n", room->path);
    printf("conference: %d\n", room->conference);
    printf("flags     : %d\n", room->flags);

    if (bit_test(room->flags, RM_INUSE)) printf("  RM_INUSE\n");
    if (bit_test(room->flags, RM_PUBLIC)) printf("  RM_PUBLIC\n");
    if (bit_test(room->flags, RM_PRIVATE)) printf("  RM_PRIVATE\n");
    if (bit_test(room->flags, RM_PREFONLY)) printf("  RM_PERFONLY\n");
    if (bit_test(room->flags, RM_PERMROOM)) printf("  RM_PERMROOM\n");
    if (bit_test(room->flags, RM_FORGOTTEN)) printf("  RM_FORGOTTEN\n");
    if (bit_test(room->flags, RM_NETWORK)) printf("  RM_NETWORK\n");
    if (bit_test(room->flags, RM_READONLY)) printf("  RM_READONLY\n");
    if (bit_test(room->flags, RM_UPLOAD)) printf("  RM_UPLOAD\n");
    if (bit_test(room->flags, RM_DOWNLOAD)) printf("  RM_DOWNLOAD\n");
    if (bit_test(room->flags, RM_MESSAGES)) printf("  RM_MESSAGES\n");
    if (bit_test(room->flags, RM_BULLETIN)) printf("  RM_BULLENTIN\n");
    if (bit_test(room->flags, RM_DIRECTORY)) printf("  RM_DIRECTORY\n");
    if (bit_test(room->flags, RM_SUBSYS)) printf("  RM_SUBSYS\n");
    
    printf("revision  : %d\n", room->revision);

    return OK;

}

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
    queue results;
    room_base_t *temp = NULL;
    room_search_t *result = NULL;

    when_error_in {

        stat = setup();
        check_status(stat, OK, E_INVOPS);

        stat = que_init(&results);
        check_status(stat, QUE_OK, E_INVOPS);

        stat = room_open(room);
        check_return(stat, room);

        stat = room_search(room, NULL, 0, find_rooms_all, &results);
        check_return(stat, room);

        while ((result = que_pop_head(&results))) {

            stat = room_get(room, result->index, &temp);
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


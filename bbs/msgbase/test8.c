
#include <stdio.h>
#include <errno.h>

#include "jam.h"
#include "when.h"
#include "errors.h"
#include "tracer.h"
#include "misc/misc.h"

jam_t *jam;
tracer_t *dump;
errors_t *errors;

int dump_trace(char *buffer) {

    fprintf(stderr, "%s\n", buffer);

    return OK;

}

int main(int argc, char **argv) {

    int base = 1;
    int stat = OK;
    int timeout = 1;
    char *path = ".";
    char retries = 30;
    ulong user_crc = 0;
    char *name = "001";
    char *sysop = "sysop";
    jam_lastread_t lastread;

    when_error_in {

        errors = errors_create();
        check_creation(errors);

        dump = tracer_create(errors);
        check_creation(dump);

        jam = jam_create(path, name, retries, timeout, base, dump);
        check_creation(jam);

        stat = jam_open(jam);
        check_return(stat, jam);

        stat = jam_crc32(jam, (uchar *)sysop, strlen(sysop), &user_crc);
        check_return(stat, jam);

        stat = jam_get_lastread(jam, user_crc, &lastread);
        check_return(stat, jam);

        printf("user id  : %ld\n", lastread.user_id);
        printf("user crc : %ld\n", lastread.user_crc);
        printf("last read: %ld\n", lastread.last_read_msg);
        printf("high read: %ld\n", lastread.high_read_msg);

        stat = jam_close(jam);
        check_return(stat, jam);

        jam_destroy(jam);
        tracer_destroy(dump);
        errors_destroy(errors);

        exit_when;

    } use {

        capture_trace(dump);
        tracer_dump(dump, dump_trace);
        clear_error();

    } end_when;

    return 0;

}


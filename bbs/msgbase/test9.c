
#include <stdio.h>
#include <errno.h>

#include "jam.h"
#include "when.h"
#include "errors.h"
#include "tracer.h"
#include "misc/misc.h"

jam_t *jam;
tracer_t *dump;
errors_t *errs;

int dump_trace(char *buffer) {

    fprintf(stderr, "%s\n", buffer);

    return OK;

}

int setup(void) {

    int base = 1;
    int stat = OK;
    int timeout = 1;
    char *path = ".";
    char retries = 30;
    char *name = "001";

    when_error_in {

        errs = errors_create();
        check_creation(errs);

        dump = tracer_create(errs);
        check_creation(dump);

        jam = jam_create(path, name, retries, timeout, base, dump);
        check_creation(jam);

        exit_when;

    } use {

        stat = ERR;
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

void cleanup(void) {

    jam_destroy(jam);
    tracer_destroy(dump);
    errors_destroy(errs);

}

int main(int argc, char **argv) {

    int stat = OK;
    ssize_t msgs = 0;
    ssize_t msgnum = 0;

    when_error_in {

        stat = setup();
        check_status(stat, OK, E_INVOPS);

        stat = jam_open(jam);
        check_return(stat, jam);

        stat = jam_size(jam, &msgs);
        check_return(stat, jam);

        msgnum = msgs / 2;

        printf("deleting: %ld\n", msgnum + 1);

        jam_del_message(jam, msgnum);
        check_return(stat, jam);

        stat = jam_close(jam);
        check_return(stat, jam);

        exit_when;

    } use {

        capture_trace(dump);
        tracer_dump(dump, dump_trace);
        clear_error();

    } end_when;

    cleanup();

    return 0;

}


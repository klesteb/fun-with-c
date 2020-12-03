
#include <stdio.h>
#include <errno.h>

#include "jam.h"
#include "when.h"
#include "errors.h"
#include "tracer.h"

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
    char *name = "001";
    jam_t *temp = NULL;

    when_error_in {

        errors = errors_create();
        check_creation(errors);

        dump = tracer_create(errors);
        check_creation(dump);

        temp = jam_create(path, name, retries, timeout, base, dump);
        check_creation(temp);

        stat = jam_open(temp);
        check_return(stat, temp);

        stat = jam_remove(temp);
        check_return(stat, temp);

        jam_destroy(temp);
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



#include <stdio.h>
#include <errno.h>

#include "node.h"
#include "when.h"
#include "files.h"
#include "errors.h"
#include "tracer.h"
#include "misc/misc.h"

node_t *nodes;
tracer_t *dump;
errors_t *errs;

int dump_trace(char *buffer) {

    fprintf(stderr, "%s\n", buffer);

    return OK;

}

int setup(void) {

    int stat = OK;
    int timeout = 1;
    int retries = 30;
    char *path = "../../data/";

    when_error_in {

        errs = errors_create();
        check_creation(errs);

        dump = tracer_create(errs);
        check_creation(dump);

        nodes = node_create(path, 32, retries, timeout, dump);
        check_creation(nodes);

        exit_when;

    } use {

        stat = ERR;
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

void cleanup(void) {

    node_destroy(nodes);
    tracer_destroy(dump);
    errors_destroy(errs);

}

int main(int argc, char **argv) {

    int stat = OK;

    when_error_in {

        stat = setup();
        check_status(stat, OK, E_INVOPS);

        stat = node_open(nodes);
        check_return(stat, nodes);

        stat = node_close(nodes);
        check_return(stat, nodes);

        exit_when;

    } use {

        capture_trace(dump);
        tracer_dump(dump, dump_trace);
        clear_error();

    } end_when;

    cleanup();

    return 0;

}


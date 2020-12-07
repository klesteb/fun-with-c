
#include <stdio.h>
#include <errno.h>

#include "jam.h"
#include "qwk.h"
#include "when.h"
#include "errors.h"
#include "tracer.h"
#include "misc/misc.h"

jam_t *jam;
qwk_t *qwk;
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

        qwk = qwk_create(path, retries, timeout, FALSE, dump);
        check_creation(qwk);

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
    qwk_destroy(qwk);
    tracer_destroy(dump);
    errors_destroy(errs);

}

int main(int argc, char **argv) {

    int stat = OK;

    when_error_in {

        stat = setup();
        check_status(stat, OK, E_INVOPS);

        stat = qwk_open(qwk);
        check_return(stat, qwk);
        
        stat = qwk_open_ndx(qwk, "001");
        check_return(stat, qwk);
        
        stat = qwk_close(qwk);
        check_return(stat, qwk);
        
        exit_when;

    } use {

        capture_trace(dump);
        tracer_dump(dump, dump_trace);
        clear_error();

    } end_when;

    cleanup();

    return 0;

}


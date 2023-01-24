
#include <stdio.h>
#include <errno.h>

#include "rms/files.h"
#include "include/when.h"
#include "errors/errors.h"
#include "tracer/tracer.h"
#include "cclibs/misc/misc.h"
#include "bbs/src/qwk/qwk.h"

qwk_t *qwk;
tracer_t *dump;
errors_t *errs;

int dump_trace(char *buffer) {

    fprintf(stderr, "%s\n", buffer);

    return OK;

}

int setup(void) {

    int stat = OK;
    int timeout = 1;
    char *path = ".";
    char retries = 30;

    when_error_in {

        errs = errors_create();
        check_creation(errs);

        dump = tracer_create(errs);
        check_creation(dump);

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

    qwk_destroy(qwk);
    tracer_destroy(dump);
    errors_destroy(errs);

}

int main(int argc, char **argv) {

    int stat = OK;
    qwk_ndx_t ndx;
    ssize_t count = 0;

    when_error_in {

        ndx.index = 128;
        ndx.conference = 1;

        stat = setup();
        check_status(stat, OK, E_INVOPS);

        stat = qwk_open(qwk);
        check_return(stat, qwk);

        stat = qwk_open_ndx(qwk, "001");
        check_return(stat, qwk);

        stat = qwk_get_ndx(qwk, &ndx, &count);
        check_return(stat, qwk);

        do {
            
            printf("index     : %ld\n", ndx.index);
            printf("conference: %d\n", ndx.conference);

            stat = qwk_get_ndx(qwk, &ndx, &count);
            check_return(stat, qwk);

        } while (count > 0);

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


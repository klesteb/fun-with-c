
#include <stdio.h>
#include <log4c.h>

#include "logger/logger.h"
#include "errors/errors.h"
#include "tracer/tracer.h"
#include "messages/message.h"
#include "include/error_codes.h"

errors_t *errs = NULL;
logger_t *logs = NULL;
message_t *msgs = NULL;
tracer_t *dump = NULL;

#undef process_error
#define process_error() { \
    capture_trace(dump);  \
    clear_error();        \
}


int setup(void) {

    int stat = OK;
    char *process = "test9";
    char *category = "test9";
    char *facility = "system";

    when_error_in {

        logs = log_create(facility, category, process);
        check_creation(logs);

        errs = errors_create();
        check_creation(errs);

        msgs = message_create(NULL, 0);
        check_creation(msgs);

        dump = tracer_create(errs);
        check_creation(dump);

        exit_when;

    } use {

        process_error();
        stat = ERR;

    } end_when;

    return stat;

}

int print_dump(char *string) {

    char *fmt = "%s";

    log_error(logs, fmt, string);
    return OK;

}

void cleanup(void) {

    log_destroy(logs);
    errors_destroy(errs);
    message_destroy(msgs);

}

int test(void) {

    int stat = OK;
    int retries = 0;

    when_error {

        AGAIN:
        cause_error(E_INVOPS);
        exit_when;

    } use {

        retries++;
        if (retries < 2) retry(AGAIN);

        process_error();
        stat = ERR;

    } end_when;

    return stat;

}

int main(int argc, char **argv) {

    int stat = OK;
    int rc = EXIT_SUCCESS;

    atexit(cleanup);

    when_error_in {

        stat = setup();
        check_status(stat, OK, E_INVOPS);

        stat = test();
        check_status(stat, OK, E_INVOPS);

        exit_when;

    } use {

        rc = EXIT_FAILURE;
        process_error();
        tracer_dump(dump, print_dump);

    } end_when;

    return rc;

}

